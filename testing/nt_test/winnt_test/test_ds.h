#ifndef __TEST_DS_H__
#define __TEST_DS_H__
#include <cstdio>
#include <map>
#include <string>
#include <vector>

class test_collection
{
	int ticket;
	std::vector<void(*)()> tests;
	std::map<int, void(*)()> id_to_test;
	std::map<void(*)(), std::string> test_to_name;

	public:
		size_t get_test_count() { return this->tests.size() ; }
		void add_test(void(*name)(), std::string testname)
		{
			tests.push_back(name);
			std::pair<void(*)(), std::string> member(name, testname);
			test_to_name.insert(member);
			std::pair<int, void(*)()> mem_of_ind(ticket, name);
			id_to_test.insert(mem_of_ind);
			ticket++;
		}

		int run_test(int index);
		int run_all_tests();
		
		test_collection() : ticket(0)
		{

		}
};

#endif