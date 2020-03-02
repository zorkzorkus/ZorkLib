#include "..\ZorkLib\ZorkLib.hpp"
#include "..\ZorkLib\ZorkLibWindow.hpp"
#include "..\ZorkLib\ZorkLibUSB.hpp"

#include <fstream>
#include <iostream>


using namespace std;

// 100% saturation, 100% value results in colors with one channel = 00, another = ff, last one is between 00 and ff
ZorkLib::ColorHSV c = ZorkLib::ColorHSV(0.f, 1.f, 1.f);

void Render(ZorkLib::SimpleRenderer& sr) {
	sr.Fill(c);
}

void Update(float elap) {
	c.AddHue(elap * 360.f / 12.f); // takes 12 seconds to complete one color wheel cycle
}

int main() {

	ZorkLib::Window w(L"debugging test");

	w.SetSimpleRenderFunction(Render);
	w.WindowLoop(Update);

}