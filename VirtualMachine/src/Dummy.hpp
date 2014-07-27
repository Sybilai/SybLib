#ifndef DUMMY_HPP
#define DUMMY_HPP
#include "IBot.hpp"
#include <string>


// The game interface will store data in here, which my be then requested by the vm when executing the bot.
class Dummy : public boom::IBot
{
public:
	Dummy(const std::string& name) : boom::IBot(name) { }
	
	void Update()
	{
		int i = 5;
	}
};

#endif // DUMMY_HPP