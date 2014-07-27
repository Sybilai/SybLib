#include "StackMachine.hpp"
#include <cassert>


StackMachine::StackMachine() :
m_StackSize(0)
{ }

void StackMachine::Push(int value)
{
	assert(m_StackSize < MAX_STACK);
	m_Stack[m_StackSize + 1] = value;
}

int StackMachine::Pop()
{
	assert(m_StackSize > 0);
	return m_Stack[--m_StackSize];
}