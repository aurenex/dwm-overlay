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

long __cdecl PresentDWM
(
	IDXGISwapChainDWMLegacy*,
	unsigned int,
	unsigned int,
	unsigned int,
	struct tagRECT const*,
	unsigned int,
	struct DXGI_SCROLL_RECT const*,
	struct IDXGIResource*,
	unsigned int
);

long __cdecl PresentMultiplaneOverlay
(
	IDXGISwapChainDWMLegacy*,
	unsigned int,
	unsigned int,
	enum DXGI_HDR_METADATA_TYPE,
	void const*,
	unsigned int,
	struct _DXGI_PRESENT_MULTIPLANE_OVERLAY const*
);