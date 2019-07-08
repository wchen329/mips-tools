# PRISCAS
PRISCAS (*formerly MIPS Tools*) is a collection of tools that may be useful in the aid of computer architecture study, particularly for the MIPS architecture. It is intended to be fully functional on both Windows and UNIX-like systems upon its completion.

## Build Instructions
### Windows
Within the PRISCAS directory there is a Visual Studio solution. This Visual Studio solution is for VS2010 and it should be able to be opened in any Visual Studio that is newer that 2010 as well. From there it is as simple to run "Build" and the project should build.

Output executables for the shell will be in `PRISCAS\Debug` (if building the Debug target) or `PRISCAS\Release` (if building the Release target). The unit test package exeuctable is `testing\tests\winnt_test.exe`.

simUI is built using Qt `qmake`. The easiest way to build simUI is opening the simUI.pro Qt project file into Qt Creator and building it there.

#### Building the Installer
The installer build process usees the *Inno Setup 6* freeware to build the installer. The installer script is located in `installer`, but before building that script in the Inno Setup Compiler, the Release targets must be built for both simUI *and* PRISCAS Shell (which in turn, must build PRISCAS Core). The installer assumes that simUI is built using Qt 5.3.2 with kit MSVC 2010 w/OpenGL, so change the correspond file entry accordingly.

### UNIX and UNIX-like
Compilation on UNIX systems can be done using one of the many make utilities. gmake is known to work. To make just the Shell, typing `make` will suffice. To build simUI, however, it is required to define QMAKE either as an environment variable or macro in the Makefile. To manually specify the macro type in `make "QMAKE=/qtpath/qmake"`, where `/qtpath/qmake` refers to the path to the qmake application and the make application should do the rest. After make is finished, just go to the `bin` directory (which will be created if it doesn't already exist) and run `mtshell` (PRISCAS Shell) or `simUI` (PRISCAS simUI).
#### 
Other useful targets:
#### `all` - the default target, builds the `mtshell` executable and `simUI` executable with debugging symbols and no optimization (debug)
#### `clean` - clean the solution, remove executables and objects
#### `release` - build the project as a release (that is, without debugging symbols and with optimization on), except simUI which builds as debug
#### `test` - build and run the unit test package
