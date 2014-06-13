#include <iostream>

#include "games/bomberman/BombGame.hpp"
#include "ai/IBot.hpp"
#include "ai/AStarSearch.hpp"

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
			if (m_pPos)
			{
				std::string myPath;
				Vec2 myPos = *m_pPos;
				Vec2 target(2.0, 2.0);
				Search(myPos, target, myPath);
				std::string path = "\"up\", \"up\"";
				Send("move", &path);
				a = 0;
			}
		}
	}
};


int main()
{
	IBot *myTerminator = new Terminator();
	BombGame game(myTerminator);
	return game.Run();
}