/*
*	Undocumented DXGI structures
* 
*	Windows 10 21H2 IoT LTSC
* 	dxgi.dll 10.0.19041.5794
* 
*	https://t.me/rxznve
*	https://t.me/aurenex
*	https://github.com/aurenex
*/

#ifndef DXGI_UNDOCUMENTED_H_GUARD
#define DXGI_UNDOCUMENTED_H_GUARD

// DXGI_SCROLL_RECT 24 (0x18) bytes
typedef struct _DXGI_SCROLL_RECT
{
	POINT offset;
	RECT region;
} DXGI_SCROLL_RECT;

// DXGI_PRESENT_MULTIPLANE_OVERLAY 136 (0x88) bytes
typedef struct _DXGI_PRESENT_MULTIPLANE_OVERLAY
{
	UINT LayerIndex;
	UINT Flags;
	char unknown1[0x14];
	RECT SrcRect;
	RECT DstRect;
	RECT ClipRect;
	DXGI_MODE_ROTATION Rotation;
	char unknown2[0x04]; // Maybe blend
	UINT DirtyRectsCount;
	RECT* pDirtyRects;
	char unknown3[0x28];
} DXGI_PRESENT_MULTIPLANE_OVERLAY;

MIDL_INTERFACE("f69f223b-45d3-4aa0-98c8-c40c2b231029")
IDXGISwapChainDWMLegacy : public IDXGIDeviceSubObject
{
	virtual HRESULT STDMETHODCALLTYPE Present
	(
		UINT SyncInterval,
		UINT Flags
	) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetBuffer
	(
		UINT Buffer,
		REFIID riid,
		void** ppSurface
	) = 0;
};

MIDL_INTERFACE("713f394e-92ca-47e7-ab81-1159c2791e54")
IDXGIFactoryDWM : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateSwapChain
	(
		IUnknown* pDevice,
		DXGI_SWAP_CHAIN_DESC* pDesc,
		IDXGIOutput* pOutput,
		IDXGISwapChainDWMLegacy** ppSwapChainDWM
	) = 0;
};

HRESULT (STDMETHODCALLTYPE* PresentDWM)
(
	IDXGISwapChainDWMLegacy* pSwapChain,
	UINT SyncInterval,
	UINT PresentFlags,
	UINT DirtyRectsCount,
	const RECT* pDirtyRects,
	UINT ScrollRectsCount,
	const DXGI_SCROLL_RECT* pScrollRects,
	IDXGIResource* pResource,
	UINT FrameIndex // ???
);

HRESULT (STDMETHODCALLTYPE* PresentMultiplaneOverlay)
(
	IDXGISwapChainDWMLegacy* pSwapChain,
	UINT SyncInterval,
	UINT PresentFlags,
	DXGI_HDR_METADATA_TYPE MetadataType,
	const VOID* pMetadata,
	UINT OverlayCount,
	const DXGI_PRESENT_MULTIPLANE_OVERLAY* pOverlays
);

#endif //DXGI_UNDOCUMENTED_H_GUARD