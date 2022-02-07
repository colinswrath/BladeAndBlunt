This is a CommonLib implementation of a .NETScriptFramework plugin originally written by KernalsEgg. 
Much of the credit for the RE work goes to KernalsEgg.

This was made for Blade and Blunt - A Combat Overhaul by SimonMagus616.

##What does it do?
**Adds specified spells in ini for the following scenarios:**  
`IsAttacking`  
`IsBlocking`  
`IsSneaking`  
`When drawing bow` (used for bow stamina drain)  
  
**Includes engine fixes/changes for:**  
`Movement speed for player scale` (Needs to be verified)  
`Block cap should use fBlockMax instead of fPlayerMaxResistance` (also needs to be verified)  

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/colinswrath/BladeAndBlunt
cd ExamplePlugin-CommonLibSSE
git submodule update --init --recursive
cmake -B build -S .
```
