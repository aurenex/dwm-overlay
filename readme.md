# dwm-overlay

This may be the first DWM overlay project on GitHub that gets a pointer to the IDXGISwapChainDWMLegacy virtual function table without a memory scan, using undocumented GUIDs. By default, this project only works on Windows 10/11. To support older OS versions, you need to update the offsets for the virtual function table.


Sorry for the shitty code, I'll do better later.

![dwm-overlay-preview](https://github.com/aurenex/dwm-overlay/assets/125130325/eae17979-1a9a-4ab8-8b72-555331951adc)

### Bugs
- ~~Debug version causes system to freeze~~.
- Duplicating an overlay in an area that is not updating.
- The overlay disappears after reconnecting the monitor.
- Doesn't work on Win11 24H2+ with NVIDIA graphics cards.

### Compilation
- Clone or [download][repo-download-link] this repository.
- Open the **dwm-overlay** solution file in [Visual Studio IDE][vs-download-link].
- Select the target platform.
- Press `ctrl + shift + b` to compile.

### Usage
- Open any DLL-injector **as administrator**.
- Find the **dwm.exe** process.
- Inject the `dwm-overlay.dll` into process.

Subscribe to my Telegram [channel][tg-channel-link] or make a donation if you want to support me.

```
BTC bc1qcmfgc3780pqr4p93t57k76jhy7yttuzc0pc8pv
ETH 0xa3b829A5D4302De54eE9f3F2cF64409f4D2C1b38
TON UQAGAeIjsUFmRR4FWs7fRGB6CvU1AzC2q9i58Bw5_nZSp1Af
```

[repo-download-link]: <https://github.com/aurenex/dwm-overlay/archive/refs/heads/master.zip>
[vs-download-link]: <https://visualstudio.microsoft.com/downloads/>
[tg-channel-link]: <https://t.me/aurenex>
