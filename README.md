# MIPS-Tools
MIPS-Tools is a collection of tools that may be useful in the aid of computer architecture study, particularly for the MIPS architecture. It is intended to be fully functional on both Windows and UNIX-like systems upon its completion.

## Build Instructions
### Windows
Within the MIPS Tools directory there is a Visual Studio solution. This Visual Studio solution is for VS2010 and it should be able to be opened in any Visual Studio that is newer that 2010 as well. From there it is as simple to run "Build" and the project should build.

Output executables for the shell and simUI will be in `MIPS Tools\Debug` (if building the Debug target) or `MIPS Tools\Release` (if building the Release target). The unit test package exeuctable is `testing\tests\winnt_test.exe`.

### UNIX and UNIX-like
Compilation on UNIX systems can be done using one of the many make utilities. gmake is known to work. To make all applications available to UNIX systems in the MIPS Tools suite (currently the MIPS Tools Shell), just type in the following command: `make` And the make application should do the rest. After make is finished, just go to the `bin` directory (which will be created if it doesn't already exist) and run the `mtshell` application created.
#### 
Other useful targets:
#### `all` - the default target, builds the `mtshell` executable with debugging symbols and no optimization (debug)
#### `clean` - clean the solution, remove executables and objects
#### `release` - build the project as a release (that is, without debugging symbols and with optimization on)
#### `test` - build and run the unit test package
