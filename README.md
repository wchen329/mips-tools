# MIPS-Tools
MIPS-Tools is a collection of tools that may be useful in the aid of computer architecture study, particularly for the MIPS architecture. It is intended to be fully functional on both Windows and UNIX-like systems upon its completion.

## Build Instructions
### Windows
Within the MIPS Tools directory there is a Visual Studio solution. This Visual Studio solution is for VS2010 and it should be able to be opened in any Visual Studio that is newer that 2010 as well. From there it is as simple to run "Build" and the project should build.

### UNIX (applies to BSD, Linux)
UNIX compiling can be done currently by grouping the needed source files in g++ and compiling. A standard UNIX/GNU Makefile is soon to arive though!

## Using
To use MIPS-Tools directly, run the *MIPS Tools Shell"*.

## Features
The following is a list of planned features for the simulator:
* Basic architecture support for simulating a CPU and a single memory system
* MIPS raw assembler (with no executable formatting)
* Latency and cycle verbosity support
* Default single cycle CPU and five stage pipeline support (and pipeline diagrams in ASCII!)
* SMT emulation support
* R10k processor simulation support
* Custom processor and ISA (x86?) support through dynamic linking
