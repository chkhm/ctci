
# Using Boost under Windows with Visual Studio 2022 and cmake

Making Boost work under Windows with Visual Studio while using cmake is not well-documented. 

This approach works for me:

### Download and install vcpkg

vcpkg is a reasonably simple way for downloading, compiling, and "installing" popular open source libraries under Windows.
The steps how to install it are described [here](https://vcpkg.io/en/getting-started.html 'vcpkg install page') in detail.

The following are the abbreviated instructions, assuming you put your git projects into the folder _D:\git_.

Open a powershell.

Clone vcpkg:
```powershell
D:
cd git
git clone https://github.com/Microsoft/vcpkg.git
```

Now, run bootstrap:
```powershell
.\vcpkg\bootstrap-vcpkg.bat
```

Install Boost (important: you usually need the 64 bit version. vcpkg by default installs the 32 bit version.). This step takes a bit of time (~15 minutes):
```powershell
.\vcpkg\vcpkg.exe install boost:x64-windows
```

Integrate with MSBuild (this step does not help with cmake but helps if you work with Visual Studio projects that use a sln solution file):
```powershell
.\vcpkg\vcpkg.exe integrate install
```

Now you can create your CMakeLists.txt file. This is how it should look like:
```CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# set the project name
project(myproject)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED Tru

# This is the important part, not mentioned anywhere!
set(_VCPKG_INSTALLED_DIR D:/git/vcpkg/installed/x64-windows)

set(BOOST_ROOT ${_VCPKG_INSTALLED_DIR})
set(Boost_DIR ${_VCPKG_INSTALLED_DIR})
set(Boost_NO_SYSTEM_PATHS ON)

find_package(Boost 1.79.0 )
if (Boost_FOUND)
	include_directories( ${Boost_INCLUDE_DIRS} )
	add_executable(myexecutable myexecutable.cpp)
	#target_link_libraries(myexecutable Boost::whatever)
endif()
```

Apparently the key is to set the _VCPKG_INSTALLED_DIR! You can probably set this also somewhere in the CMakeSettings.json file. Let me know if you
find a way to put it there.

Funnyly enough, I did not need to specify a target link library yet. Maybe, this will be neccesary, if I use more Boost functionality.

My CMakeSetting.json looks like this:
```json
{
  "configurations": [
    {
      "name": "x64-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${projectDir}\\out\\build\\${name}",
      "installRoot": "${projectDir}\\out\\install\\${name}",
      "buildCommandArgs": "",
      "ctestCommandArgs": "",
      "cmakeToolchain": "D:/git/vcpkg/scripts/buildsystems/vcpkg.cmake"
    }
  ]
}
```

If you want Visual Studio syntax highlighting being able to find the Boost header files, you need to modify the CppProperties.json of your project:
```json
{
  "configurations": [
    {
      "inheritEnvironments": [
        "msvc_x64"
      ],
      "name": "x64-Debug",
      "includePath": [
        "${env.INCLUDE}",
        "D:/git/vcpkg/installed/x64-windows/include",
        "${workspaceRoot}\\**"
      ],
      "defines": [
        "WIN32",
        "_DEBUG",
        "UNICODE",
        "_UNICODE"
      ],
      "intelliSenseMode": "windows-msvc-x64",
      "compilerSwitches": "std=c++20"
    }
  ]
}
```

Here is a tiny code snippet, to check out if this works:
```c++
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

int main(int argc, char** argv) {
	string str = "42";
	int x = boost::lexical_cast<int>(str);
	cout << "x: " << x << endl;

	return 0;
}
```

This integrates and compiles for me with Visual Studio 2022 Community Edition.
