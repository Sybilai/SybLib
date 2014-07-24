#ifndef SYB_WORLDINTERFACE_HPP
#define SYB_WORLDINTERFACE_HPP


namespace syb
{
	/// Game-specific functions that act as a proxy between AIs and the world.
	class WorldInterface
	{
	public:
		WorldInterface() { }
		virtual ~WorldInterface() { }
	};
} // namespace syb

#endif // SYB_WORLDINTERFACE_HPP