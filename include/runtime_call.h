#ifndef __RUNTIME_CALL_H__
#define __RUNTIME_CALL_H__
#include <vector>
#include "diag_cpu.h"
#include "mem.h"
#include "mb.h"
#include "parser_err.h"
#include "shell.h"

namespace mipsshell
{
	void Enter_Interactive(int arg);
	void breakpoint(std::vector<std::string> & args, Shell& inst);
	void cycle(std::vector<std::string> & args, Shell& inst);
	void cpuopts(std::vector<std::string> & args, Shell& inst);
	void exit(std::vector<std::string> & args, Shell& inst);
	void help(std::vector<std::string> & args, Shell& inst);
	void mem(std::vector<std::string> & args, Shell& inst);
	void pci(std::vector<std::string> & args, Shell& inst);
	void power(std::vector<std::string> & args, Shell& inst);
	void rst(std::vector<std::string> & args, Shell& inst);
	void run(std::vector<std::string> & args, Shell& inst);
	void sound(std::vector<std::string> & args, Shell& inst);
	void state(std::vector<std::string> & args, Shell& inst);
	void time(std::vector<std::string> & args, Shell& inst);
	void trace(std::vector<std::string> & args, Shell& inst);
	void vga(std::vector<std::string> & args, Shell& inst);
}

#endif