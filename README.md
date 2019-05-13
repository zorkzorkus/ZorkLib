# ZorkLib
C++ Wrapper Library for WinAPI, Direct2D, GUIs and often used functionality. Quick and easy to use.  
This library is work in progress and will often change existing parts. Developed using Visual Studio 2017.  
Main development is done privately on TFS. Occasionally a new version will be ported here.

#### Features
* ZorkLibWindow 
	* Create a Window that provides a KeyDown, MouseDown framework with simplified Direct2D rendering. Allows access to underlying functionality.
* ZorkLibUSB
	* Easier access to USB devices using **libusb**. Supports opening device via VID, PID, Serial Number. Writing and reading data through control and bulk transactions.
* ZorkUtility
	* Utility functions such as getting files from a directory, or string manipulation.

#### Future Plans
* GUI elements such as buttons, drop-down lists, auto scaling plots.
* Sound using XAudio2.
* Networking using boost::asio.
* Anything that is frequently used and has to be programmed again or copied over.
* Partly port to linux.

#### Limitations
* Because of the direct usage of WinAPI and DirectX, this library only works on Windows. Ports to other platforms is unlikely.
* Code is unfinished and often a mess ;)
* Much functionality is still missing.
* Some things may be designed poorly and will be altered when it becomes a problem.

#### Example

	#include <ZorkLib.hpp>
	#include <ZorkLibWindow.hpp>
	
	void OnMouseEvent(UINT32 key, INT32 x, INT32 y, bool down) {
		// Triggered on all mouse buttons presses / releases. (LBUTTON, RBUTTON, MBUTTON, X1, X2)
	}
	
	void OnKeyEvent(UINT32 key, bool down) {
		// Triggered when a keyboard button was pressed or released.
	}
	
	void Render(ZorkLib::SimpleRenderer& sr) {
		sr.Fill(ZorkLib::ColorEnum::White);
	}
	
	int main() {
    
		ZorkLib::Window window(L"Example");
		window.SetSimpleRenderFunction(Render);
		window.SetKeyEventFunction(OnKeyEvent);
		window.SetMouseEventFunction(OnMouseEvent);
        
        // This function will run and call framework methods until exit condition is met. (Here: When escape key is pressed)
		window.WindowLoop([] {
			return ZorkLib::IsKeyDown(VK_ESCAPE);
		});
        
	}
