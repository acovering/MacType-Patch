#include <dwrite_3.h>
#include "common.h"
#include "userparams.h"


#include <windows.h>


static void hookIDWriteBitmapRenderTargetIfStill(IDWriteBitmapRenderTarget* pDWriteBitmapRenderTarget);
static void hookIDWriteGdiInteropIfStill(IDWriteGdiInterop* pDWriteGdiInterop);
static void hookIDWriteFactoryIfStill(IDWriteFactory* pDWriteFactory);
static void hookIDWriteGlyphRunAnalysisIfStill(IDWriteGlyphRunAnalysis* pDWriteGlyphRunAnalysis);



namespace Impl_IDWriteBitmapRenderTarget
{
	static HRESULT WINAPI DrawGlyphRun(
		IDWriteBitmapRenderTarget* This,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		DWRITE_GLYPH_RUN const* glyphRun,
		IDWriteRenderingParams* renderingParams,
		COLORREF textColor,
		RECT* blackBoxRect
	) {
		HRESULT hr = E_FAIL;
		if (FAILED(hr)) {
			hr = This->DrawGlyphRun(
				baselineOriginX,
				baselineOriginY,
				measuringMode,
				glyphRun,
				DirectWriteParams.pDWriteRenderingParams,
				textColor,
				blackBoxRect
			);
		}
		if (FAILED(hr)) {
			hr = This->DrawGlyphRun(
				baselineOriginX,
				baselineOriginY,
				measuringMode,
				glyphRun,
				renderingParams,
				textColor,
				blackBoxRect
			);
		}
		return hr;
	}
}

namespace Impl_IDWriteFactory
{
	static HRESULT WINAPI GetGdiInterop(
		IDWriteFactory* This,
		IDWriteGdiInterop** gdiInterop
	) {
		HRESULT hr = This->GetGdiInterop(gdiInterop);
		if (SUCCEEDED(hr)) {
			hookIDWriteGdiInteropIfStill(*gdiInterop);
		}
		return hr;
	}
	
	static HRESULT WINAPI CreateGlyphRunAnalysis(
		IDWriteFactory* This,
		DWRITE_GLYPH_RUN const* glyphRun,
		FLOAT pixelsPerDip,
		DWRITE_MATRIX const* transform,
		DWRITE_RENDERING_MODE renderingMode,
		DWRITE_MEASURING_MODE measuringMode,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		IDWriteGlyphRunAnalysis** glyphRunAnalysis
	) {
		HRESULT hr = E_FAIL;
		if (FAILED(hr) && renderingMode != DWRITE_RENDERING_MODE_ALIASED) {
			IDWriteFactory3* f;
			hr = This->QueryInterface(&f);
			if (SUCCEEDED(hr)) {
				DWRITE_MATRIX m;
				DWRITE_MATRIX const* pm = transform;
				if (pm) {
					m = *transform;
					m.m11 *= pixelsPerDip;
					m.m22 *= pixelsPerDip;
					pm = &m;
				}
				hr = f->CreateGlyphRunAnalysis(
					glyphRun,
					pm,
					(DWRITE_RENDERING_MODE1)renderingMode,
					measuringMode,
					DWRITE_GRID_FIT_MODE_DEFAULT,
					DWRITE_TEXT_ANTIALIAS_MODE_CLEARTYPE,
					baselineOriginX,
					baselineOriginY,
					glyphRunAnalysis
				);
				f->Release();
			}
		}
		if (FAILED(hr) && renderingMode != DWRITE_RENDERING_MODE_ALIASED) {
			IDWriteFactory2* f;
			hr = This->QueryInterface(&f);
			if (SUCCEEDED(hr)) {
				DWRITE_MATRIX m;
				DWRITE_MATRIX const* pm = transform;
				if (pm) {
					m = *transform;
					m.m11 *= pixelsPerDip;
					m.m22 *= pixelsPerDip;
					pm = &m;
				}
				hr = f->CreateGlyphRunAnalysis(
					glyphRun,
					pm,
					renderingMode,
					measuringMode,
					DWRITE_GRID_FIT_MODE_DEFAULT,
					DWRITE_TEXT_ANTIALIAS_MODE_CLEARTYPE,
					baselineOriginX,
					baselineOriginY,
					glyphRunAnalysis
				);
				f->Release();
			}
		}
		
		if (FAILED(hr) && renderingMode != DWRITE_RENDERING_MODE_ALIASED) {
			hr = This->CreateGlyphRunAnalysis(
				glyphRun,
				pixelsPerDip,
				transform,
				DirectWriteParams.RenderingMode,
				measuringMode,
				baselineOriginX,
				baselineOriginY,
				glyphRunAnalysis
			);
		}
		if (FAILED(hr)) {
			hr = This->CreateGlyphRunAnalysis(
				glyphRun,
				pixelsPerDip,
				transform,
				renderingMode,
				measuringMode,
				baselineOriginX,
				baselineOriginY,
				glyphRunAnalysis
			);
		}
		if (SUCCEEDED(hr)) {
			hookIDWriteGlyphRunAnalysisIfStill(*glyphRunAnalysis);
		}
		return hr;
	}
}

namespace Impl_IDWriteFactory2
{
	static HRESULT WINAPI CreateGlyphRunAnalysis(
		IDWriteFactory2* This,
		DWRITE_GLYPH_RUN const* glyphRun,
		DWRITE_MATRIX const* transform,
		DWRITE_RENDERING_MODE renderingMode,
		DWRITE_MEASURING_MODE measuringMode,
		DWRITE_GRID_FIT_MODE gridFitMode,
		DWRITE_TEXT_ANTIALIAS_MODE antialiasMode,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		IDWriteGlyphRunAnalysis** glyphRunAnalysis
	) {
		HRESULT hr = E_FAIL;
		if (FAILED(hr) && renderingMode != DWRITE_RENDERING_MODE_ALIASED) {
			IDWriteFactory3* f;
			hr = This->QueryInterface(&f);
			if (SUCCEEDED(hr)) {
				hr = f->CreateGlyphRunAnalysis(
					glyphRun,
					transform,
					(DWRITE_RENDERING_MODE1)renderingMode,
					measuringMode,
					gridFitMode,
					antialiasMode,
					baselineOriginX,
					baselineOriginY,
					glyphRunAnalysis
				);
				f->Release();
			}
		}
		if (FAILED(hr) && renderingMode != DWRITE_RENDERING_MODE_ALIASED) {
			hr = This->CreateGlyphRunAnalysis(
				glyphRun,
				transform,
				DirectWriteParams.RenderingMode,
				measuringMode,
				DirectWriteParams.GridFitMode,
				antialiasMode,
				baselineOriginX,
				baselineOriginY,
				glyphRunAnalysis
			);
		}
		if (FAILED(hr)) {
			hr = This->CreateGlyphRunAnalysis(
				glyphRun,
				transform,
				renderingMode,
				measuringMode,
				gridFitMode,
				antialiasMode,
				baselineOriginX,
				baselineOriginY,
				glyphRunAnalysis
			);
		}
		if (SUCCEEDED(hr)) {
			hookIDWriteGlyphRunAnalysisIfStill(*glyphRunAnalysis);
		}
		return hr;
	}
}

namespace Impl_IDWriteFactory3
{
	static HRESULT WINAPI CreateGlyphRunAnalysis(
		IDWriteFactory3* This,
		DWRITE_GLYPH_RUN const* glyphRun,
		DWRITE_MATRIX const* transform,
		DWRITE_RENDERING_MODE1 renderingMode,
		DWRITE_MEASURING_MODE measuringMode,
		DWRITE_GRID_FIT_MODE gridFitMode,
		DWRITE_TEXT_ANTIALIAS_MODE antialiasMode,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		IDWriteGlyphRunAnalysis** glyphRunAnalysis
	) {
		HRESULT hr = E_FAIL;
		if (FAILED(hr) && renderingMode != DWRITE_RENDERING_MODE1_ALIASED) {
			hr = This->CreateGlyphRunAnalysis(
				glyphRun,
				transform,
				DirectWriteParams.RenderingMode1,
				measuringMode,
				DirectWriteParams.GridFitMode,
				antialiasMode,
				baselineOriginX,
				baselineOriginY,
				glyphRunAnalysis
			);
		}
		if (FAILED(hr)) {
			hr = This->CreateGlyphRunAnalysis(
				glyphRun,
				transform,
				renderingMode,
				measuringMode,
				gridFitMode,
				antialiasMode,
				baselineOriginX,
				baselineOriginY,
				glyphRunAnalysis
			);
		}
		if (SUCCEEDED(hr)) {
			hookIDWriteGlyphRunAnalysisIfStill(*glyphRunAnalysis);
		}
		return hr;
	}
}

namespace Impl_IDWriteGdiInterop
{
	static HRESULT WINAPI CreateBitmapRenderTarget(
		IDWriteGdiInterop* This,
		HDC hdc,
		UINT32 width,
		UINT32 height,
		IDWriteBitmapRenderTarget** renderTarget
	) {
		HRESULT hr = This->CreateBitmapRenderTarget(
			hdc,
			width,
			height,
			renderTarget
		);
		if (SUCCEEDED(hr)) {
			hookIDWriteBitmapRenderTargetIfStill(*renderTarget);
		}
		return hr;
	}
}

namespace Impl_IDWriteGlyphRunAnalysis
{
	static HRESULT WINAPI GetAlphaBlendParams(
		IDWriteGlyphRunAnalysis* This,
		IDWriteRenderingParams* renderingParams,
		FLOAT* blendGamma,
		FLOAT* blendEnhancedContrast,
		FLOAT* blendClearTypeLevel
	) {
		HRESULT hr = E_FAIL;
		if (FAILED(hr)) {
			hr = This->GetAlphaBlendParams(
				DirectWriteParams.pDWriteRenderingParams,
				blendGamma,
				blendEnhancedContrast,
				blendClearTypeLevel
			);
		}
		if (FAILED(hr)) {
			hr = This->GetAlphaBlendParams(
				renderingParams,
				blendGamma,
				blendEnhancedContrast,
				blendClearTypeLevel
			);
		}
		return hr;
	}
}



static void hookIDWriteBitmapRenderTarget(IDWriteBitmapRenderTarget* pDWriteBitmapRenderTarget) {
	void** v = getVtbl(pDWriteBitmapRenderTarget);
	hook(v[3], Impl_IDWriteBitmapRenderTarget::DrawGlyphRun);
}

static void hookIDWriteFactory(IDWriteFactory* pDWriteFactory) {
	void** v = getVtbl(pDWriteFactory);
	hook(v[17], Impl_IDWriteFactory::GetGdiInterop);
	hook(v[23], Impl_IDWriteFactory::CreateGlyphRunAnalysis);
}

static void hookIDWriteFactory2(IDWriteFactory2* pDWriteFactory2) {
	void** v = getVtbl(pDWriteFactory2);
	hook(v[30], Impl_IDWriteFactory2::CreateGlyphRunAnalysis);
}

static void hookIDWriteFactory3(IDWriteFactory3* pDWriteFactory3) {
	void** v = getVtbl(pDWriteFactory3);
	hook(v[31], Impl_IDWriteFactory3::CreateGlyphRunAnalysis);
}

static void hookIDWriteGdiInterop(IDWriteGdiInterop* pDWriteGdiInterop) {
	void** v = getVtbl(pDWriteGdiInterop);
	hook(v[7], Impl_IDWriteGdiInterop::CreateBitmapRenderTarget);
}

static void hookIDWriteGlyphRunAnalysis(IDWriteGlyphRunAnalysis* pDWriteGlyphRunAnalysis) {
	void** v = getVtbl(pDWriteGlyphRunAnalysis);
	hook(v[5], Impl_IDWriteGlyphRunAnalysis::GetAlphaBlendParams);
}



static void hookIDWriteBitmapRenderTargetIfStill(IDWriteBitmapRenderTarget* pDWriteBitmapRenderTarget) {
	void** vtbl = getVtbl(pDWriteBitmapRenderTarget);
	auto lock = globalMutex.getLock();
	if (insertVtbl(vtbl)) {
		hookIDWriteBitmapRenderTarget(pDWriteBitmapRenderTarget);
	}
}

static void hookIDWriteFactoryIfStill(IDWriteFactory* pDWriteFactory) {
	void** vtbl = getVtbl(pDWriteFactory);
	auto lock = globalMutex.getLock();
	if (insertVtbl(vtbl)) {
		hookIDWriteFactory(pDWriteFactory);
		hookIfImplemented(pDWriteFactory, hookIDWriteFactory2);
		hookIfImplemented(pDWriteFactory, hookIDWriteFactory3);
	}
}

static void hookIDWriteGdiInteropIfStill(IDWriteGdiInterop* pDWriteGdiInterop) {
	void** vtbl = getVtbl(pDWriteGdiInterop);
	auto lock = globalMutex.getLock();
	if (insertVtbl(vtbl)) {
		hookIDWriteGdiInterop(pDWriteGdiInterop);
	}
}

static void hookIDWriteGlyphRunAnalysisIfStill(IDWriteGlyphRunAnalysis* pDWriteGlyphRunAnalysis) {
	void** vtbl = getVtbl(pDWriteGlyphRunAnalysis);
	auto lock = globalMutex.getLock();
	if (insertVtbl(vtbl)) {
		hookIDWriteGlyphRunAnalysis(pDWriteGlyphRunAnalysis);
	}
}



namespace Impl
{
	static HRESULT WINAPI DWriteCreateFactory(
		__in   DWRITE_FACTORY_TYPE factoryType,
		__in   REFIID iid,
		__out  IUnknown **factory
	) {
		HRESULT hr = ::DWriteCreateFactory(factoryType, iid, factory);
		if (SUCCEEDED(hr)) {
			IDWriteFactory* pDWriteFactory;
			HRESULT hr2 = (*factory)->QueryInterface(&pDWriteFactory);
			if (SUCCEEDED(hr2)) {
				hookIDWriteFactoryIfStill(pDWriteFactory);
				pDWriteFactory->Release();
			}
		}
		return hr;
	}
}



extern void hookDirectWrite() {
	HMODULE hModuleDWrite = GetModuleHandleW(L"dwrite.dll");
	if (!hModuleDWrite) return;
	
	FARPROC DWriteCreateFactoryProc = GetProcAddress(hModuleDWrite, "DWriteCreateFactory");
	if (DWriteCreateFactoryProc) {
		auto orig = (decltype(&Impl::DWriteCreateFactory))DWriteCreateFactoryProc;
		hook(orig, Impl::DWriteCreateFactory);
	}
}
