#include "Bot.h"
using namespace boom;


Bot::Bot(std::string name) : 
	IBot(name) 
{ }

// -------------------------------
// This will be called every frame
// -------------------------------
// Possible commands to authority:
// WorldInterface::Stop()
// WorldInterface::GoTo(x, y)
// WorldInterface::PlantBomb()
// WorldInterface::GetToSafety()
// WorldInterface::IsTileSafe(x, y)
// WorldInterface::GetClosestPlayer()
// WorldInterface::IsCurrentPathSafe()
// -------------------------------

// ----------------------------------------------------------------------
// Using a "behavior tree" - i.e. if-else-if-else
// Alternatively, use the provided StateMachine and implement your own
// states and transitions(part of the states themselves).
// ----------------------------------------------------------------------
void Bot::Update()
{
	
}
