#ifndef SYB_ENGINE_RULES_HPP
#define SYB_ENGINE_RULES_HPP


namespace syb
{
	/// Specific rulesets: e.g. GameRules(es) should have accessors(if they need any) inlined
	class EngineRules
	{
		friend class Game;
	public:
		EngineRules();
		virtual ~EngineRules();

		const unsigned int& StartFrame() const;
		/// Constant frames per second
		const unsigned int& UpdateRate() const;

	protected:
		bool m_bReceived;

		unsigned int m_StartFrame;
		unsigned int m_UpdateRate;
	};

	inline EngineRules::EngineRules() :
		m_bReceived(false),
		m_StartFrame(0), 
		m_UpdateRate(0)
	{ }
	
	inline EngineRules::~EngineRules()
	{ }

	inline const unsigned int& EngineRules::StartFrame() const
	{
		return m_StartFrame;
	}

	inline const unsigned int& EngineRules::UpdateRate() const
	{
		return m_UpdateRate;
	}
} // namespace syb

#endif // SYB_ENGINE_RULES_HPP