#include <strstream>
#include "streams.h"

namespace priscas_io
{
	null_text_stream null_tstream;
	const std::string newLine = "\n";

	template<class InputC> std::string GeneralConvToStr(InputC in)
	{
		std::string ret;

		std::strstream conv;
		conv << in;

		while(!conv.eof())
		{
			ret += conv.get();
		}

		if(ret.length() > 0)
		{
			ret.pop_back();
		}

		return ret;
	}

	std::string StrTypes::IntToStr(int in)
	{
		return GeneralConvToStr<int>(in);	
	}

	std::string StrTypes::Int32ToStr(long in)
	{
		return GeneralConvToStr<long>(in);
	}
	std::string StrTypes::UIntToStr(unsigned in)
	{
		return GeneralConvToStr<unsigned>(in);
	}

	std::string StrTypes::UInt32ToStr(unsigned long in)
	{
		return GeneralConvToStr<unsigned long>(in);
	}

	std::string StrTypes::UInt64ToStr(unsigned long long in)
	{
		return GeneralConvToStr<unsigned long long>(in);
	}

	std::string StrTypes::SizeToStr(size_t in)
	{
		return GeneralConvToStr<size_t>(in);
	}

	std::string StrTypes::FloatToStr(float in)
	{
		return GeneralConvToStr<float>(in);
	}

}