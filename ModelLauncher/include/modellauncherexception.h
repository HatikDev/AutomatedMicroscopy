#include <stdexcept>
#include <string>

class ModelLauncherException : public std::exception
{
public:
	ModelLauncherException(const std::string& mesg);
	const char* what() const override;

private:
	std::string m_mesg;
};
