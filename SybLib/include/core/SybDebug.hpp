#ifndef SYB_DEBUG_HPP
#define SYB_DEBUG_HPP

#include <iostream>


namespace syb
{
	// defines aren't scoped but, #YOLO
#define OUT_DATASTREAM 1 

	template <typename T>
	static void TRACE(T msg)
	{
		std::cout << msg
			<< "\n===============================================\n";
	}
} // namespace syb

#endif // SYB_DEBUG_HPP