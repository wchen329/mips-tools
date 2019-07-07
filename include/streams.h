#ifndef __STREAMS_H__
#define __STREAMS_H__

#include <cstdint>
#include <string>

namespace priscas_io
{

	/* A "text_stream"
	 * Or, another writable ("file") for the output of the simulator
	 * Derived versions of "text_stream" can be set to receive output and error streams of 
	 * the Shell
	 */
	class text_stream
	{
		public:
			virtual void operator<<(std::string& input) = 0;
			virtual void operator>>(std::string& output) = 0;
			virtual bool isNull() { return false; }
	};

	/* A text stream that does nothing
	 * -
	 */
	class null_text_stream : public text_stream
	{
		public:
			void operator<<(std::string& input) { }
			void operator>>(std::string& output) { }
			bool isNull() { return true; }
	};

	extern null_text_stream null_tstream;
	extern const std::string newLine;

	/* Utilities for printing a number
	 *
	 *
	 */
	class StrTypes
	{
		public:
			static std::string IntToStr(int in);
			static std::string Int32ToStr(long in);
			static std::string UIntToStr(unsigned in);
			static std::string FloatToStr(float in);
			static std::string SizeToStr(size_t in);
			static std::string UInt32ToStr(unsigned long in);
			static std::string UInt64ToStr(unsigned long long in);
	};
}

#endif