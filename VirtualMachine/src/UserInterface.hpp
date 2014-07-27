#ifndef UI_HPP
#define UI_HPP
#include <string>


// Get connection data and bot files until everything is valid for connection, and interpretation, respectively.
class UserInterface
{
public:
	static void Launch();

	static std::string s_BotToken;
	static std::string s_BotName;
	static std::string s_Address;
	static std::string s_Port;
};

#endif // UI_HPP