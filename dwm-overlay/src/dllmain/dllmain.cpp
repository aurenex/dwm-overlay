// Yooooo hello bro :3
// HA-HA-HA-HA-HA-HA-HA

#include <Windows.h>
#include <atlbase.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

#include "dxgi/undocumented.h"

#include "minhook/include/minhook.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

decltype(&PresentDWM) fn_PresentDWM = nullptr;
decltype(&PresentMultiplaneOverlay) fn_PresentMultiplaneOverlay = nullptr;

// In a debug build, attempting to use an & operator on a non-null pointer
// causes the system to freeze as the code attempts to display an error message.
// I'll think of something later :D
CComPtr<ID3D11Device> g_Device = nullptr;
CComPtr<ID3D11DeviceContext> g_DeviceContext = nullptr;
CComPtr<ID3D11RenderTargetView> g_RenderTargetView = nullptr;

void draw(IDXGISwapChainDWMLegacy* pSwapChain)
{
	if (!ImGui::GetCurrentContext())
	{
		pSwapChain->GetDevice(IID_PPV_ARGS(&g_Device));
		g_Device->GetImmediateContext(&g_DeviceContext);

		CComPtr<ID3D11Texture2D> RenderTargetTexture = nullptr;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&RenderTargetTexture));
		g_Device->CreateRenderTargetView(RenderTargetTexture, nullptr, &g_RenderTargetView);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(GetDesktopWindow());
		ImGui_ImplDX11_Init(g_Device, g_DeviceContext);
	}

	g_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, nullptr);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("DWM overlay");
	ImGui::Text("Copyright (C) 2024 Aurenex");
	ImGui::Text("https://t.me/aurenex");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HRESULT STDMETHODCALLTYPE hk_PresentDWM
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
)
{
	draw(pSwapChain);

	return fn_PresentDWM(
		pSwapChain,
		SyncInterval,
		PresentFlags,
		DirtyRectsCount,
		pDirtyRects,
		ScrollRectsCount,
		pScrollRects,
		pResource,
		FrameIndex);
}

HRESULT STDMETHODCALLTYPE hk_PresentMultiplaneOverlay
(
	IDXGISwapChainDWMLegacy* pSwapChain,
	UINT SyncInterval,
	UINT PresentFlags,
	enum DXGI_HDR_METADATA_TYPE MetadataType,
	const void* pMetadata,
	UINT OverlayCount,
	const struct _DXGI_PRESENT_MULTIPLANE_OVERLAY* pOverlays
)
{
	draw(pSwapChain);

	return fn_PresentMultiplaneOverlay(
		pSwapChain,
		SyncInterval,
		PresentFlags,
		MetadataType,
		pMetadata,
		OverlayCount,
		pOverlays);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	const KBDLLHOOKSTRUCT* hookStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
	const ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_WndProcHandler(GetDesktopWindow(), 
		static_cast<UINT>(wParam), hookStruct->vkCode, hookStruct->scanCode);

	if (io.WantCaptureKeyboard)
		return -1;

	return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	const MSLLHOOKSTRUCT* hookStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
	const ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_WndProcHandler(GetDesktopWindow(), static_cast<UINT>(wParam),
		hookStruct->flags, MAKELPARAM(hookStruct->pt.x, hookStruct->pt.y));

	if (io.WantCaptureMouse and wParam != WM_MOUSEMOVE)
		return -1;

	return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

DWORD WINAPI MainThread(LPVOID lpParameter)
{
	CComPtr<IDXGIFactory> pFactory = nullptr;
	CreateDXGIFactory(IID_PPV_ARGS(&pFactory));

	CComPtr<IDXGIAdapter> pAdapter = nullptr;
	pFactory->EnumAdapters(0, &pAdapter);

	CComPtr<IDXGIOutput> pOutput = nullptr;
	pAdapter->EnumOutputs(0, &pOutput);

	CComPtr<IDXGIFactoryDWM> pFactoryDWM = nullptr;
	pFactory->QueryInterface(IID_PPV_ARGS(&pFactoryDWM));

	const D3D_FEATURE_LEVEL FeatureLevels []
	{
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_11_0
	};

	CComPtr<ID3D11Device> pDevice = nullptr;
	CComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;

	D3D11CreateDevice
	(
		pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		FeatureLevels,
		ARRAYSIZE(FeatureLevels),
		D3D11_SDK_VERSION,
		&pDevice,
		nullptr,
		&pDeviceContext
	);

	DXGI_SWAP_CHAIN_DESC SwapChainDesc {};
	{
		SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.BufferCount = 1;
	}

	uintptr_t** pSwapChain = nullptr;
	pFactoryDWM->CreateSwapChain(pDevice, &SwapChainDesc, pOutput, (IDXGISwapChainDWMLegacy**)&pSwapChain);

	// Current offsets are supported on Windows 10 - 11
	// Don't forget to update the offsets for your version
	const uintptr_t fpPresentDWM = (*pSwapChain)[16];
	const uintptr_t fpPresentMultiplaneOverlay = (*pSwapChain)[23];

	fn_PresentDWM = reinterpret_cast
		<decltype(fn_PresentDWM)>(fpPresentDWM);

	fn_PresentMultiplaneOverlay = reinterpret_cast
		<decltype(fn_PresentMultiplaneOverlay)>(fpPresentMultiplaneOverlay);

	// You can use vtable hook instead of minhook
	MH_Initialize();
	MH_CreateHook(reinterpret_cast<void*>(fpPresentDWM),
		&hk_PresentDWM, reinterpret_cast<void**>(&fn_PresentDWM));
	MH_CreateHook(reinterpret_cast<void*>(fpPresentMultiplaneOverlay),
		&hk_PresentMultiplaneOverlay, reinterpret_cast<void**>(&fn_PresentMultiplaneOverlay));

	if (MH_EnableHook(MH_ALL_HOOKS))
		return EXIT_FAILURE;

	const HHOOK hMouseHook = SetWindowsHookExW(WH_MOUSE_LL, &LowLevelMouseProc, nullptr, 0);
	const HHOOK hKeyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, &LowLevelKeyboardProc, nullptr, 0);

	// TODO: Add UnhookWindowsHookEx
	if (!hMouseHook or !hKeyboardHook)
		return EXIT_FAILURE;

	MSG message {};

	while (GetMessageW(&message, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	UnhookWindowsHookEx(hMouseHook);
	UnhookWindowsHookEx(hKeyboardHook);

	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, &MainThread, nullptr, 0, nullptr);
	break;

	case DLL_PROCESS_DETACH:
		MH_Uninitialize();
	}

	return TRUE;
}
