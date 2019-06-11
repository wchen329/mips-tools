#include "stdafx.h"
#include "test_ds.h"
#include "test_list.h"
#include <iostream>

using namespace System;

/* Basic CLI/CLR based testing framework
 *
 * wchen329@wisc.edu
 */
int main(int argc, char ** argv)
{
	/* This section is used for running the tests themselves.
	 * It's NOT recommended to try to use this manually, otherwise
	 * you'll probably get a segmentation fault...
	 */
	if(argc >= 2)
	{
		if(argc != 3)
		{
			std::cout << "Incorrect arguments. Please try again." << std::endl;
		}
		if(strcmp(argv[1], "-t") != 0) return -1;
		void(*funct)() = (void(*)())atoi(argv[2]);
		funct();
		return 0;
	}

	/* Main execution path
	 *
	 */
    Console::WriteLine(L"MIPS Tools Testing Suite for Windows NT");
	Console::WriteLine(L"---------------------------------------");
	Console::WriteLine(L"Collecting tests...");

	test_collection online_tests = test_collection();

	// Add tests
	online_tests.add_test(test_assert, "test_assert");
	online_tests.add_test(test_register_32, "test_register_32");
	online_tests.add_test(test_sc_cpu_ops, "test_sc_cpu_ops");
	online_tests.add_test(test_shell_comments, "test_shell_comments");
	online_tests.add_test(test_sc_cpu_loop, "test_sc_cpu_loop");

	std::cout << "Found " << online_tests.get_test_count() << " tests." << std::endl;

	Console::WriteLine(L"Running tests...");

	if(online_tests.run_all_tests())
	{
		std::cout << "Exiting with code 1 [failure]" << std::endl;
		std::cin.get();
		return 1;
	}

	Console::WriteLine(L"---TESTING FINISHED---");
	std::cout << "Exiting with code 0 [success]" << std::endl;
	std::cin.get();
    return 0;
}
