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
		case CUC_GOTO:
		{
			// Top of the stack contains a flag to which the pointer should jump to
			// Set the pointer to the retrieved flag
			i = m_Stack.Pop();
			break;
		}

		case CUC_STACK_LOAD:
		{
			// Load a value from the address table through its "address"(stored on top of the stack)
			// into the stack.
			m_Stack.Push(m_AddressTable[m_Stack.Pop()]);
			break;
		}

		case CUC_STACK_STORE:
		{
			// Store a value from the top of the stack onto a value found in the symbol table through 
			// its address, taken from the value below the top of the stack i.e.
			//   TOP  -> value_to_store
			//           address 
			//           ...
			// BOTTOM -> ...
			Byte_t value = m_Stack.Pop();
			Address_t addr = m_Stack.Pop();
			m_AddressTable[addr] = value;
			break;
		}

		case CUC_LITERAL_NUMBER:
		{
			int value = bytecode[++i];
			m_Stack.Push(value);
			break;
		}

		case CUC_BYTE:
		{
			// Adds a symbol onto the table


			break;
		}

		case CUC_MULTIPLY:
		{
			break;
		}

		case CUC_ADD:
		{
			break;
		}

		case CUC_SUBSTRACT:
		{
			int op1 = m_Stack.Pop(),
				op2 = m_Stack.Pop();
			break;
		}

		case BOO_PLANT_BOMB:
		{
			boom::WorldInterface::PlantBomb();
			break;
		}

		case BOO_GO_TO:
		{
			int target_x = m_Stack.Pop();
			int target_y = m_Stack.Pop();
			boom::WorldInterface::GoTo(target_x, target_y);
			break;
		}

		} // switch
	}
}