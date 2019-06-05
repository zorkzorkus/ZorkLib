#pragma once

#define UNICODE

#include "ZorkLibUtility.hpp"
#include "ZorkLibWindowObjects.hpp"

#include <string>
#include <functional>

namespace ZorkLib {

	// ----------------
	// global functions
	// ----------------

	bool IsKeyDown(UINT32 key);
	Bitmap LoadBitmap(std::wstring filePath, DirectXAccess* pDirectX);

	// --------------------
	// class SimpleRenderer
	// --------------------

	class SimpleRenderer {

	public:

		SimpleRenderer(DirectXAccess& rDirectX);

		ID2D1SolidColorBrush* ToColorBrush(Color color);

		void DrawRectangle(Rectangle rect, Color color, float angle = 0.f, float strokeWidth = 1.f);
		void DrawRectangleOuter(Rectangle rect, Color color, float angle = 0.f, float strokeWidth = 1.f);
		void DrawRectangleInner(Rectangle rect, Color color, float angle = 0.f, float strokeWidth = 1.f);
		void DrawLine(Line line, Color c, float strokeWidth = 1.f);
		void DrawEllipse(Ellipse e, Color c, float angle = 0.f, float strokeWidth = 1.f);

		void FillRectangle(Rectangle rect, Color color, float angle = 0.f);
		void FillEllipse(Ellipse e, Color c, float angle = 0.f);
		void Fill(Color c);

		void RenderBitmap(Bitmap bmp, Rectangle rect, float angle = 0.f, float opacity = 1.f);
		void RenderText(std::wstring text, Rectangle r, UINT32 size, Color c, float angle = 0.f, TextAlignmentEnum tae = TextAlignmentEnum::Center);
		void RenderTextClipping(std::wstring text, Rectangle r, UINT32 size, Color c, float angle = 0.f, TextAlignmentEnum tae = TextAlignmentEnum::Center);

	private:

		DirectXAccess& m_DirectX;

	};

	// ------------
	// class Window
	// ------------

	class Window {

	public:

		Window(std::wstring windowName);
		Window(Rectangle windowRect, std::wstring windowName);
		Window(Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight, std::wstring windowName);
		Window(Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight, std::wstring windowName, DWORD wndFlags, DWORD wndExFlags);

		Window(const Window&) = delete;
		Window(const Window*) = delete;
		Window(const Window&&) = delete;
		Window& operator=(const Window&) = delete;

		void HandleMessages();
		void Render();
		void WindowLoop(std::function<bool(void)> ExitCondition);
		void WindowLoop(std::function<bool(void)> ExitCondition, std::function<void(void)> Callback);
		void SetSimpleRenderFunction(std::function<void(SimpleRenderer&)> simpleRender);
		void SetDirectRenderFunction(std::function<void(DirectXAccess&)> directRender);
		void SetMouseEventFunction(std::function<void(UINT32, INT32, INT32, bool)> mouseEventFunction);
		void SetKeyEventFunction(std::function<void(UINT32, bool)> keyEventFunction);
		Bitmap LoadBitmap(std::wstring fileName);
		void SaveContent(std::wstring fileName);
		Point GetCursorPos();
		DirectXAccess* GetDirectXAccess();

	private:

		UINT32 m_RenderWidth;
		UINT32 m_RenderHeight;
		UINT32 m_WindowWidth;
		UINT32 m_WindowHeight;

		bool m_Vsync;

		DirectXAccess m_DirectX;
		SimpleRenderer m_SimpleRenderer;
		std::function<void(SimpleRenderer&)> m_SimpleRenderFunction;
		std::function<void(DirectXAccess&)> m_DirectRenderFunction;
		std::function<void(UINT32, INT32, INT32, bool)> m_MouseEventFunction;
		std::function<void(UINT32, bool)> m_KeyEventFunction;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT32 msg, WPARAM w, LPARAM l);
		static LRESULT CALLBACK StaticWindowProcedure(HWND hWnd, UINT32 msg, WPARAM w, LPARAM l);

	};

}