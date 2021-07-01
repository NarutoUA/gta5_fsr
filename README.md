# Grand Theft Auto V + FidelityFx Super Resolution 1.0
This mod replaces original Grand Theft Auto V upscaler with [FidelityFx Super Resolution 1.0](https://www.amd.com/en/technologies/radeon-software-fidelityfx-super-resolution)

[![Build status](https://ci.appveyor.com/api/projects/status/dlc9l4bitc5knyf5?svg=true)](https://ci.appveyor.com/project/NarutoUA/gta5-fsr)

## Original upscaler VS FidelityFx Super Resolution:
[![youtube](http://img.youtube.com/vi/yN3tfoUUyos/0.jpg)](http://www.youtube.com/watch?v=yN3tfoUUyos "Original upscaler vs FidelifyFx Super Resolution")

### ScreenshotComparison:
1. [Original upscale VS FSR upscale](https://screenshotcomparison.com/comparison/15394)
2. [Native VS FSR #1](https://screenshotcomparison.com/comparison/15427)
3. [Native VS FSR #2](https://screenshotcomparison.com/comparison/15428)

### Installation:
1. Get the latest [release](https://github.com/NarutoUA/gta5_fsr/releases)
2. Copy `d3d11.dll` and `gta5_fsr.ini` to Grand Theft Auto V root folder (e.g. \Steam\steamapps\common\Grand Theft Auto V)
3. Go to ingame settings `Advanced Graphics Settings` and set `Frame scaling mode`:
   + 0.5x - Performance Mode
   + 0.667x - Balanced Mode
   + 0.75x - Quality Mode
   + 0.883x - Ultra Quality Mode
4. Check `gta5_fsr.ini` file if you need more control over mod settings

### Compatibility:
| ENB | FiveM | RAGE.MP |
| :---: | :-----: | :-------: |
| ✔️<sup>1</sup>  | ❓  | ❓  |

1. Rename this mod `d3d11.dll` to `d3d11_fsr.dll` and edit `enblocal.ini` file:  

```
[PROXY]
EnableProxyLibrary=true
InitProxyFunctions=true
ProxyLibrary=d3d11_fsr.dll
```
