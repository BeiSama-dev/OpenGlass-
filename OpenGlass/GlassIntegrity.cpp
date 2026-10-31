#include "pch.h"
#include "HookHelper.hpp"
#include "uDWMProjection.hpp"
#include "dwmcoreProjection.hpp"
#include "Shared.hpp"
#include "GlassKernel.hpp"
#include "GlassIntegrity.hpp"
#include "GlassSafetyZoneLayer.hpp"
#include "GlassCoverageSet.hpp"
#include "GlassRenderer.hpp"

using namespace OpenGlass;

namespace OpenGlass::GlassIntegrity
{
	HRESULT MyCOcclusionContext_CheckAndRecordOverlayCandidate(
		dwmcore::COcclusionContext* This,
		dwmcore::CVisual* visual,
		void* surface,
		const dwmcore::CMILMatrix* matrix,
		const dwmcore::CShape* shape,
		int flags
	);
	HRESULT MyCOcclusionContext_CollectRectangleForOcclusion(
		dwmcore::COcclusionContext* This,
		const D2D1_RECT_F* rectangle,
		void* param3,
		void* param4,
		D2D1_RECT_F* param5
	);

	HRESULT MyCOcclusionContext_Compute_Pre_W10_2004(
		dwmcore::COcclusionContext* This,
		const dwmcore::CVisualTree* visualTree,
		UINT count,
		const D2D1_RECT_F* rectangles,
		float unknown1,
		bool unknown2,
		const dwmcore::CMILMatrix* matrix,
		const DWM::span<dwmcore::COverlayContext*>& overlays
	);
	HRESULT MyCOcclusionContext_Compute(
		dwmcore::COcclusionContext* This,
		const dwmcore::CVisualTree* visualTree,
		const DWM::span<D2D1_RECT_F>& rectangles,
		float unknown,
		const DWM::span<dwmcore::COverlayContext*>& overlays
	);
	HRESULT MyCOcclusionContext_DrawGeometry(
		dwmcore::IDrawingContext* This,
		dwmcore::CLegacyMilBrush* brush,
		dwmcore::CGeometry* geometry
	);
	HRESULT MyCOcclusionContext_SetDeviceTransform(
		dwmcore::COcclusionContext* This,
		const dwmcore::CMILMatrix* matrix
	);
	void MyCOcclusionContext_Destructor(dwmcore::COcclusionContext* This);

	bool MyCArrayBasedCoverageSet_IsCovered(
		dwmcore::CArrayBasedCoverageSet* This,
		const D2D1_RECT_F& coverage,
		int depth
	);
	bool MyCOcclusionContext_IsOccluded(
		dwmcore::COcclusionContext* This,
		const D2D1_RECT_F& coverage,
		int depth,
		bool ignoreDeviceTransform
	);
	HRESULT MyCOcclusionContext_PageInPixelsRectToDeviceRect(
		dwmcore::COcclusionContext* This,
		const D2D1_RECT_F& src,
		D2D1_RECT_F* dst
	);
	HRESULT MyCHwndRenderTarget_RenderDirtyRegion(
		dwmcore::CHwndRenderTarget* This,
		dwmcore::CDrawingContext* drawingContext,
		dwmcore::CComposeTop* composeTop
	);
	D2D1_RECT_F* MyCDirtyRegion_GetUnOccludedDirtyRegion(
		dwmcore::CDirtyRegion* This,
		D2D1_RECT_F* dirtyRect,
		dwmcore::COcclusionContext* occlusionContext,
		const dwmcore::CVisualTree* tree,
		bool inflate,
		unsigned int i,
		const D2D1_RECT_F& bounds
	);
	D2D1_RECT_F* MyCDirtyRegion_GetUnOccludedDirtyRect(
		dwmcore::CDirtyRegion* This,
		D2D1_RECT_F* dirtyRect,
		int i,
		const D2D1_RECT_F& bounds,
		bool useSuperSample,
		const DWM::span<dwmcore::CVisual>& visuals,
		dwmcore::COcclusionContext* occlusionContext
	);
	D2D1_RECT_F* MyCDirtyRegion_GetOptimizedRect_WS2022(
		dwmcore::CDirtyRegion* This,
		D2D1_RECT_F* dirtyRect,
		int i,
		const D2D1_RECT_F& bounds,
		const D2D1_SIZE_U& size,
		bool transform,
		const dwmcore::CMILMatrix* matrix,
		const DWM::span<dwmcore::CVisual>& visuals,
		const dwmcore::CRegion* region,
		dwmcore::COcclusionContext* occlusionContext
	);
	D2D1_RECT_F* MyCDirtyRegion_GetOptimizedRect(
		dwmcore::CDirtyRegion* This,
		D2D1_RECT_F* dirtyRect,
		int i,
		const D2D1_RECT_F& bounds,
		const dwmcore::CRegion* region,
		const dwmcore::CMILMatrix* matrix,
		bool useSuperSample,
		const DWM::span<dwmcore::CVisual>& visuals,
		dwmcore::COcclusionContext* occlusionContext
	);
	D2D1_RECT_F* MyCTreeDirty_GetOptimizedRect(
		dwmcore::CTreeDirty* This,
		D2D1_RECT_F* dirtyRect,
		UINT i,
		const D2D1_RECT_F& bounds,
		dwmcore::COcclusionContext* occlusionContext,
		const dwmcore::CRegion* region,
		const dwmcore::CMILMatrix* matrix,
		bool useSuperSample,
		const DWM::span<dwmcore::CVisual>& visuals
	);

	template <typename T>
	HRESULT MyCDrawingContext_DrawVisualTree(
		dwmcore::CDrawingContext* This,
		const D2D1_RECT_F& rectangle,
		dwmcore::COcclusionContext* occlusionContext,
		T&& callback
	);
	HRESULT MyCDrawingContext_DrawVisualTree_Pre_Win10_2004(
		dwmcore::CDrawingContext* This,
		dwmcore::CVisualTree* tree,
		const D2D1_RECT_F& rectangle,
		dwmcore::COverlayContext* overlayContext,
		int unknown1,
		bool unknown2,
		bool unknown3,
		bool useOcclusionContext,
		const D2D1_RECT_F* unknown4,
		bool unknown5,
		bool unknown6,
		bool unknown7
	);
	HRESULT MyCDrawingContext_DrawVisualTree_Win10(
		dwmcore::CDrawingContext* This,
		dwmcore::CVisualTree* tree,
		const D2D1_RECT_F& rectangle,
		dwmcore::COcclusionContext* occlusionContext,
		int clearMode,
		bool useSuperSample
	);
	HRESULT MyCDrawingContext_DrawVisualTree_Win11(
		dwmcore::CDrawingContext* This,
		dwmcore::CVisualTree* tree,
		const D2D1_RECT_F& rectangle,
		dwmcore::COcclusionContext* occlusionContext,
		int clearMode,
		bool useSuperSample,
		dwmcore::CVisual* visualOverride
	);

	decltype(&MyCOcclusionContext_CheckAndRecordOverlayCandidate) g_COcclusionContext_CheckAndRecordOverlayCandidate_Org{ nullptr };
	union CollectRectangleForOcclusionOriginal
	{
		decltype(&MyCOcclusionContext_CollectRectangleForOcclusion) generic{ nullptr };
		HRESULT(*pre22000)(dwmcore::COcclusionContext*, const D2D1_RECT_F*, bool, D2D1_RECT_F*);
		void(*w11_22000)(dwmcore::COcclusionContext*, const D2D1_RECT_F*, bool, bool, D2D1_RECT_F*);
		void(*w11_24h2_pre7840)(dwmcore::COcclusionContext*, const D2D1_RECT_F&, D2D1_RECT_F*);
		void(*w11_24h2_7840)(dwmcore::COcclusionContext*, const D2D1_RECT_F&, bool, D2D1_RECT_F*);
	};
	CollectRectangleForOcclusionOriginal g_COcclusionContext_CollectRectangleForOcclusion_Org{};

	static union
	{
		decltype(&MyCOcclusionContext_Compute_Pre_W10_2004) g_COcclusionContext_Compute_Pre_W10_2004_Org;
		decltype(&MyCOcclusionContext_Compute) g_COcclusionContext_Compute_Org{ nullptr };
	};
	decltype(&MyCOcclusionContext_DrawGeometry) g_COcclusionContext_DrawGeometry_Org{ nullptr };
	decltype(&MyCOcclusionContext_DrawGeometry)* g_COcclusionContext_DrawGeometry_Org_Address{ nullptr };
	decltype(&MyCOcclusionContext_SetDeviceTransform) g_COcclusionContext_SetDeviceTransform_Org{ nullptr };
	decltype(&MyCOcclusionContext_Destructor) g_COcclusionContext_Destructor_Org{ nullptr };

	decltype(&MyCArrayBasedCoverageSet_IsCovered) g_CArrayBasedCoverageSet_IsCovered_Org{ nullptr };
	decltype(&MyCOcclusionContext_IsOccluded) g_COcclusionContext_IsOccluded_Org{ nullptr };
	decltype(&MyCOcclusionContext_PageInPixelsRectToDeviceRect) g_COcclusionContext_PageInPixelsRectToDeviceRect_Org{ nullptr };
	decltype(&MyCHwndRenderTarget_RenderDirtyRegion) g_CHwndRenderTarget_RenderDirtyRegion_Org{ nullptr };

	decltype(&MyCDirtyRegion_GetUnOccludedDirtyRegion) g_CDirtyRegion_GetUnOccludedDirtyRegion_Org{ nullptr };
	decltype(&MyCDirtyRegion_GetUnOccludedDirtyRect) g_CDirtyRegion_GetUnOccludedDirtyRect_Org{ nullptr };
	static union
	{
		decltype(&MyCDirtyRegion_GetOptimizedRect_WS2022) g_CDirtyRegion_GetOptimizedRect_WS2022_Org;
		decltype(&MyCDirtyRegion_GetOptimizedRect) g_CDirtyRegion_GetOptimizedRect_Org{ nullptr };
	};
	decltype(&MyCTreeDirty_GetOptimizedRect) g_CTreeDirty_GetOptimizedRect_Org{ nullptr };

	static union
	{
		PVOID g_CDrawingContext_DrawVisualTree_Org{ nullptr };
		decltype(&MyCDrawingContext_DrawVisualTree_Pre_Win10_2004) g_CDrawingContext_DrawVisualTree_Pre_Win10_2004_Org;
		decltype(&MyCDrawingContext_DrawVisualTree_Win10) g_CDrawingContext_DrawVisualTree_Win10_Org;
		decltype(&MyCDrawingContext_DrawVisualTree_Win11) g_CDrawingContext_DrawVisualTree_Win11_Org;
	};

	Util::ObjectPool<dwmcore::CD2DContext*, CGlassSafetyZoneLayer> g_safetyZonePool{};

	std::unordered_map<dwmcore::COcclusionContext*, ULONGLONG> g_shrunkCoverageSetMap{};
	void ShrinkOccludersAboveGlass(dwmcore::COcclusionContext* occlusionContext);

	enum class GlassSafetyZoneMode : UCHAR
	{
		Disabled,
		Visible,
		Always
	};
	GlassSafetyZoneMode g_glassSafetyZoneMode{ GlassSafetyZoneMode::Visible };

	struct UnoccludedDirtyRegionCalculationContext
	{
		dwmcore::COcclusionContext* occlusionContext;
		D2D1_RECT_F* dirtyRect;
		dwmcore::CMILMatrix deviceTransform;
		UINT deviceTransformFlag;

		void Enter(dwmcore::COcclusionContext* context)
		{
			if (g_glassSafetyZoneMode != GlassSafetyZoneMode::Disabled && context)
			{
				occlusionContext = context;
				if (g_CArrayBasedCoverageSet_IsCovered_Org)
				{
					dirtyRect = nullptr;
					deviceTransformFlag = *occlusionContext->GetDeviceTransformFlag();
					if (!(deviceTransformFlag & 0x1))
					{
						deviceTransform = *occlusionContext->GetDeviceTransform();
						*occlusionContext->GetDeviceTransformFlag() |= 0x1;
						*const_cast<dwmcore::CMILMatrix*>(occlusionContext->GetDeviceTransform()) = *dwmcore::CMILMatrix::Identity;
					}
				}
			}
		}
		void Leave()
		{
			if (occlusionContext)
			{
				if (g_CArrayBasedCoverageSet_IsCovered_Org)
				{
					if (!(deviceTransformFlag & 0x1))
					{
						*occlusionContext->GetDeviceTransformFlag() = deviceTransformFlag;
						*const_cast<dwmcore::CMILMatrix*>(occlusionContext->GetDeviceTransform()) = deviceTransform;
					}
					dirtyRect = nullptr;
					deviceTransformFlag = 0;
				}
				occlusionContext = nullptr;
			}
		}
		bool IsActive()
		{
			return occlusionContext != nullptr;
		}
	};
	using udrcc_leave_scope_exit = wil::unique_any<UnoccludedDirtyRegionCalculationContext*, decltype(&UnoccludedDirtyRegionCalculationContext::Leave), &UnoccludedDirtyRegionCalculationContext::Leave, wil::details::pointer_access_none>;
	[[nodiscard]] inline udrcc_leave_scope_exit EnterUnoccludedDirtyRegionCalculationContext(UnoccludedDirtyRegionCalculationContext* pudrcc, dwmcore::COcclusionContext* context) noexcept
	{
		pudrcc->Enter(context);
		return udrcc_leave_scope_exit{ pudrcc };
	}
	UnoccludedDirtyRegionCalculationContext g_calculationContext{};

	uint16_t g_COcclusionContext_IsDeviceTransformAssigned_Instructions[] =
	{
		0x41, 0x80, 0xBF, HookHelper::c_patwc, 0x03, 0x00, 0x00, 0x00,	// cmp     byte ptr[r15 + 3xxh], 0
		0x74, 0x23,														// jz      short xx
	};
	uint8_t* g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation = nullptr;

	bool g_mpoCandidateFiltering{ true };

	struct CCollectedOcclusionRectangle
	{
		dwmcore::COcclusionContext* context{};
		D2D1_RECT_F rectangle{};
		int depth{ -1 };
	};
	CCollectedOcclusionRectangle g_lastCollectedOcclusionRectangle{};

	enum class CollectRectangleForOcclusionAbi : UCHAR
	{
		Pre_22000,
		W11_22000,
		W11_24H2_Pre7840,
		W11_24H2_7840
	};
	CollectRectangleForOcclusionAbi g_collectRectangleForOcclusionAbi{ CollectRectangleForOcclusionAbi::W11_24H2_7840 };

	struct CoverageSetCheckpoint
	{
		dwmcore::COcclusionContext* context;
		ULONGLONG id;
		std::unordered_map<dwmcore::CZOrderedRectBase*, D2D1_RECT_F> saves;
	};
	std::unordered_map<dwmcore::CArrayBasedCoverageSet*, CoverageSetCheckpoint> g_coverageSetCheckpointMap;

	void CaptureCollectedOcclusionRectangle(
		dwmcore::COcclusionContext* occlusionContext,
		const D2D1_RECT_F* rectangle
	)
	{
		g_lastCollectedOcclusionRectangle = {};
		if (rectangle)
		{
			g_lastCollectedOcclusionRectangle = {
				occlusionContext,
				*rectangle,
				static_cast<int>(occlusionContext->GetCurrentZ())
			};
		}
	}

	bool IsOverlayCandidateCoveredByGlass(
		dwmcore::COcclusionContext* occlusionContext,
		const dwmcore::CShape* shape
	)
	{
		const auto collected = g_lastCollectedOcclusionRectangle;
		g_lastCollectedOcclusionRectangle = {};

		if (!GlassKernel::GetBlurRadius())
		{
			return false;
		}

		const auto depth = static_cast<int>(occlusionContext->GetCurrentZ());
		D2D1_RECT_F deviceBounds{};
		if (shape)
		{
			D2D1_RECT_F bounds{};
			if (SUCCEEDED(shape->GetTightBounds(&bounds, occlusionContext->GetWorldTransform())))
			{
				deviceBounds = occlusionContext->PageInPixelsRectToDeviceRect(bounds);
			}
		}
		else if (
			collected.context == occlusionContext &&
			collected.depth == depth
		)
		{
			const auto worldBounds = RectF::TransformRect(
				collected.rectangle,
				occlusionContext->GetWorldTransform()->GetD2DMatrix()
			);
			deviceBounds = occlusionContext->PageInPixelsRectToDeviceRect(worldBounds);
		}

		if (wil::rect_is_empty(deviceBounds))
		{
			return false;
		}

		const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(occlusionContext->GetArrayBasedCoverageSet());
		return glassCoverageSet && glassCoverageSet->IsPartiallyCovered(deviceBounds, depth);
	}
}

void GlassIntegrity::ShrinkOccludersAboveGlass(dwmcore::COcclusionContext* occlusionContext)
{
	const auto frameId = dwmcore::g_versionInfo.build < os::build_w10_2004 ? dwmcore::GetCurrentFrameId() : occlusionContext->GetFrameId();
	const auto expansion = GlassKernel::GetBlurRadius();

	if (!expansion)
	{
		return;
	}
	const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(occlusionContext->GetArrayBasedCoverageSet());
	if (!glassCoverageSet || glassCoverageSet->IsEmpty())
	{
		return;
	}
	if (g_shrunkCoverageSetMap[occlusionContext] == frameId)
	{
		return;
	}
	else
	{
		g_shrunkCoverageSetMap[occlusionContext] = frameId;
	}

	enum ShrinkSide
	{
		ShrinkSide_Left,
		ShrinkSide_Top,
		ShrinkSide_Right,
		ShrinkSide_Bottom
	};
	std::unordered_set<const dwmcore::CZOrderedRect*> visibleGlassSet{};

	const auto coverageSet = occlusionContext->GetArrayBasedCoverageSet();
	const auto matrix = occlusionContext->GetDeviceTransform();
	// 从 Compute 进来，和 IsCovered 有没有挂钩无关：没挂上时调它会跳进无效函数指针
	if (!Shared::g_coverageSetIsCoveredHooked)
	{
		return;
	}
	for (const auto& glassRegion : glassCoverageSet->GetViews())
	{
		if (
			!coverageSet->IsCovered(
				glassRegion.m_transformedRect,
				glassRegion.m_depth
			)
		)
		{
			visibleGlassSet.insert(&glassRegion);
		}
	}

	const auto collectAndTryShrinkOccluders = [visibleGlassSet, expansion, &matrix, occlusionContext, coverageSet, frameId](auto&& views)
	{
		using CZOrderedRectT = std::remove_reference_t<decltype(views)>::value_type;
		std::unordered_map<CZOrderedRectT*, std::bitset<4>> targetOccluderSet{};

		for (const auto& glassRegion : visibleGlassSet)
		{
			for (auto& occluder : views)
			{
				if (occluder.m_depth >= glassRegion->m_depth)
				{
					break;
				}

				const D2D1_RECT_F& glassRect = glassRegion->m_transformedRect;
				if (
					!wil::rect_is_empty(occluder.m_transformedRect) &&
					std::abs(wil::rect_height(occluder.m_transformedRect) * wil::rect_width(occluder.m_transformedRect)) > 1.f &&

					RectF::DoesIntersectUnsafe(occluder.m_transformedRect, glassRect)
				)
				{
					std::bitset<4> sides{};
					if (occluder.m_transformedRect.left > glassRect.left)
					{
						sides.set(ShrinkSide_Left, true);
					}
					if (occluder.m_transformedRect.top > glassRect.top)
					{
						sides.set(ShrinkSide_Top, true);
					}
					if (occluder.m_transformedRect.right < glassRect.right)
					{
						sides.set(ShrinkSide_Right, true);
					}
					if (occluder.m_transformedRect.bottom < glassRect.bottom)
					{
						sides.set(ShrinkSide_Bottom, true);
					}
					targetOccluderSet.try_emplace(&occluder, sides).first->second |= sides;
				}
			}
		}

		auto& checkpoint = g_coverageSetCheckpointMap[coverageSet];
		checkpoint.context = occlusionContext;
		checkpoint.id = frameId;
		checkpoint.saves.clear();
		checkpoint.saves.reserve(targetOccluderSet.size());
		for (auto& [occluder, sides] : targetOccluderSet)
		{
			auto& originalRect = occluder->m_originalRect;

			checkpoint.saves.emplace(const_cast<dwmcore::CZOrderedRectBase*>(reinterpret_cast<dwmcore::CZOrderedRectBase const*>(occluder)), originalRect);

			if (sides.test(ShrinkSide_Left))
			{
				originalRect.left += expansion;
			}
			if (sides.test(ShrinkSide_Top))
			{
				originalRect.top += expansion;
			}
			if (sides.test(ShrinkSide_Right))
			{
				originalRect.right -= expansion;
			}
			if (sides.test(ShrinkSide_Bottom))
			{
				originalRect.bottom -= expansion;
			}

			if (wil::rect_is_empty(originalRect))
			{
				originalRect = {};
			}
			occluder->UpdateDeviceRect(matrix);
		}
	};

	if (Util::VersionBefore<os::build_w11_24h2, os::revision_24h2_with_25h2_code_staged>(dwmcore::g_versionInfo.build, dwmcore::g_versionInfo.revision))
	{
		collectAndTryShrinkOccluders(coverageSet->GetOccluderArray()->views());
	}
	else
	{
		collectAndTryShrinkOccluders(coverageSet->GetOccluderArray2()->views());
	}
}

void GlassIntegrity::FlipOccludersCheckpoint(dwmcore::CArrayBasedCoverageSet* coverageSet)
{
	if (
		const auto it = g_coverageSetCheckpointMap.find(coverageSet);
		it != g_coverageSetCheckpointMap.end()
	)
	{
		auto& checkpoint = it->second;
		if (dwmcore::GetCurrentFrameId() != checkpoint.id)
		{
			return;
		}

		const auto matrix = checkpoint.context->GetDeviceTransform();
		for (auto& [occluder, backup] : checkpoint.saves)
		{
			std::swap(occluder->GetOriginalRect(), backup);
			occluder->UpdateDeviceRect(matrix);
		}
	}
}

HRESULT GlassIntegrity::MyCOcclusionContext_CollectRectangleForOcclusion(
	dwmcore::COcclusionContext* This,
	const D2D1_RECT_F* rectangle,
	void* param3,
	void* param4,
	D2D1_RECT_F* param5
)
{
	CaptureCollectedOcclusionRectangle(This, rectangle);

	switch (g_collectRectangleForOcclusionAbi)
	{
	case CollectRectangleForOcclusionAbi::Pre_22000:
		return g_COcclusionContext_CollectRectangleForOcclusion_Org.pre22000(
			This,
			rectangle,
			param3 != nullptr,
			static_cast<D2D1_RECT_F*>(param4)
		);
	case CollectRectangleForOcclusionAbi::W11_22000:
		g_COcclusionContext_CollectRectangleForOcclusion_Org.w11_22000(
			This,
			rectangle,
			param3 != nullptr,
			param4 != nullptr,
			param5
		);
		return S_OK;
	case CollectRectangleForOcclusionAbi::W11_24H2_Pre7840:
		g_COcclusionContext_CollectRectangleForOcclusion_Org.w11_24h2_pre7840(
			This,
			*rectangle,
			static_cast<D2D1_RECT_F*>(param3)
		);
		return S_OK;
	default:
		g_COcclusionContext_CollectRectangleForOcclusion_Org.w11_24h2_7840(
			This,
			*rectangle,
			param3 != nullptr,
			static_cast<D2D1_RECT_F*>(param4)
		);
		return S_OK;
	}
}

HRESULT GlassIntegrity::MyCOcclusionContext_CheckAndRecordOverlayCandidate(
	dwmcore::COcclusionContext* This,
	dwmcore::CVisual* visual,
	void* surface,
	const dwmcore::CMILMatrix* matrix,
	const dwmcore::CShape* shape,
	int flags
)
{
	if (
		!g_mpoCandidateFiltering ||
		IsOverlayCandidateCoveredByGlass(This, shape)
	)
	{
		return S_OK;
	}

	return g_COcclusionContext_CheckAndRecordOverlayCandidate_Org(
		This,
		visual,
		surface,
		matrix,
		shape,
		flags
	);
}

HRESULT GlassIntegrity::MyCOcclusionContext_Compute_Pre_W10_2004(
	dwmcore::COcclusionContext* This,
	const dwmcore::CVisualTree* visualTree,
	UINT count,
	const D2D1_RECT_F* rectangles,
	float unknown1,
	bool unknown2,
	const dwmcore::CMILMatrix* matrix,
	const DWM::span<dwmcore::COverlayContext*>& overlays
)
{
	HRESULT hr{ S_OK };
	if (!g_COcclusionContext_DrawGeometry_Org)
	{
		g_COcclusionContext_DrawGeometry_Org_Address = reinterpret_cast<decltype(g_COcclusionContext_DrawGeometry_Org_Address)>(&HookHelper::get_vftable_from(This)[4]);
		HookHelper::PatchPointerT(g_COcclusionContext_DrawGeometry_Org_Address, MyCOcclusionContext_DrawGeometry, &g_COcclusionContext_DrawGeometry_Org);
	}
	if (const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(This->GetArrayBasedCoverageSet()); glassCoverageSet)
	{
		glassCoverageSet->Clear();
	}
	const auto expansion = GlassKernel::GetBlurRadius();
	if (
		expansion &&
		count
	)
	{
		const auto extendedRectangles = std::make_unique_for_overwrite<D2D1_RECT_F[]>(count);
		memcpy_s(
			extendedRectangles.get(),
			count * sizeof(D2D1_RECT_F),
			rectangles,
			count * sizeof(D2D1_RECT_F)
		);
		for (auto& rectangle : std::span{ extendedRectangles.get(), count })
		{
			rectangle.left -= expansion * 2.f;
			rectangle.top -= expansion * 2.f;
			rectangle.right += expansion * 2.f;
			rectangle.bottom += expansion * 2.f;
		}

		hr = g_COcclusionContext_Compute_Pre_W10_2004_Org(
			This,
			visualTree,
			count,
			extendedRectangles.get(),
			unknown1,
			unknown2,
			matrix,
			overlays
		);
	}
	else
	{
		hr = g_COcclusionContext_Compute_Pre_W10_2004_Org(
			This,
			visualTree,
			count,
			rectangles,
			unknown1,
			unknown2,
			matrix,
			overlays
		);
	}

	return hr;
}
HRESULT GlassIntegrity::MyCOcclusionContext_Compute(
	dwmcore::COcclusionContext* This,
	const dwmcore::CVisualTree* visualTree,
	const DWM::span<D2D1_RECT_F>& rectangles,
	float unknown,
	const DWM::span<dwmcore::COverlayContext*>& overlays
)
{
	HRESULT hr{ S_OK };
	if (!g_COcclusionContext_DrawGeometry_Org)
	{
		g_COcclusionContext_DrawGeometry_Org_Address = reinterpret_cast<decltype(g_COcclusionContext_DrawGeometry_Org_Address)>(&HookHelper::get_vftable_from(This)[4]);
		HookHelper::PatchPointerT(g_COcclusionContext_DrawGeometry_Org_Address, MyCOcclusionContext_DrawGeometry, &g_COcclusionContext_DrawGeometry_Org);
	}
	if (const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(This->GetArrayBasedCoverageSet()); glassCoverageSet)
	{
		glassCoverageSet->Clear();
	}
	const auto expansion = GlassKernel::GetBlurRadius();
	if (
		expansion &&
		rectangles.length
	)
	{
		const auto extendedRectangles = std::make_unique_for_overwrite<D2D1_RECT_F[]>(rectangles.length);
		memcpy_s(
			extendedRectangles.get(),
			rectangles.length * sizeof(D2D1_RECT_F),
			rectangles.data,
			rectangles.length * sizeof(D2D1_RECT_F)
		);
		for (auto& rectangle : std::span{ extendedRectangles.get(), rectangles.length })
		{
			rectangle.left -= expansion * 2.f;
			rectangle.top -= expansion * 2.f;
			rectangle.right += expansion * 2.f;
			rectangle.bottom += expansion * 2.f;
		}

		hr = g_COcclusionContext_Compute_Org(
			This,
			visualTree,
			DWM::span{ rectangles.length, extendedRectangles.get() },
			unknown,
			overlays
		);
	}
	else
	{
		hr = g_COcclusionContext_Compute_Org(
			This,
			visualTree,
			rectangles,
			unknown,
			overlays
		);
	}

	return hr;
}
HRESULT GlassIntegrity::MyCOcclusionContext_DrawGeometry(
	dwmcore::IDrawingContext* This,
	dwmcore::CLegacyMilBrush* brush,
	dwmcore::CGeometry* geometry
)
{
	const auto hr = g_COcclusionContext_DrawGeometry_Org(
		This,
		brush,
		geometry
	);

	if (
		FAILED(hr) ||
		HookHelper::get_vftable_from(brush) != dwmcore::CSolidColorLegacyMilBrush::vftable
	)
	{
		return hr;
	}

	dwmcore::CShapePtr geometryShape{};
	if (
		FAILED(geometry->GetShapeData(nullptr, &geometryShape)) ||
		!geometryShape ||
		geometryShape->IsEmpty()
	)
	{
		return hr;
	}

	const auto solidColorBrush = static_cast<dwmcore::CSolidColorLegacyMilBrush*>(brush);
	auto color = solidColorBrush->GetRealizedColor();
	const auto expansion = GlassKernel::GetBlurRadius();
	const auto reinterpreter = GlassKernel::AlphaChannelReinterpreter(color.a);
	const auto valid = reinterpreter.GetIsValid();
	const auto active = reinterpreter.GetIsActive();
	const auto maximized = reinterpreter.GetIsMaximized();

	if (
		GlassKernel::CRealizedGlassColorizationParameters realizedGlassColorizationParameters;
		!valid ||
		(
			valid &&
			(
				color.a = 1.f,
				realizedGlassColorizationParameters = GlassKernel::RealizeWindowColorization(
					GlassKernel::GetBaseColor(Shared::IsTransparencyDisabled(), maximized),
					GlassKernel::GetSourceColor(active),
					GlassKernel::GetColorizationOpacity(active, maximized),
					Shared::IsTransparencyDisabled(),
					false
				),
				Shared::IsTransparencyDisabled() ||
				Shared::IsGlassFullyOpaque(
					realizedGlassColorizationParameters.color.a,
					realizedGlassColorizationParameters.blurBalance,
					realizedGlassColorizationParameters.afterglowBalance
				)
			)
		)
	)
	{
		UINT count{};
		if (!geometryShape->IsRectangles(&count))
		{
			return hr;
		}
		const auto rectangles = std::make_unique_for_overwrite<D2D1_RECT_F[]>(count);
		if (!geometryShape->GetRectangles(rectangles.get(), count))
		{
			return hr;
		}

		for (const auto& rect : std::span{ rectangles.get(), count })
		{
			RETURN_IF_FAILED(
				This->DrawSolidRectangle(
					rect,
					color
				)
			);
		}
	}
	// here are the glass regions
	else if (
		valid &&
		expansion
	)
	{
		const auto occlusionContext = This->GetOcclusionContext();
		if (const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(occlusionContext->GetArrayBasedCoverageSet(), true); glassCoverageSet)
		{
			D2D1_RECT_F bounds{};
			RETURN_IF_FAILED(geometryShape->GetTightBounds(&bounds, occlusionContext->GetWorldTransform()));
			if (
				!wil::rect_is_empty(bounds) &&
				std::abs(wil::rect_height(bounds) * wil::rect_width(bounds)) > 1.f
			)
			{
				glassCoverageSet->Add(
					bounds,
					occlusionContext->GetCurrentZ(),
					occlusionContext->GetDeviceTransform()
				);
			}
		}
	}

	return hr;
}

HRESULT GlassIntegrity::MyCOcclusionContext_SetDeviceTransform(
	dwmcore::COcclusionContext* This,
	const dwmcore::CMILMatrix* matrix
)
{
	if (
		const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(This->GetArrayBasedCoverageSet());
		glassCoverageSet
	)
	{
		glassCoverageSet->SetDeviceTransform(matrix);
	}

	return g_COcclusionContext_SetDeviceTransform_Org(This, matrix);
}

void GlassIntegrity::MyCOcclusionContext_Destructor(dwmcore::COcclusionContext* This)
{
	if (g_lastCollectedOcclusionRectangle.context == This)
	{
		g_lastCollectedOcclusionRectangle = {};
	}
	CArrayBasedGlassCoverageSet::Remove(This->GetArrayBasedCoverageSet());
	g_coverageSetCheckpointMap.erase(This->GetArrayBasedCoverageSet());
	g_shrunkCoverageSetMap.erase(This);
	return g_COcclusionContext_Destructor_Org(This);
}


bool GlassIntegrity::MyCArrayBasedCoverageSet_IsCovered(
	dwmcore::CArrayBasedCoverageSet* This,
	const D2D1_RECT_F& coverage,
	int depth
)
{
	FlipOccludersCheckpointScoped(This);

	const auto expansion = GlassKernel::GetBlurRadius();
	if (!expansion || !g_calculationContext.IsActive())
	{
		return g_CArrayBasedCoverageSet_IsCovered_Org(This, coverage, depth);
	}

	const auto dirtyRectScope = wil::scope_exit([] static { g_calculationContext.dirtyRect = nullptr; });
	const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(This);
	if (Util::VersionBefore<os::build_w11_24h2, os::revision_24h2_with_25h2_code_staged>(dwmcore::g_versionInfo.build, dwmcore::g_versionInfo.revision))
	{
		auto covered = g_CArrayBasedCoverageSet_IsCovered_Org(This, coverage, depth);
		if (
			glassCoverageSet &&
			!glassCoverageSet->IsEmpty() &&
			!covered &&
			glassCoverageSet->IsPartiallyCovered(coverage, depth)
		)
		{
			const D2D1_RECT_F extendedCoverage
			{
				coverage.left - expansion,
				coverage.top - expansion,
				coverage.right + expansion,
				coverage.bottom + expansion
			};

			covered = g_CArrayBasedCoverageSet_IsCovered_Org(This, extendedCoverage, depth);

			if (!covered)
			{
				// unpaged dirty rect
				if (g_calculationContext.dirtyRect)
				{
					g_calculationContext.dirtyRect->left -= expansion;
					g_calculationContext.dirtyRect->top -= expansion;
					g_calculationContext.dirtyRect->right += expansion;
					g_calculationContext.dirtyRect->bottom += expansion;
				}
				// paged dirty rect
				const_cast<D2D1_RECT_F&>(coverage) = extendedCoverage;
			}
		}

		return covered;
	}
	else
	{
		const D2D1_RECT_F shrunkCoverage =
		{
			coverage.left + expansion,
			coverage.top + expansion,
			coverage.right - expansion,
			coverage.bottom - expansion
		};
		auto covered = g_CArrayBasedCoverageSet_IsCovered_Org(This, shrunkCoverage, depth);
		bool shrink = true;
		if (
			glassCoverageSet &&
			!glassCoverageSet->IsEmpty() &&
			glassCoverageSet->IsPartiallyCovered(shrunkCoverage, depth) &&
			!covered
		)
		{
			covered = g_CArrayBasedCoverageSet_IsCovered_Org(This, coverage, depth);

			if (!covered)
			{
				shrink = false;
			}
		}

		if (shrink)
		{
			// unpaged dirty rect
			if (g_calculationContext.dirtyRect)
			{
				g_calculationContext.dirtyRect->left += expansion;
				g_calculationContext.dirtyRect->top += expansion;
				g_calculationContext.dirtyRect->right -= expansion;
				g_calculationContext.dirtyRect->bottom -= expansion;
			}
			// paged dirty rect
			const_cast<D2D1_RECT_F&>(coverage) = shrunkCoverage;
		}

		return covered;
	}
}
bool GlassIntegrity::MyCOcclusionContext_IsOccluded(
	dwmcore::COcclusionContext* This,
	const D2D1_RECT_F& coverage,
	int depth,
	bool ignoreDeviceTransform
)
{
	FlipOccludersCheckpointScoped(This->GetArrayBasedCoverageSet());

	auto occluded = g_COcclusionContext_IsOccluded_Org(
		This,
		coverage,
		depth,
		ignoreDeviceTransform
	);

	const auto expansion = GlassKernel::GetBlurRadius();
	if (!expansion || !g_calculationContext.IsActive())
	{
		return occluded;
	}

	const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(This->GetArrayBasedCoverageSet());

	if (
		glassCoverageSet &&
		!glassCoverageSet->IsEmpty() &&
		glassCoverageSet->IsPartiallyCovered(This->PageInPixelsRectToDeviceRect(coverage), depth)
	)
	{
		const D2D1_RECT_F extendedCoverage
		{
			coverage.left - expansion,
			coverage.top - expansion,
			coverage.right + expansion,
			coverage.bottom + expansion
		};

		occluded = g_COcclusionContext_IsOccluded_Org(
			This,
			extendedCoverage,
			depth,
			ignoreDeviceTransform
		);

		if (!occluded)
		{
			// coverage is actually paged dirty rect
			const_cast<D2D1_RECT_F&>(coverage) = extendedCoverage;
		}
	}

	return occluded;
}
HRESULT GlassIntegrity::MyCOcclusionContext_PageInPixelsRectToDeviceRect(
	dwmcore::COcclusionContext* This,
	const D2D1_RECT_F& src,
	D2D1_RECT_F* dst
)
{
	const auto invokeOriginal = [This](const D2D1_RECT_F& candidate, D2D1_RECT_F* result)
	{
		return g_COcclusionContext_PageInPixelsRectToDeviceRect_Org(This, candidate, result);
	};

	if (g_calculationContext.IsActive())
	{
		g_calculationContext.dirtyRect = const_cast<D2D1_RECT_F*>(&src);

		if (const auto expansion = GlassKernel::GetBlurRadius(); expansion)
		{
			if (
				Util::VersionBefore<os::build_w11_24h2, os::revision_24h2_page_in_pixels_helper_reemitted>(
					dwmcore::g_versionInfo.build,
					dwmcore::g_versionInfo.revision
				)
			)
			{
				g_calculationContext.dirtyRect->left -= expansion;
				g_calculationContext.dirtyRect->top -= expansion;
				g_calculationContext.dirtyRect->right += expansion;
				g_calculationContext.dirtyRect->bottom += expansion;
			}
			else
			{
				const D2D1_RECT_F extendedDirtyRect
				{
					src.left - expansion,
					src.top - expansion,
					src.right + expansion,
					src.bottom + expansion
				};
				D2D1_RECT_F extendedDeviceRect{};
				const auto extendedResult = invokeOriginal(extendedDirtyRect, &extendedDeviceRect);
				bool useExtendedDirtyRect{ static_cast<bool>(extendedResult) };
				if (useExtendedDirtyRect && g_glassSafetyZoneMode != GlassSafetyZoneMode::Always)
				{
					const auto coverageSet = This->GetArrayBasedCoverageSet();
					const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(coverageSet);
					useExtendedDirtyRect = glassCoverageSet && glassCoverageSet->IsVisible(extendedDeviceRect, coverageSet);
				}
				if (useExtendedDirtyRect)
				{
					*g_calculationContext.dirtyRect = extendedDirtyRect;
					*dst = extendedDeviceRect;
					return extendedResult;
				}
			}
		}
	}

	return invokeOriginal(src, dst);
}

HRESULT GlassIntegrity::MyCHwndRenderTarget_RenderDirtyRegion(
	dwmcore::CHwndRenderTarget* This,
	dwmcore::CDrawingContext* drawingContext,
	dwmcore::CComposeTop* composeTop
)
{
	const auto calculationScope = EnterUnoccludedDirtyRegionCalculationContext(
		&g_calculationContext,
		drawingContext->GetOcclusionContext()
	);
	return g_CHwndRenderTarget_RenderDirtyRegion_Org(
		This,
		drawingContext,
		composeTop
	);
}
D2D1_RECT_F* GlassIntegrity::MyCDirtyRegion_GetUnOccludedDirtyRegion(
	dwmcore::CDirtyRegion* This,
	D2D1_RECT_F* dirtyRect,
	dwmcore::COcclusionContext* occlusionContext,
	const dwmcore::CVisualTree* tree,
	bool inflate,
	unsigned int i,
	const D2D1_RECT_F& bounds
)
{
	const auto calculationScope = EnterUnoccludedDirtyRegionCalculationContext(
		&g_calculationContext,
		occlusionContext
	);
	return g_CDirtyRegion_GetUnOccludedDirtyRegion_Org(
		This,
		dirtyRect,
		occlusionContext,
		tree,
		inflate,
		i,
		bounds
	);
}
D2D1_RECT_F* GlassIntegrity::MyCDirtyRegion_GetUnOccludedDirtyRect(
	dwmcore::CDirtyRegion* This,
	D2D1_RECT_F* dirtyRect,
	int i,
	const D2D1_RECT_F& bounds,
	bool useSuperSample,
	const DWM::span<dwmcore::CVisual>& visuals,
	dwmcore::COcclusionContext* occlusionContext
)
{
	const auto context = occlusionContext ? occlusionContext : This->GetOcclusionContext();
	const auto calculationScope = EnterUnoccludedDirtyRegionCalculationContext(
		&g_calculationContext,
		context->GetFrameId() == dwmcore::GetCurrentFrameId() ? context : nullptr
	);
	return g_CDirtyRegion_GetUnOccludedDirtyRect_Org(
		This,
		dirtyRect,
		i,
		bounds,
		useSuperSample,
		visuals,
		occlusionContext
	);
}
D2D1_RECT_F* GlassIntegrity::MyCDirtyRegion_GetOptimizedRect_WS2022(
	dwmcore::CDirtyRegion* This,
	D2D1_RECT_F* dirtyRect,
	int i,
	const D2D1_RECT_F& bounds,
	const D2D1_SIZE_U& size,
	bool transform,
	const dwmcore::CMILMatrix* matrix,
	const DWM::span<dwmcore::CVisual>& visuals,
	const dwmcore::CRegion* region,
	dwmcore::COcclusionContext* occlusionContext
)
{
	const auto context = occlusionContext ? occlusionContext : This->GetOcclusionContext();
	const auto calculationScope = EnterUnoccludedDirtyRegionCalculationContext(
		&g_calculationContext,
		context->GetFrameId() == dwmcore::GetCurrentFrameId() ? context : nullptr
	);
	return g_CDirtyRegion_GetOptimizedRect_WS2022_Org(
		This,
		dirtyRect,
		i,
		bounds,
		size,
		transform,
		matrix,
		visuals,
		region,
		occlusionContext
	);
}
D2D1_RECT_F* GlassIntegrity::MyCDirtyRegion_GetOptimizedRect(
	dwmcore::CDirtyRegion* This,
	D2D1_RECT_F* dirtyRect,
	int i,
	const D2D1_RECT_F& bounds,
	const dwmcore::CRegion* region,
	const dwmcore::CMILMatrix* matrix,
	bool useSuperSample,
	const DWM::span<dwmcore::CVisual>& visuals,
	dwmcore::COcclusionContext* occlusionContext
)
{
	const auto context = occlusionContext ? occlusionContext : This->GetOcclusionContext();
	const auto calculationScope = EnterUnoccludedDirtyRegionCalculationContext(
		&g_calculationContext,
		context->GetFrameId() == dwmcore::GetCurrentFrameId() ? context : nullptr
	);
	return g_CDirtyRegion_GetOptimizedRect_Org(
		This,
		dirtyRect,
		i,
		bounds,
		region,
		matrix,
		useSuperSample,
		visuals,
		occlusionContext
	);
}
D2D1_RECT_F* GlassIntegrity::MyCTreeDirty_GetOptimizedRect(
	dwmcore::CTreeDirty* This,
	D2D1_RECT_F* dirtyRect,
	UINT i,
	const D2D1_RECT_F& bounds,
	dwmcore::COcclusionContext* occlusionContext,
	const dwmcore::CRegion* region,
	const dwmcore::CMILMatrix* matrix,
	bool useSuperSample,
	const DWM::span<dwmcore::CVisual>& visuals
)
{
	const auto calculationScope = EnterUnoccludedDirtyRegionCalculationContext(
		&g_calculationContext,
		occlusionContext->GetFrameId() == dwmcore::GetCurrentFrameId() ? occlusionContext : nullptr
	);
	return g_CTreeDirty_GetOptimizedRect_Org(
		This,
		dirtyRect,
		i,
		bounds,
		occlusionContext,
		region,
		matrix,
		useSuperSample,
		visuals
	);
}

template <typename T>
HRESULT GlassIntegrity::MyCDrawingContext_DrawVisualTree(
	dwmcore::CDrawingContext* This,
	const D2D1_RECT_F& rectangle,
	dwmcore::COcclusionContext* occlusionContext,
	T&& callback
)
{
	HRESULT hr{ S_OK };

	const auto safetyZonePoolCleanup = wil::scope_exit([]
	{
		g_safetyZonePool.Cleanup(std::chrono::seconds{ 30 });
	});

	do
	{
		if (g_glassSafetyZoneMode == GlassSafetyZoneMode::Disabled)
		{
			break;
		}

		if (
			!occlusionContext ||
			(
				dwmcore::g_versionInfo.build >= os::build_w10_2004 &&
				occlusionContext->GetFrameId() != dwmcore::GetCurrentFrameId()
			)
		)
		{
			break;
		}

		const auto expansion = GlassKernel::GetBlurRadius();
		if (!expansion)
		{
			break;
		}

		occlusionContext->SetDeviceTransform(This->GetDeviceTransform());
		const auto coverageSet = occlusionContext->GetArrayBasedCoverageSet();
		const auto transformedRect = occlusionContext->PageInPixelsRectToDeviceRect(rectangle);
		const auto glassCoverageSet = CArrayBasedGlassCoverageSet::GetOrCreate(coverageSet);

		if (
			g_glassSafetyZoneMode != GlassSafetyZoneMode::Always &&
			(
				!glassCoverageSet ||
				glassCoverageSet->IsEmpty() ||
				!glassCoverageSet->IsVisible(transformedRect, coverageSet)
			)
		)
		{
			break;
		}

		ShrinkOccludersAboveGlass(occlusionContext);
		if (GlassKernel::IsCurrentCVIFullyTransparent())
		{
			break;
		}

		const auto d2dContext = This->GetD3DDevice()->GetD2DContext();
		const auto context = d2dContext->GetDeviceContext();

		if (dwmcore::g_versionInfo.build < os::build_w10_2004)
		{
			if (!This->GetRenderTarget())
			{
				return S_OK;
			}
		}
		else
		{
			if (!This->GetDeviceTarget())
			{
				return S_OK;
			}
		}
		if (!context)
		{
			break;
		}

		LOG_IF_FAILED(This->ApplyRenderStateInternal(false)); // apply clip and other states
		LOG_IF_FAILED(This->FlushD2D()); // flush previous draw calls
		d2dContext->EnsureBeginDraw(); // refresh d2d selected target

		auto safetyZoneLayer = g_safetyZonePool.Acquire(d2dContext);
		const auto safetyZoneLayerScope = wil::scope_exit([&]
		{
			g_safetyZonePool.Release(d2dContext, std::move(safetyZoneLayer));
		});

		D2D1_RECT_F extendedPixelRectangle{};
		if (
			hr = safetyZoneLayer->Push(
				context,
				This->GetDeviceTransform()->GetD2DMatrix(),
				rectangle,
				expansion,
				extendedPixelRectangle
			);
			FAILED(hr)
		)
		{
			LOG_HR(hr);
			break;
		}

		hr = callback(extendedPixelRectangle);

		LOG_IF_FAILED(This->ApplyRenderStateInternal(false)); // apply clip and other states
		LOG_IF_FAILED(This->FlushD2D()); // flush previous draw calls

		safetyZoneLayer->Pop();

#ifdef _DEBUG
		if (GetAsyncKeyState(VK_SHIFT))
		{
			winrt::com_ptr<ID2D1SolidColorBrush> brush{};
			context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), brush.put());
			context->DrawRectangle(rectangle, brush.get());
		}
#endif

		return hr;
	}
	while (false);

	hr = callback(rectangle);

	return hr;
}


HRESULT GlassIntegrity::MyCDrawingContext_DrawVisualTree_Pre_Win10_2004(
	dwmcore::CDrawingContext* This,
	dwmcore::CVisualTree* tree,
	const D2D1_RECT_F& rectangle,
	dwmcore::COverlayContext* overlayContext,
	int unknown1,
	bool unknown2,
	bool unknown3,
	bool useOcclusionContext,
	const D2D1_RECT_F* unknown4,
	bool unknown5,
	bool unknown6,
	bool unknown7
)
{
	return MyCDrawingContext_DrawVisualTree(
		This,
		rectangle,
		useOcclusionContext ? This->GetOcclusionContext() : nullptr,
		[=](const D2D1_RECT_F& replacedRectangle)
		{
			return g_CDrawingContext_DrawVisualTree_Pre_Win10_2004_Org(
				This,
				tree,
				replacedRectangle,
				overlayContext,
				unknown1,
				unknown2,
				unknown3,
				useOcclusionContext,
				unknown4,
				unknown5,
				unknown6,
				unknown7
			);
		}
	);
}
HRESULT GlassIntegrity::MyCDrawingContext_DrawVisualTree_Win10(
	dwmcore::CDrawingContext* This,
	dwmcore::CVisualTree* tree,
	const D2D1_RECT_F& rectangle,
	dwmcore::COcclusionContext* occlusionContext,
	int clearMode,
	bool useSuperSample
)
{
	return MyCDrawingContext_DrawVisualTree(
		This,
		rectangle,
		occlusionContext,
		[=](const D2D1_RECT_F& replacedRectangle)
		{
			return g_CDrawingContext_DrawVisualTree_Win10_Org(
				This,
				tree,
				replacedRectangle,
				occlusionContext,
				clearMode,
				useSuperSample
			);
		}
	);
}
HRESULT GlassIntegrity::MyCDrawingContext_DrawVisualTree_Win11(
	dwmcore::CDrawingContext* This,
	dwmcore::CVisualTree* tree,
	const D2D1_RECT_F& rectangle,
	dwmcore::COcclusionContext* occlusionContext,
	int clearMode,
	bool useSuperSample,
	dwmcore::CVisual* visualOverride
)
{
	return MyCDrawingContext_DrawVisualTree(
		This,
		rectangle,
		occlusionContext,
		[=](const D2D1_RECT_F& replacedRectangle)
		{
			return g_CDrawingContext_DrawVisualTree_Win11_Org(
				This,
				tree,
				replacedRectangle,
				occlusionContext,
				clearMode,
				useSuperSample,
				visualOverride
			);
		}
	);
}

void GlassIntegrity::DestroyDeviceResources(dwmcore::CD2DContext* d2dContext)
{
	g_safetyZonePool.Cleanup(std::chrono::seconds{ 0 }, d2dContext);
}

void GlassIntegrity::Update([[maybe_unused]] GlassEngine::UpdateType type)
{
	if (type & GlassEngine::UpdateType::Backdrop)
	{
		g_glassSafetyZoneMode = static_cast<GlassSafetyZoneMode>(std::clamp(GlassEngine::GetDwordFromRegistry(L"GlassSafetyZoneMode", 1), 0ul, 2ul));
		g_mpoCandidateFiltering = GlassEngine::GetDwordFromRegistry(L"EnableMpoCandidateFiltering", 1) != 0;
	}
}

void GlassIntegrity::Startup()
{
	if (Shared::g_disabledHooks.test(Shared::DisabledHooks_GlassIntegrity))
	{
		return;
	}

	// CArrayBasedCoverageSet::IsCovered 解析不到时整个模块不挂钩
	{
		dwmcore::g_projectionArray.ApplyToVariable("CArrayBasedCoverageSet::IsCovered", g_CArrayBasedCoverageSet_IsCovered_Org);
		if (!g_CArrayBasedCoverageSet_IsCovered_Org)
		{
			Shared::g_coverageSetIsCoveredHooked = false;
			return;
		}
		// 上游 3.x 已删除翻转/还原补丁；该版本起默认关闭，可用注册表强制打开
		const auto coverageSetHookDefault = dwmcore::g_versionInfo.build >= os::build_flip_occluders_removed ? 1ul : 0ul;
		Shared::g_coverageSetIsCoveredHooked = GlassEngine::GetDwordFromRegistry(L"DisableCoverageSetHook", coverageSetHookDefault) == 0;
	}

	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::CheckAndRecordOverlayCandidate", g_COcclusionContext_CheckAndRecordOverlayCandidate_Org);
	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::CollectRectangleForOcclusion", g_COcclusionContext_CollectRectangleForOcclusion_Org.generic);

	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::Compute", g_COcclusionContext_Compute_Org);
	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::SetDeviceTransform", g_COcclusionContext_SetDeviceTransform_Org);
	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::~COcclusionContext", g_COcclusionContext_Destructor_Org);

	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::IsOccluded", g_COcclusionContext_IsOccluded_Org);
	dwmcore::g_projectionArray.ApplyToVariable("COcclusionContext::PageInPixelsRectToDeviceRect", g_COcclusionContext_PageInPixelsRectToDeviceRect_Org);
	dwmcore::g_projectionArray.ApplyToVariable("CHwndRenderTarget::RenderDirtyRegion", g_CHwndRenderTarget_RenderDirtyRegion_Org);
	dwmcore::g_projectionArray.ApplyToVariable("CDirtyRegion::GetUnOccludedDirtyRegion", g_CDirtyRegion_GetUnOccludedDirtyRegion_Org);
	dwmcore::g_projectionArray.ApplyToVariable("CDirtyRegion::GetUnOccludedDirtyRect", g_CDirtyRegion_GetUnOccludedDirtyRect_Org);
	dwmcore::g_projectionArray.ApplyToVariable("CDirtyRegion::GetOptimizedRect", g_CDirtyRegion_GetOptimizedRect_Org);
	dwmcore::g_projectionArray.ApplyToVariable("CDirtyRegion::GetOptimizedRect", g_CDirtyRegion_GetOptimizedRect_WS2022_Org);
	dwmcore::g_projectionArray.ApplyToVariable("CTreeDirty::GetOptimizedRect", g_CTreeDirty_GetOptimizedRect_Org);

	dwmcore::g_projectionArray.ApplyToVariable("CDrawingContext::DrawVisualTree", g_CDrawingContext_DrawVisualTree_Org);

	const auto build_before_w11_24h2 = dwmcore::g_versionInfo.build < os::build_w11_24h2;
	const auto build_before_server_2022 = dwmcore::g_versionInfo.build < os::build_server_2022;
	const auto build_before_w11_21h2 = dwmcore::g_versionInfo.build < os::build_w11_21h2;
	const auto build_before_w10_2004 = dwmcore::g_versionInfo.build < os::build_w10_2004;
	const auto build_before_w10_1903 = dwmcore::g_versionInfo.build < os::build_w10_1903;

	if (build_before_w10_2004 && !build_before_w10_1903)
	{
		g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation = const_cast<uint8_t*>(HookHelper::FindPattern(std::span{ reinterpret_cast<const uint8_t*>(g_CHwndRenderTarget_RenderDirtyRegion_Org) + 1500, 3000 }, g_COcclusionContext_IsDeviceTransformAssigned_Instructions));
		if (g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation)
		{
			g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation = &g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation[8];
			HookHelper::PatchInstructions(
				g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation,
				std::array<uint8_t, 2>
				{
					0x74, 0x23
				}
			);
		}
	}

	if (build_before_w11_21h2)
	{
		g_collectRectangleForOcclusionAbi = CollectRectangleForOcclusionAbi::Pre_22000;
	}
	else if (build_before_w11_24h2)
	{
		g_collectRectangleForOcclusionAbi = CollectRectangleForOcclusionAbi::W11_22000;
	}
	else if (
		dwmcore::g_versionInfo.build == os::build_w11_24h2 &&
		dwmcore::g_versionInfo.revision < os::revision_24h2_occlusion_abi_break
	)
	{
		g_collectRectangleForOcclusionAbi = CollectRectangleForOcclusionAbi::W11_24H2_Pre7840;
	}
	else
	{
		g_collectRectangleForOcclusionAbi = CollectRectangleForOcclusionAbi::W11_24H2_7840;
	}

	HookHelper::PatchFunctions(
		std::initializer_list<HookHelper::DetourInfo>
		{
			{ &g_COcclusionContext_CheckAndRecordOverlayCandidate_Org, &MyCOcclusionContext_CheckAndRecordOverlayCandidate, g_COcclusionContext_CheckAndRecordOverlayCandidate_Org != nullptr },
			{ &g_COcclusionContext_CollectRectangleForOcclusion_Org.generic, &MyCOcclusionContext_CollectRectangleForOcclusion, g_COcclusionContext_CollectRectangleForOcclusion_Org.generic != nullptr },

			{ &g_COcclusionContext_Compute_Pre_W10_2004_Org, &MyCOcclusionContext_Compute_Pre_W10_2004, build_before_w10_2004 },
			{ &g_COcclusionContext_Compute_Org, &MyCOcclusionContext_Compute, !build_before_w10_2004 },
			{ &g_COcclusionContext_SetDeviceTransform_Org, &MyCOcclusionContext_SetDeviceTransform },
			{ &g_COcclusionContext_Destructor_Org, &MyCOcclusionContext_Destructor },

			{ &g_CHwndRenderTarget_RenderDirtyRegion_Org, &MyCHwndRenderTarget_RenderDirtyRegion, build_before_w10_2004 && !build_before_w10_1903 },
			{ &g_CDirtyRegion_GetUnOccludedDirtyRegion_Org, &MyCDirtyRegion_GetUnOccludedDirtyRegion, build_before_w10_2004 },
			{ &g_CDirtyRegion_GetUnOccludedDirtyRect_Org, &MyCDirtyRegion_GetUnOccludedDirtyRect, build_before_server_2022 && !build_before_w10_2004 },
			{ &g_CDirtyRegion_GetOptimizedRect_WS2022_Org, &MyCDirtyRegion_GetOptimizedRect_WS2022, build_before_w11_21h2 && !build_before_server_2022 },
			{ &g_CDirtyRegion_GetOptimizedRect_Org, &MyCDirtyRegion_GetOptimizedRect, build_before_w11_24h2 && !build_before_w11_21h2 },
			{ &g_CTreeDirty_GetOptimizedRect_Org, &MyCTreeDirty_GetOptimizedRect, !build_before_w11_24h2 },

			{ &g_COcclusionContext_IsOccluded_Org, &MyCOcclusionContext_IsOccluded, g_CArrayBasedCoverageSet_IsCovered_Org == nullptr },
			{ &g_CArrayBasedCoverageSet_IsCovered_Org, &MyCArrayBasedCoverageSet_IsCovered, Shared::g_coverageSetIsCoveredHooked },
			{ &g_COcclusionContext_PageInPixelsRectToDeviceRect_Org, &MyCOcclusionContext_PageInPixelsRectToDeviceRect, g_CArrayBasedCoverageSet_IsCovered_Org != nullptr },

			{ &g_CDrawingContext_DrawVisualTree_Pre_Win10_2004_Org, &MyCDrawingContext_DrawVisualTree_Pre_Win10_2004, build_before_w10_2004 },
			{ &g_CDrawingContext_DrawVisualTree_Win10_Org, &MyCDrawingContext_DrawVisualTree_Win10, !build_before_w10_2004 && build_before_server_2022 },
			{ &g_CDrawingContext_DrawVisualTree_Win11_Org, &MyCDrawingContext_DrawVisualTree_Win11, !build_before_server_2022 }
		},
		true
	);
}

void GlassIntegrity::Shutdown()
{
	if (g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation)
	{
		HookHelper::PatchInstructions(
			g_COcclusionContext_IsDeviceTransformAssigned_PatchLocation,
			std::array<uint8_t, 2>
			{
				0x90, 0x90
			}
		);
	}

	const auto build_before_w11_24h2 = dwmcore::g_versionInfo.build < os::build_w11_24h2;
	const auto build_before_server_2022 = dwmcore::g_versionInfo.build < os::build_server_2022;
	const auto build_before_w11_21h2 = dwmcore::g_versionInfo.build < os::build_w11_21h2;
	const auto build_before_w10_2004 = dwmcore::g_versionInfo.build < os::build_w10_2004;
	const auto build_before_w10_1903 = dwmcore::g_versionInfo.build < os::build_w10_1903;
	HookHelper::PatchFunctions(
		std::initializer_list<HookHelper::DetourInfo>
		{
			{ &g_COcclusionContext_CheckAndRecordOverlayCandidate_Org, &MyCOcclusionContext_CheckAndRecordOverlayCandidate, g_COcclusionContext_CheckAndRecordOverlayCandidate_Org != nullptr },
			{ &g_COcclusionContext_CollectRectangleForOcclusion_Org.generic, &MyCOcclusionContext_CollectRectangleForOcclusion, g_COcclusionContext_CollectRectangleForOcclusion_Org.generic != nullptr },

			{ &g_COcclusionContext_Compute_Pre_W10_2004_Org, &MyCOcclusionContext_Compute_Pre_W10_2004, build_before_w10_2004 },
			{ &g_COcclusionContext_Compute_Org, &MyCOcclusionContext_Compute, !build_before_w10_2004 },
			{ &g_COcclusionContext_SetDeviceTransform_Org, &MyCOcclusionContext_SetDeviceTransform },
			{ &g_COcclusionContext_Destructor_Org, &MyCOcclusionContext_Destructor },

			{ &g_CHwndRenderTarget_RenderDirtyRegion_Org, &MyCHwndRenderTarget_RenderDirtyRegion, build_before_w10_2004 && !build_before_w10_1903 },
			{ &g_CDirtyRegion_GetUnOccludedDirtyRegion_Org, &MyCDirtyRegion_GetUnOccludedDirtyRegion, build_before_w10_2004 },
			{ &g_CDirtyRegion_GetUnOccludedDirtyRect_Org, &MyCDirtyRegion_GetUnOccludedDirtyRect, build_before_server_2022 && !build_before_w10_2004 },
			{ &g_CDirtyRegion_GetOptimizedRect_WS2022_Org, &MyCDirtyRegion_GetOptimizedRect_WS2022, build_before_w11_21h2 && !build_before_server_2022 },
			{ &g_CDirtyRegion_GetOptimizedRect_Org, &MyCDirtyRegion_GetOptimizedRect, build_before_w11_24h2 && !build_before_w11_21h2 },
			{ &g_CTreeDirty_GetOptimizedRect_Org, &MyCTreeDirty_GetOptimizedRect, !build_before_w11_24h2 },

			{ &g_COcclusionContext_IsOccluded_Org, &MyCOcclusionContext_IsOccluded, g_CArrayBasedCoverageSet_IsCovered_Org == nullptr },
			{ &g_CArrayBasedCoverageSet_IsCovered_Org, &MyCArrayBasedCoverageSet_IsCovered, Shared::g_coverageSetIsCoveredHooked },
			{ &g_COcclusionContext_PageInPixelsRectToDeviceRect_Org, &MyCOcclusionContext_PageInPixelsRectToDeviceRect, g_CArrayBasedCoverageSet_IsCovered_Org != nullptr },

			{ &g_CDrawingContext_DrawVisualTree_Pre_Win10_2004_Org, &MyCDrawingContext_DrawVisualTree_Pre_Win10_2004, build_before_w10_2004 },
			{ &g_CDrawingContext_DrawVisualTree_Win10_Org, &MyCDrawingContext_DrawVisualTree_Win10, !build_before_w10_2004 && build_before_server_2022 },
			{ &g_CDrawingContext_DrawVisualTree_Win11_Org, &MyCDrawingContext_DrawVisualTree_Win11, !build_before_server_2022 }
		},
		false
	);

	SwitchToThread();

	if (g_COcclusionContext_DrawGeometry_Org)
	{
		HookHelper::PatchPointerT(
			g_COcclusionContext_DrawGeometry_Org_Address,
			g_COcclusionContext_DrawGeometry_Org
		);
	}

	CArrayBasedGlassCoverageSet::RemoveAll();
	g_lastCollectedOcclusionRectangle = {};
	g_shrunkCoverageSetMap.clear();
	g_safetyZonePool.Cleanup(std::chrono::seconds{ 0 });
	g_coverageSetCheckpointMap.clear();
}
