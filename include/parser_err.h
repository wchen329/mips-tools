#ifndef __PARSER_ERR_H__
#define __PARSER_ERR_H__ 
namespace mipsshell
{
	// exception class
	class parser_err
	{
		public:
			virtual void err_pr() = 0; // print the error
	};

	class badformat_err : public parser_err
	{
		public:
			void err_pr(); // print the error
	};

	class badimm_err : public parser_err
	{
		public:
			void err_pr();
	};

	class expected_err : public parser_err
	{
		public:
			void err_pr();
	};

	class unexpected_err : public parser_err
	{
		public:
			void err_pr();
			unexpected_err(const char * expected, const char * unexpected) : expected(expected), unexpected(unexpected) {};
		private:
			const char * unexpected;	// unexpected sequence
			const char * expected;
	};
}

#endif