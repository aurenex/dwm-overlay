#pragma once

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

HRESULT STDMETHODCALLTYPE PresentDWM
(
	IDXGISwapChainDWMLegacy* pSwapChain,
	UINT SyncInterval,
	UINT PresentFlags,
	UINT DirtyRectsCount,
	const RECT* pDirtyRects,
	UINT ScrollRectsCount,
	const RECT* pScrollRects,
	IDXGIResource* pResource,
	UINT FrameIndex // I'm not sure about the name of this variable
);

HRESULT STDMETHODCALLTYPE PresentMultiplaneOverlay
(
	IDXGISwapChainDWMLegacy* pSwapChain,
	UINT SyncInterval,
	UINT PresentFlags,
	enum DXGI_HDR_METADATA_TYPE MetadataType,
	const void* pMetadata,
	UINT OverlayCount,
	const struct _DXGI_PRESENT_MULTIPLANE_OVERLAY* pOverlays
);