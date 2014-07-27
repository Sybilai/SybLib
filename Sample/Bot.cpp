#include "Bot.h"
using namespace boom;


Bot::Bot(std::string name) : 
	IBot(name) 
{ }

// -------------------------------
// This will be called every frame
// -------------------------------
// Possible commands to authority:
// WorldInterface::PlantBomb();
// WorldInterface::GoTo(x, y);
// WorldInterface::Stop()
// WorldInterface::GetClosestPlayer()
// WorldInterface::IsCurrentPathSafe()
// -------------------------------
void Bot::Update()
{
	if (bombs_within_range.size() && !current_path.size())
	{
		// Stationary, bomb is about to go boom
		// Most urgent matter: gtfo
		/*if (world[x - 1][y] == WALKABLE)
			WorldInterface::GoTo(x - 1, y);
		else if (world[x + 1][y] == WALKABLE)
			WorldInterface::GoTo(x + 1, y);
		else if (world[x][y - 1] == WALKABLE)
			WorldInterface::GoTo(x, y - 1);
		else if (world[x][y + 1] == WALKABLE)
			WorldInterface::GoTo(x, y + 1);*/
		WorldInterface::GetToSafety();
		//else well, f***
	}
	else if (current_path.size())
	{
		// Currently moving
		if (!WorldInterface::IsCurrentPathSafe())
			// Path is tainted
			WorldInterface::Stop();

		if (players_within_range.size())
			WorldInterface::PlantBomb();
	}
	else if (players.size())
	{
		if (players_within_range.size())
			WorldInterface::PlantBomb();
		else if (players.size())
		{
			int closest_enemy = WorldInterface::GetClosestPlayer();
			WorldInterface::GoTo(players[closest_enemy].x, players[closest_enemy].y);
		}
	}
}