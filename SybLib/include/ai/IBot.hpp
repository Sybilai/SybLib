#ifndef SYB_IBOT_HPP
#define SYB_IBOT_HPP

#include <string>
// Still not sure about shared_ptr polymorphism
//#include <memory> 


namespace syb
{
	class IBot
	{
	public:
		IBot(std::string name);
		virtual ~IBot();
		const std::string& Name() const;

		//void AttachWorldInterface(WorldInterface*);
		void Identify(const unsigned int& id);

		virtual void Update() = 0;

	protected:
		// Disregard any conventions used hitherto in the entire library for convenience:
		// my_id instead of MyId() or m_MyId
		// 

		/// Currently, AIs can see the whole map and everything in it. This helps it identify itself.
		/// Also, should teams be introduced, this may be something like the team's id. 
		/// Again, helps identify friendlies.
		unsigned int my_id;

		//WorldInterface* pWorld;

	private:
		/// Not sure if should be stored. Or this should be stored instead of m_MyId and identify through this instead.
		/// Currently, I don't think the authority checks for name clashes, therefore, this makes me feel safe and snuggly.
		std::string m_Name;
	};
} // namespace syb

#endif // SYB_IBOT_HPP