# MIPS-Tools
MIPS-Tools is a collection of tools that may be useful in the aid of computer architecture study, particularly for the MIPS architecture. It is intended to be fully functional on both Windows and UNIX-like systems upon its completion.

## Build Instructions
### Windows
Within the MIPS Tools directory there is a Visual Studio solution. This Visual Studio solution is for VS2010 and it should be able to be opened in any Visual Studio that is newer that 2010 as well. From there it is as simple to run "Build" and the project should build.

### UNIX and UNIX-like
Compilation on UNIX systems can be done using one of the many make utilities. gmake is known to work. To make all applications available to UNIX systems in the MIPS Tools suite (currently the MIPS Tools Shell), just type in the following command: `make` And the make application should do the rest. After make is finished, just go to the `bin` directory (which will be created if it doesn't already exist) and run the `mtshell` application created.
#### 
Other useful targets:
#### `clean` - clean the solution, remove executables and objects
