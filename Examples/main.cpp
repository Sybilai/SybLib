#include <iostream>

#include "games/bomberman/BombGame.hpp"
#include "ai/IBot.hpp"

using namespace syb;

class Terminator : public IBot
{
public:
	Terminator() : IBot("Mitica") { }

	void Update()
	{
		static int a = 1;
		if (a)
		{
		//std::string path = "\"up\", \"up\", \"left\", \"left\", \"right\", \"down\", \"up\"";
			std::string path = "\"up\", \"up\"";
			Send("move", &path);
			a = 0;
		}
	}
};


int main()
{
	IBot *myTerminator = new Terminator();
	BombGame game(myTerminator);
	return game.Run();
}