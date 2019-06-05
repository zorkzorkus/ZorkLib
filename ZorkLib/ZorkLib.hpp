#pragma once

/*
Header for binding the correct library
*/

#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "ZorkLibDebug64.lib")
	#else
		#pragma comment(lib, "ZorkLibDebug32.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "ZorkLib64.lib")
	#else
		#pragma comment(lib, "ZorkLib32.lib")
	#endif
#endif

