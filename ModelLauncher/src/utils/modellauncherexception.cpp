#include "modellauncherexception.h"

ModelLauncherException::ModelLauncherException(const std::string& mesg)
	: m_mesg{ mesg }
{
}

const char* ModelLauncherException::what() const /*override*/
{
	return m_mesg.c_str();
}
