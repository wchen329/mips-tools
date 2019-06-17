#ifndef __BRANDING_H__
#define __BRANDING_H__

/* Header that includes information about project name, description, and versioning, etc., etc.
 * wchen329
 */
namespace branding
{
	const std::string APPLICATION_NAME = std::string("PRISCAS");
	const std::string APPLICATION_DESC = std::string("[PRISCAS: PRISCAS Really Is Simply a Computer Architecture Simulator]");
	const std::string APPLICATION_VERSION = std::string("0.2");

	#ifdef P_RELEASE
		const std::string APPLICATION_DBG = "(release)";
	#else
		const std::string APPLICATION_DBG = "(developmental build)";
	#endif
}


#endif