#include <iostream>
#include <Windows.h>
#include "test_ds.h"
#include "test_list.h"
#include "test_register.h"

test_collection online_tests;

/* Basic Win32 based testing framework
 *
 * wchen329@wisc.edu
 */
int main(int argc, char ** argv)
{

	/* Main execution path
	 *
	 */
    std::cout << "pRISCAS Testing Suite for Windows NT" << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	std::cout << "Collecting tests..." << std::endl;

	// Add tests
	Setup_Tests();

	std::cout << "Found " << online_tests.get_test_count() << " tests." << std::endl;

	std::cout << "Running tests..." << std::endl;

	if(online_tests.run_all_tests())
	{
		std::cout << "Exiting with code 1 [failure]" << std::endl;
		std::cin.get();
		return 1;
	}

	std::cout << "---TESTING FINISHED---" << std::endl;
	std::cout << "Exiting with code 0 [success]" << std::endl;
	std::cin.get();
    return 0;
}
