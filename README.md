# sierra-engine-editor
Sierra Engine Editor source tree

## Description
Sierra Engine Editor is the Editor component for the Sierra Engine.

In a nutshell, it allows users to create their own games, and the Editor bundles the necessary tools to do so.

The Editor bundles and compiles your project into a format a format that allows Sierra Engine to understand and run it (Creates an executable that links to Sierra Engine library and acts as a middle layer between the Engine and Assets).

The Editor will generally only build for it's own native platform, but the Engine supports cross-compilation to other platforms.

## Building
To build Sierra Engine Editor, you will need to have the following dependencies installed:
- Qt 6.8 or later
- CMake 3.16 or later
- A C++23 compatible compiler (GCC, Clang, MSVC)

Clone the repository:
```bashgit clone 
git clone https://github.com/SierraFiveGroup/sierra-engine-editor.git
```
Create a build directory and navigate into it:
```bash
mkdir build
cd build
```
Run CMake to configure the project:
```bash
cmake ..
```
Build the project using your preferred build system:
```bash
make -j$(nproc)  # For Unix-based systems
```
or
```bash
cmake --build . --config Release  # For Windows with MSVC
```

## Legal
This project is licensed under the GNU Lesser General Public License v2.1 ONLY (LGPL v2.1). See the LICENSE file for details.

### License Exceptions
This project includes code that is licensed under different terms. See the LICENSE file for details.
Files included in the "lresources" directory are NOT covered by the LGPL v2.1 license. For the complete notice, please refer to lresources/NOTICE.txt.