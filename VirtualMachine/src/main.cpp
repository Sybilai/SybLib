#include "CucuLang/VirtualMachine.hpp"
#include "translator/Translator.hpp"
#include "UserInterface.hpp"
#include "Bomberman.hpp"
#include "Dummy.hpp"
#include <thread>


void RunThreadedGame(BombermanGame* game)
{
	game->Run();
}


int main(int argc, char *argv[])
{
	// Get connection paramters and bot files(also validate bot files).
	if (argc == 4)
	{
		// Avoid the CUI. Go straight into action. Assumes Dummy was implemented and the VM is not going to be used.
		// argv:
		// [0] = address
		// [1] = port
		// [2] = token
		// [3] = name
		UserInterface::s_Address = argv[0];
		UserInterface::s_Port = argv[1];
		UserInterface::s_BotToken = argv[2];
		UserInterface::s_BotName = argv[3];
	}
	else
		UserInterface::Launch();

	// Heat up the game.
	BombermanGame game(UserInterface::s_Address, UserInterface::s_Port, UserInterface::s_BotToken);
	Dummy bot(UserInterface::s_BotName);
	game.RegisterBot(&bot);

	bool session = false;
	// Launch into infinity
	if (argc == 4)
	{
		session = true;
		game.Run();
	}
	//else

	if (session)
		return 0;
	
	// Prepare the VM.
	CVM vm;
	std::string bytecode = Translator::Convert();

	// Give the game its only little playpen.
	std::thread game_thread(RunThreadedGame, &game);

	// Start up the VM.
	vm.Execute(bytecode);

	game_thread.join();
}