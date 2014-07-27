#ifndef CUC_STACK_HPP
#define CUC_STACK_HPP


class StackMachine
{
public:
	StackMachine();
	void Push(int value);
	int Pop();

	static const int MAX_STACK = 128;
	int m_StackSize;
	int m_Stack[MAX_STACK];
};

#endif // CUC_STACK_HPP