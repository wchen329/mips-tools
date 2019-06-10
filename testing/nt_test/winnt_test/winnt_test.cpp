#include "stdafx.h"
#include "test_ds.h"
#include "test_list.h"
#include <iostream>

using namespace System;

int main(int argc, char ** argv)
{
	if(argc >= 2)
	{
		if(argc == 2)
		{
			std::cout << "Insufficient arguments. Please try again." << std::endl;
		}
		if(strcmp(argv[1], "-t") != 0) return -1;
		void(*funct)() = (void(*)())atoi(argv[2]);
		funct();
		return 0;
	}

    Console::WriteLine(L"MIPS Tools Testing Suite");
	Console::WriteLine(L"------------------------");
	Console::WriteLine(L"Collecting tests...");

	test_collection online_tests = test_collection();

	// Add tests
	online_tests.add_test(test_assert, "test_assert");
	online_tests.add_test(test_register_32, "test_register_32");

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
