#include "VirtualMachine.hpp"


CVM::CVM()
{ }

void CVM::Execute(std::string& bytecode)
{
	for (size_t i = 0; i < bytecode.size(); ++i)
	{
		char instruction = bytecode[i];
		switch (instruction)
		{
		case PLANT_BOMB:
		{
			boom::WorldInterface::PlantBomb();
			break;
		}

		case GO_TO:
		{
			int target_x = m_Stack.Pop();
			int target_y = m_Stack.Pop();
			boom::WorldInterface::GoTo(target_x, target_y);
			break;
		}

		case NUMBER_LITERAL:
		{
			int value = bytecode[++i];
			m_Stack.Push(value);
			break;
		}
		} // switch
	}
}