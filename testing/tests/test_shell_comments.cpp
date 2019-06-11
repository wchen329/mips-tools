#include <string>
#include <vector>
#include "mb.h"
#include "shell.h"

#ifndef WIN32
int main()
{
#else
void test_shell_comments()
{
	using namespace mips_tools;
	std::string flag_i = "-i";
	std::string file_name = "data/comments.s";
	std::vector<std::string> args;
	args.push_back(flag_i);
	args.push_back(file_name);
	mb s;
	shell(args, s);
}
#endif