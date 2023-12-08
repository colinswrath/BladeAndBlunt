This is a CommonLib implementation of a .NETScriptFramework plugin originally written by KernalsEgg. 
Much of the credit for the RE work goes to KernalsEgg.

This was made for Blade and Blunt - A Combat Overhaul by SimonMagus616.

## What does it do?  
**Adds specified spells in ini for the following scenarios:**  
`IsAttacking`  
`IsBlocking`  
`IsSneaking`
`IsCasting`
`Spell when drawing bow`  
`Spell when firing xbow`  
`Spell when reloading xbow`  

**Includes engine fixes/changes for:**  
`Movement speed for player scale`  
`Block cap will use fBlockMax instead of fPlayerMaxResistance`  
`Spell absorption cap will no longer exceed fPlayerMaxResistance`    

**Other features**  
`Override for new armor rating scaling formula`    
`Override Block and Bash stamina cost with corresponding perks set in ini`  
`Injury system will apply injury stages based on damage taken`  
`Support for injury health AV penalty`  
`Handles parry and block stagger spells`  
`Scans load order for all weapons and zeroes stagger values to let Simonrim handle stagger`
`Replaces power attack keyword to differentiate from dual wield power attacks and power attacks while dual wielding`

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
