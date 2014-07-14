#include "games/bomberman/BombGame.hpp"
#include "ai/IBot.hpp"
#include "ai/AStarSearch.hpp"

using namespace syb;


class Terminator : public IBot
{
public:
	Terminator() : IBot("Mergla7,12") { }

	void Update()
	{
		static bool t = true;
		if (t)
		{
			if (m_pPos)
			{
				std::string path = JsonPath(Search(*m_pPos, Vec2(7,12)));
				Send("move", &path);
			}
			t = false;
		}
		if (m_pWorld->m_Map[(int)m_pPos->x + 1][(int)m_pPos->y + 1].m_Blocks[0] == m_pWorld->BL_BOT)
			Send("bomb");
	}
};


int main()
{
	IBot *myTerminator = new Terminator();
	BombGame game(myTerminator);
	game.Run();
}