#include <string>
#include <vector>
#include <cstdio>
#include "basic_assert.h"
#include "sc_cpu.h"
#include "mb.h"
#include "shell.h"

void test_mips32fsp_bigstallforward()
{
	using namespace priscas;
	std::string prog_name = "mtshell";
	std::string flag_i = "-i";
	std::string file_name = "asm/bigstallforward.s";
	std::string cpuspec = "-c";
	std::string cpunum = "1";
	std::vector<std::string> args;
	args.push_back(prog_name);
	args.push_back(flag_i);
	args.push_back(file_name);
	args.push_back(cpuspec);
	args.push_back(cpunum);
	priscas::Shell test_shell;
	test_shell.SetArgs(args);
	test_shell.SetQuiet();
	test_shell.Run();
	mb & test_m = test_shell.GetMotherboard();
	cpu & c = test_m.get_cpu();
	diag_cpu & pipe = dynamic_cast<diag_cpu&>(c);
	assertEquals(pipe.get_reg_data($s0).AsInt32(), -1);
	assertEquals(pipe.get_reg_data($s1).AsInt32(), -1);
	assertEquals(pipe.get_reg_data($s2).AsInt32(), 0x0000FFFF);
	assertEquals(pipe.get_reg_data($s3).AsInt32(), 0x0000FFFF);
	assertEquals(pipe.get_reg_data($k0).AsInt32(), 999);
	assertEquals(test_m.get_cycles(), 18);
}
