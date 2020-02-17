#include "..\ZorkLib\ZorkLib.hpp"
#include "..\ZorkLib\ZorkLibWindow.hpp"
#include "..\ZorkLib\ZorkLibUSB.hpp"

#include <fstream>
#include <iostream>


using namespace std;


int main() {

	ZorkLib::Window w(L"yo");

	w.LoadBitmap(L"hans");
	w.WindowLoop();

}