# Grand Theft Auto V + FidelityFx Super Resolution 1.0
This mod replaces original Grand Theft Auto V upscaler with [FidelityFx Super Resolution 1.0](https://www.amd.com/en/technologies/radeon-software-fidelityfx-super-resolution)

[![Build status](https://ci.appveyor.com/api/projects/status/dlc9l4bitc5knyf5?svg=true)](https://ci.appveyor.com/project/NarutoUA/gta5-fsr)

## Original upscaler VS FidelityFx Super Resolution:
[![youtube](http://img.youtube.com/vi/yN3tfoUUyos/0.jpg)](http://www.youtube.com/watch?v=yN3tfoUUyos "Original upscaler vs FidelifyFx Super Resolution")

### Screenshots:
1. [Original upscale VS FSR upscale](https://imgsli.com/NjAzMTI)
2. [Native VS FSR #1](https://imgsli.com/NjAyODk)
3. [Native VS FSR #2](https://imgsli.com/NjAyOTA)

#### Other videos (benchmarks, reviews, comparisons):
1. [GTA 5 4K | AMD FSR FidelityFX Super Resolution (MOD) ON/OFF | RTX 3090 | R7 5800X | Ultra Settings](https://www.youtube.com/watch?v=_wbAEirahwM) *by Jansn Benchmarks*
2. [Grand Theft Auto V FSR (mod) 4K - RX6900 XT + R9 5950X - Ultra Settings - Benchmark](https://www.youtube.com/watch?v=2A40FdqrKNA) *by 程國龍*
### Installation:
1. Get the latest [release](https://github.com/NarutoUA/gta5_fsr/releases)
2. Copy `d3d11.dll` and `gta5_fsr.ini` to Grand Theft Auto V root folder (e.g. \Steam\steamapps\common\Grand Theft Auto V)
3. Go to ingame `Graphics` settings and make sure `DirectX Version` is set to `DirectX 11`
4. Go to ingame `Advanced Graphics` settings and set `Frame scaling mode`:
   + 0.5x - Performance Mode
   + 0.667x - Balanced Mode
   + 0.75x - Quality Mode
   + 0.883x - Ultra Quality Mode
5. Check `gta5_fsr.ini` file if you need more control over mod settings

### Compatibility:
| **Name** | **Compatibility** | **Comment** |
| :------- | :---------------: | :---------- |
| ENB      | ✔️ | Rename  `d3d11.dll` (enb) to `d3d11_enb.dll`. Copy `d3d11.dll` (fsr) to game folder where enb was located. Open `gta5_fsr.ini` file and set `EnableProxyLibrary=true`,  `InitProxyFunctions=true` and `ProxyLibrary=d3d11_enb.dll` |
| GTA:O         | ✔️ | Check [**GTA Online**](#gta-online) section below for more details |
| FiveM         | ✔️ | Change `Graphics -> Render Resolution` instead of `Advanced Graphics -> Frame scaling mode` |
| RAGE.MP       | ✔️ | No issues found |
| ReShade       | ✔️  | Open `gta5_fsr.ini` file and set `EnableProxyLibrary=true`,  `InitProxyFunctions=false` and `ProxyLibrary=d3d11.dll` |
| R.E.A.L. (VR) | ❌ | Check [this](https://github.com/NarutoUA/gta5_fsr/issues/16#issuecomment-874247378) |

### Troubleshooting
If mod doesn't work or you are not sure if it works:
1. Open `gta5_fsr.ini` end set:
```
PrintDebug=true
```
2. Download and start [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview).
3. Go to "Edit" -> "Filter/Highlight" and set "Include" to "GTA5_FSR"
4. Start game. If mod works you should see messages like:

```
[20736] GTA5_FSR: Loaded original d3d11.dll
[20736] GTA5_FSR: D3D11CreateDevice
[20736] GTA5_FSR: D3D11CreateDeviceAndSwapChain
[20736] GTA5_FSR: Reading config file...
[20736] GTA5_FSR: Creating default resources...
[20736] GTA5_FSR: Initialized
[20736] GTA5_FSR: Updated some resources.
[20736] GTA5_FSR: FSR PASS, FPS: 100, MSAA: 2, FXAA: 0
[20736] GTA5_FSR: FSR PASS, FPS: 101, MSAA: 2, FXAA: 0
[20736] GTA5_FSR: FSR PASS, FPS: 99, MSAA: 2, FXAA: 0
```

If you don't see those lines or you see lines like "GTA5_FSR: Failed to create..." then save log and attach it to new [issue](https://github.com/NarutoUA/gta5_fsr/issues). Additionally, include `settings.xml` (C:\Users\%username%\Documents\Rockstar Games\GTA V\settings.xml) and your PC configuration.

### GTA Online
In general, mod works in GTA Online. If you are worried about being banned... Well, I don't know how GTA Online anticheat works so I can't guarantee that. I tried my best to make it anticheat-friendly based on my knowledge how other anticheats work. This mod *doesn't* require DLL injection, it *doesn't* modify game files, it *doesn't* change ingame memory, setup hooks etc. People ask me to add this mod to ReShade because it is allowed in GTA:Online. First of all, it's impossible. ReShade doesn't work with frame scaling and custom resolutions AFAIK. Secondly, ReShade became allowed because it started disabling Depth Stencil Buffer access, which is widely used in WallHack cheats, if any internet activity is detected (i.e. you are playing online). This mod *doesn't* touch depth buffer at all. Anyway, use this mod in GTA Online at your own risk.
