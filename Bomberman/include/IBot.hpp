#ifndef BOOM_BOT_HPP
#define BOOM_BOT_HPP

#include "sybil/ai/IBot.hpp"
#include <string>


namespace boom
{
	/// Derived bots must call IBot's constructor with some desired name:
	/// class MyCoolTerminator : public IBot
	/// {
	///		MyCoolTerminator(std::string name) : IBot(name) { }
	///     MyCoolTerminator() : IBot("mycoolname") { } 
	///     MyCoolTerminator(someargs) : IBot("string name") { } 
	/// }
	class IBot : public syb::IBot
	{
	public:
		//IBot(std::string name) : m_Name(name) { }
		IBot(std::string name) : syb::IBot(name) { }
		~IBot() { }

		virtual void Update() = 0;

	protected:
		// Bomberman requires that each bot has some name.
		// Currently all games require it.
		//std::string m_Name;
	};
} // namespace boom

#endif // BOOM_BOT_HPP