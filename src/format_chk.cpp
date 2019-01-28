#include <cstdio>
#include <vector>
#include "format_chk.h"
#include "parser_err.h"

namespace mipsshell
{

	// Returns register number corresponding with argument if any
	// Returns -1 if invalid or out of range
	int get_reg_num(char * reg_str)
	{
		std::vector<char> numbers;
		int len = strlen(reg_str);
		if(len <= 1) throw new badformat_err();
		for(int i = 1; i < len; i++)
		{
			if(reg_str[i] >= '0' && reg_str[i] <= '9')
			{
				numbers.push_back(reg_str[i]);
			}

			else if(reg_str[i] == ',' && i == len - 1) continue;

			else throw new badformat_err();
		}

		int num = -1;

		if(numbers.empty()) throw new badformat_err();
		else
		{
			char * num_str = new char[numbers.size()];

			int k = 0;
			for(std::vector<char>::iterator itr = numbers.begin(); itr < numbers.end(); itr++)
			{
				num_str[k] = *itr;
				k++;
			}
			num = atoi(num_str);
			delete[] num_str;
		}

		return num;
	}

	// Returns immediate value if valid
	int get_imm(char * str)
	{
		int len = strlen(str);
		
		for(int i = 0; i < len; i++)
		{
			if(str[i] < '0' || str[i] > '9')
			{
				if(i != 0 && str[i] != '-')
					throw new badimm_err();
			}
		}

		return atoi(str);
	}
}