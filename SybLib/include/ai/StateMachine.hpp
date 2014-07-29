#ifndef SYB_STATE_MACHINE_HPP
#define SYB_STATE_MACHINE_HPP
#include <vector>
#include <memory>


namespace boom //syb - figure out how to avoid clashes
{
	class IState
	{
	public:
		virtual void Update() = 0;
	};

	class StateMachine
	{
	public:
		std::vector<std::shared_ptr<IState>> m_States;
		std::shared_ptr<IState> m_CurrentState;
	};
} // namespace syb

#endif // SYB_STATE_MACHINE_HPP