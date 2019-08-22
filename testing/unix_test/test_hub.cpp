#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <string>
#include <vector>
#include "test_list.h"

std::vector<std::string> test_names;
std::vector<void(*)()> runnable_tests;

void register_test(std::string name, void(*test)())
{
	runnable_tests.push_back(test);
	test_names.push_back(name);
}

int main()
{
	fprintf(stdout, "PRISCAS Testing Suite for UNIX and UNIX-Like Systems\n");

	size_t passes = 0;
	int ret_code = 0;
	
	// Add tests here
	register_test("test_assert", test_assert);
	register_test("test_mips32fsp_bigstallforward", test_mips32fsp_bigstallforward);
	register_test("test_mips32fsp_core_instructions", test_mips32fsp_core_instructions);
	register_test("test_mips32fsp_forwarding_basic", test_mips32fsp_forwarding_basic);
	register_test("test_mips32fsp_forwarding_control", test_mips32fsp_forwarding_control);
	register_test("test_mips32fsp_load_to_use", test_mips32fsp_load_to_use);
	register_test("test_mips32fsp_load_to_use_control", test_mips32fsp_load_to_use_control);
	register_test("test_mips32fsp_memtomem", test_mips32fsp_memtomem);
	register_test("test_mips32fsp_nostall", test_mips32fsp_nostall);
	register_test("test_mips32fsp_registers", test_mips32fsp_registers);
	register_test("test_mips32fsp_subroutine", test_mips32fsp_subroutine);
	register_test("test_register_32", test_register_32);
	register_test("test_sc_cpu_core_instructions", test_sc_cpu_core_instructions);
	register_test("test_sc_cpu_loop", test_sc_cpu_loop);
	register_test("test_sc_cpu_ops", test_sc_cpu_ops);
	register_test("test_sc_cpu_registers", test_sc_cpu_registers);
	register_test("test_sc_cpu_subroutine", test_sc_cpu_subroutine);
	register_test("test_shell_comments", test_shell_comments);
	register_test("test_shell_multidirectsymbol", test_shell_multidirectsymbol);

	fprintf(stdout, "%ld tests detected...\n", runnable_tests.size());

	fprintf(stdout, "Running tests...\n");

	for(size_t tc = 0; tc < runnable_tests.size(); tc++)
	{
		int ret_value;
		fprintf(stdout, "Running test [%ld]: %s ", tc, test_names[tc].c_str());
		fflush(stdout);

		pid_t id = fork();
		if(id == 0)
		{
			runnable_tests[tc]();
			exit(0);		
		}

		waitpid(id, &ret_value, 0); 
		if(ret_value == 0)
		{
			fprintf(stdout, "PASSED\n");
			passes++;
		}

		else
		{
			fprintf(stdout, "FAILED\n");
			ret_code = 1;
		}
		fflush(stdout);
	}

	fprintf(stdout, "%ld out of %ld tests passed.\n", passes, test_names.size());

	if(ret_code != 0) fprintf(stdout, "Exiting with code 1 [failure]\n");
	else fprintf(stdout, "Exiting with code 0 [success]\n");	
	return ret_code;
}
