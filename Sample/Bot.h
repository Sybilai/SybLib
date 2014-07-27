#pragma once
#include "WorldInterface.hpp"
#include "IBot.hpp"
#include <string>


class Bot : public boom::IBot
{
public:
	Bot(std::string name);
	void Update();
};