#include "stdafx.h"
#include "test_ds.h"
#include "test_list.h"
#include "test_register.h"
#include <iostream>

using namespace System;

test_collection online_tests;

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
    Console::WriteLine(L"PRISCAS Testing Suite for Windows NT");
	Console::WriteLine(L"---------------------------------------");
	Console::WriteLine(L"Collecting tests...");

	// Add tests
	Setup_Tests();

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
