#include "..\ZorkLib\ZorkLib.hpp"
#include "..\ZorkLib\ZorkLibWindow.hpp"
#include "..\ZorkLib\ZorkLibUSB.hpp"

#include <fstream>
#include <iostream>


using namespace std;


void Render(ZorkLib::SimpleRenderer& sr) {


	sr.FillRectangle(ZorkLib::Rectangle(0, 0, 2560, 1440), ZorkLib::ColorHSV(ZorkLib::ColorEnum::Cyan));

}

void OnKey(UINT32 key, bool down) {
	if (down) return;
}

int main() {

	ZorkLib::Window w(ZorkLib::Rectangle::RectMainScreen().Moved(2560.f, 0.f), L"debugging test");

	w.SetSimpleRenderFunction(Render);
	w.SetKeyEventFunction(OnKey);
	w.WindowLoop();

}