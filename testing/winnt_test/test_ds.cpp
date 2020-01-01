#include "test_ds.h"

int test_collection::run_test(int index)
{
	int ret = 0;
	void(*test_routine)() = this->id_to_test.at(index);
	std::string n(this->test_to_name.at(test_routine));

	fprintf(stdout, "Running test [%d] : %s ", index, n.c_str());
	
	//running_test->StartInfo->FileName = "winnt_test.exe";
	//running_test->StartInfo->Arguments = "-t " + addr;
	//running_test->StartInfo->UseShellExecute = false;
	//running_test->StartInfo->RedirectStandardError = true;
	//running_test->Start();
	//running_test->WaitForExit();

	//int status = running_test->ExitCode;

	int status = 0;

	try
	{
		test_routine();
	}
	catch(const Failed_Assert_Exception&)
	{
		status = 1;
	}

	//Timer^ t = gcnew Timer();
	//t->Interval = 5;
	//t->Elapsed() = gcnew(System::EventHandler(this, &Timer::Elapsed());
	//t->Start();

	if(status == 0)
	{
		fprintf(stdout, "PASSED\n");
		ret = 0;
	}

	else
	{
		fprintf(stdout, "FAILED\n");
		fprintf(stdout, "[Debug Information]\n\t");
		//Console::WriteLine(running_test->StandardError->ReadToEnd());
		ret = 1;
	}

	return ret;
}

int test_collection::run_all_tests()
{
	size_t fail = 0;

	for(size_t t = 0; t < this->get_test_count(); t++)
	{
		fail += this->run_test(t);
	}

	size_t pass = this->get_test_count() - fail;

	fprintf(stdout, "%ld of %ld tests passed\n", pass, this->get_test_count());

	if(fail != 0)
		return 1;
	else return 0;
}