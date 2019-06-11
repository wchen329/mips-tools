#include <string>
#include <vector>
#include "shell.h"

/* Main routine for Shell
 * 
 */
int main(int argc, char ** argv)
{
	std::vector<std::string> args;

	for(int carg = 0; carg < argc; carg++)
	{
		args.push_back(std::string(argv[carg]));
	}

	return shell(args);
}