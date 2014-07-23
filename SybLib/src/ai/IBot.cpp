#include "../../include/ai/IBot.hpp"


namespace syb
{
	IBot::IBot(std::string name) :
		m_Name(name)
	{ }

	IBot::~IBot()
	{
		// TODO: use a shared_ptr for m_pWorldInterface to avoid any snafu
	}

	void IBot::AttachWorldInterface(WorldInterface* pInterface)
	{
		pWorld = pInterface;
	}

	const std::string& IBot::Name() const
	{
		return m_Name;
	}

	void IBot::Identify(unsigned int id)
	{
		m_MyId = id;
	}
}