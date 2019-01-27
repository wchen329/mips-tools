#ifndef __MT_EXCEPTION_H__
#define __MT_EXCEPTION_H__

/* MIPS Tools
 * Exception Class
 */


// Abstract exception class
class mt_exception
{
	public:
		int number();		// get exception number
		char * full_name();	// get full exception name

	protected:
		int except_num;
		char * qual_name;
};

// Register Out of Bounds Exception
class reg_oob_exception : mt_exception
{
	reg_oob_exception();
	~reg_oob_exception();
};

#endif