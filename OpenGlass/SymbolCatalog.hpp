#pragma once
#include <cstdio>
#include <cstdint>
#include <string>
#include <string_view>
#include "ProjectionHelper.hpp"
#include "SymbolDownloader.hpp"

// Minimal pre-resolved symbol catalog support.
//
// Upstream ships a compiled-in catalog (symbol key -> RVA, keyed by PDB GUID + age)
// so a build whose PDB is unavailable from the Microsoft symbol server can still run.
// Here the same data is read at runtime from a JSON file, which keeps the DLL small and
// makes the fallback opt-in: when the identity matches, every projection entry is filled
// straight from the RVAs and no PDB is loaded or downloaded.
namespace OpenGlass::SymbolCatalog
{
	namespace Detail
	{
		inline bool ReadTextFile(PCWSTR path, std::string& text)
		{
			wil::unique_hfile file
			{
				CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)
			};
			if (!file.is_valid())
			{
				return false;
			}

			LARGE_INTEGER size{};
			if (!GetFileSizeEx(file.get(), &size) || size.QuadPart <= 0 || size.QuadPart > 64ll * 1024 * 1024)
			{
				return false;
			}

			text.resize(static_cast<size_t>(size.QuadPart));
			DWORD read{};
			return ReadFile(file.get(), text.data(), static_cast<DWORD>(text.size()), &read, nullptr) && read == text.size();
		}

		inline void FormatGuid(const GUID& guid, char (&buffer)[40])
		{
			sprintf_s(
				buffer,
				"%08lx-%04hx-%04hx-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
				guid.Data1,
				guid.Data2,
				guid.Data3,
				guid.Data4[0], guid.Data4[1],
				guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
			);
		}

		inline char LowerCase(char value)
		{
			return (value >= 'A' && value <= 'Z') ? static_cast<char>(value - 'A' + 'a') : value;
		}

		// Case insensitive search for a needle inside the catalog text.
		inline size_t FindNoCase(const std::string& text, std::string_view needle, size_t start)
		{
			if (needle.empty() || text.size() < needle.size())
			{
				return std::string::npos;
			}
			for (auto i = start; i + needle.size() <= text.size(); ++i)
			{
				size_t j = 0;
				while (j < needle.size() && LowerCase(text[i + j]) == LowerCase(needle[j]))
				{
					++j;
				}
				if (j == needle.size())
				{
					return i;
				}
			}
			return std::string::npos;
		}

		inline uint32_t ParseHex(const std::string& text, size_t pos)
		{
			// Skip to the opening quote of the value, then accept an optional 0x prefix.
			auto quote = text.find('"', pos);
			if (quote == std::string::npos)
			{
				return 0;
			}
			auto i = quote + 1;
			if (i + 1 < text.size() && text[i] == '0' && LowerCase(text[i + 1]) == 'x')
			{
				i += 2;
			}

			uint32_t value{};
			bool any{ false };
			for (; i < text.size(); ++i)
			{
				const auto c = LowerCase(text[i]);
				uint32_t digit{};
				if (c >= '0' && c <= '9')
				{
					digit = static_cast<uint32_t>(c - '0');
				}
				else if (c >= 'a' && c <= 'f')
				{
					digit = static_cast<uint32_t>(c - 'a' + 10);
				}
				else
				{
					break;
				}
				value = value * 16u + digit;
				any = true;
			}
			return any ? value : 0;
		}

		inline uint32_t ParseDecimal(const std::string& text, size_t pos)
		{
			auto colon = text.find(':', pos);
			if (colon == std::string::npos)
			{
				return 0;
			}
			uint32_t value{};
			bool any{ false };
			for (auto i = colon + 1; i < text.size(); ++i)
			{
				const auto c = text[i];
				if (c < '0' || c > '9')
				{
					if (any)
					{
						break;
					}
					if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
					{
						continue;
					}
					return 0;
				}
				value = value * 10u + static_cast<uint32_t>(c - '0');
				any = true;
			}
			return any ? value : 0;
		}

		inline bool ReadResourceText(int resourceId, std::string& text)
		{
			const auto module = wil::GetModuleInstanceHandle();
			const auto resource = FindResourceW(module, MAKEINTRESOURCEW(resourceId), RT_RCDATA);
			if (!resource)
			{
				return false;
			}

			const auto size = SizeofResource(module, resource);
			const auto data = LoadResource(module, resource);
			if (!size || !data)
			{
				return false;
			}

			const auto bytes = static_cast<const char*>(LockResource(data));
			if (!bytes)
			{
				return false;
			}

			text.assign(bytes, size);
			return true;
		}

		inline bool ContainsIdentity(const std::string& text, const CPdbIdentity& identity)
		{
			char guid[40]{};
			FormatGuid(identity.guid, guid);

			const auto guidPos = FindNoCase(text, guid, 0);
			if (guidPos == std::string::npos)
			{
				return false;
			}

			// The age follows the GUID inside the same "pdb" object.
			const auto agePos = FindNoCase(text, "\"age\"", guidPos);
			if (agePos == std::string::npos || agePos - guidPos > 512)
			{
				return false;
			}
			return ParseDecimal(text, agePos) == identity.age;
		}

		inline bool FindRva(const std::string& text, std::string_view symbolName, uint32_t& rva)
		{
			std::string needle;
			needle.reserve(symbolName.size() + 1);
			needle.push_back('"');
			needle.append(symbolName);

			// catalog 的 key 会带版本后缀（Name.22000），后面允许引号或点
			auto keyPos = text.find(needle);
			while (keyPos != std::string::npos)
			{
				const auto afterName = keyPos + needle.size();
				if (afterName < text.size() && (text[afterName] == '"' || text[afterName] == '.'))
				{
					break;
				}
				keyPos = text.find(needle, keyPos + 1);
			}
			if (keyPos == std::string::npos)
			{
				return false;
			}

			// Keep the search inside the same symbol object.
			const auto rvaPos = text.find("\"rva\"", keyPos);
			if (rvaPos == std::string::npos || rvaPos - keyPos > 4096)
			{
				return false;
			}

			rva = ParseHex(text, rvaPos);
			return rva != 0;
		}
	}

	// Fills every projection entry whose symbol name appears in the catalog text.
	template <size_t N>
	inline size_t Apply(ProjectionArray<N>& array, PBYTE imageBase, const std::string& text)
	{
		size_t applied{};
		for (auto& entry : array)
		{
			uint32_t rva{};
			if (!Detail::FindRva(text, std::get<1>(entry), rva))
			{
				continue;
			}
			std::get<3>(entry) = imageBase + rva;
			++applied;
		}

		if (applied)
		{
			// A pre-filled table must be re-scanned from the beginning by ReportMissing.
			array._lastEmptyEntryIndex = 0;
		}

		return applied;
	}

	// Applies the catalog embedded as an RCDATA resource. Returns 0 when the resource is not
	// present or its PDB identity does not match the module.
	template <size_t N>
	inline size_t TryApplyFromResource(ProjectionArray<N>& array, HMODULE module, int resourceId) noexcept
	try
	{
		CPdbIdentity identity{};
		if (FAILED(GetModulePdbIdentity(module, identity)))
		{
			return 0;
		}

		std::string text{};
		if (!Detail::ReadResourceText(resourceId, text))
		{
			return 0;
		}

		if (!Detail::ContainsIdentity(text, identity))
		{
			return 0;
		}

		return Apply(array, reinterpret_cast<PBYTE>(module), text);
	}
	catch (...)
	{
		return 0;
	}

	// Loads the catalog file and applies it when its PDB identity matches the module.
	template <size_t N>
	inline size_t TryApplyFromFile(ProjectionArray<N>& array, HMODULE module, PCWSTR jsonPath) noexcept
	try
	{
		CPdbIdentity identity{};
		if (FAILED(GetModulePdbIdentity(module, identity)))
		{
			return 0;
		}

		std::string text{};
		if (!Detail::ReadTextFile(jsonPath, text))
		{
			return 0;
		}

		if (!Detail::ContainsIdentity(text, identity))
		{
			return 0;
		}

		return Apply(array, reinterpret_cast<PBYTE>(module), text);
	}
	catch (...)
	{
		return 0;
	}
}
