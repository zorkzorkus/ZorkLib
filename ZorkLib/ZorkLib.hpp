#pragma once

/*
Header for binding the correct library
*/

#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "ZorkLib64d.lib")
	#else
		#pragma comment(lib, "ZorkLib32d.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "ZorkLib64.lib")
	#else
		#pragma comment(lib, "ZorkLib32.lib")
	#endif
#endif

#ifdef ZORKLIB_HEADER_USB
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "ZorkLibUSB64d.lib")
#else
#pragma comment(lib, "ZorkLibUSB32d.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "ZorkLibUSB64.lib")
#else
#pragma comment(lib, "ZorkLibUSB32.lib")
#endif
#endif
#endif