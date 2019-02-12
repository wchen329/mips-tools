#include "mt_exception.h"
#include "syms_table.h"


namespace mips_tools
{
	void syms_table::put(char * key, BW_32 value)
	{
		this->key_list.push_back(key);
		this->val_list.push_back(value);
	}

	BW_32 syms_table::lookup(char * key)
	{
		// Slow linear lookup, will be changed as needed

		for(int i = 0; i < key_list.size(); i++)
		{
			if(strcmp(key_list[i], key) == 0)
			{
				return val_list[i];
			}
		}

		throw new mt_exception;
	}
}