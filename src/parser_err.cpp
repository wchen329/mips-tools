#include <cstdio>
#include <cstdarg>
#include "parser_err.h"
#include "messages.h"

namespace mipsshell
{
	void parser_err::err_pr()
	{
		fprintf(stderr, "An error has occurred.\n");
	}

	void unexpected_err::err_pr()
	{
		fprintf(stderr, "\"%s\" was unexpected at this time. (line %d)\n", unexpected, line);
	}

	void badformat_err::err_pr()
	{
		fprintf(stderr, BAD_FORMAT);
	}

	void badimm_err::err_pr()
	{
		fprintf(stderr, "Immediate format is incorrect.\n");
	}

	void expected_err::err_pr()
	{
		fprintf(stderr, "Command expected more arguments.\n");
	}
}