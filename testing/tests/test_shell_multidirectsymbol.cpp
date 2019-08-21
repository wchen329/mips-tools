#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_shell_multidirectsymbol()
{
	using namespace mips_tools;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/three_exits.s";
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	mipsshell::Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();
	mb & test_m = test_shell.GetMotherboard();
	cpu & c = test_m.get_cpu();
	diag_cpu & single_cycle = dynamic_cast<diag_cpu&>(c);
	BW_32 reg_check = single_cycle.get_reg_data($k0);
	assertEquals(reg_check.AsUInt32(), 999);
}