#ifndef SYB_DEF_HPP
#define SYB_DEF_HPP


namespace syb
{

// ----------------------------------------------------------------------------
// Alternative to redundant guards
// ----------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma once
#endif

// ----------------------------------------------------------------------------	
// Symbol exporting - currently ignored due to template in .dll issues 
// ----------------------------------------------------------------------------
//#if defined (WIN32) || defined (__CYGWIN__) && defined(NOT_DLL)
//	#ifdef SYBLIB_EXPORTS
//		#ifdef _MSC_VER
//			#define SYB_API __declspec(dllexport)
//		#elif defined __GNUC__
//			#define SYB_API __attribute__((dllexport))
//		#endif // Compiler specific flags 
//	#else	
//		#ifdef _MSC_VER
//			#define SYB_API __declspec(dllimport)
//		#elif defined __GNUC__
//			#define SYB_API __attribute__((dllimport))
//		#endif // Compiler specific flags 
//	#endif // SYBLIB_EXPORTS 
//#else
//	#define SYB_API 
//#endif // WIN32
#define SYB_API

// ----------------------------------------------------------------------------
// Linker flags(Visual Studio specific)
// ----------------------------------------------------------------------------
#if defined (WIN32) && defined (_MSC_VER)

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#endif

// ----------------------------------------------------------------------------
// Ignore warnings that don't help
//
// 4251: Some symbols are not meant to be exported
// 4800: Allow cast from int to bool for more compact code 
// ----------------------------------------------------------------------------
#ifdef _MSC_VER

#pragma warning(disable : 4251)
#pragma warning(disable : 4800)

#endif

// ----------------------------------------------------------------------------
// Default remote connection parameters
// ----------------------------------------------------------------------------
//const char* const SOCK_DEF_ADDRESS = "95.85.45.192";
const char* const SOCK_DEF_ADDRESS = "sybilai.com";
const char* const SOCK_DEF_PORT = "8124";
const char* const SOCK_DEF_CONNFLAG = "terminator";
const int SOCK_BUF_LEN = 16384;

// ----------------------------------------------------------------------------
// NAVIGATION DIRECTIONS
// ----------------------------------------------------------------------------
enum Direction
{
	NONE,
	UP,
	RIGHT,
	DOWN,
	LEFT
};

} // namespace syb

#endif // SYB_DEF_HPP