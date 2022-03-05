#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include "ZorkLibUtility.hpp"
#include "ZorkLibWindowObjects.hpp"
#include "ZorkLibException.hpp"

#include <string>
#include <functional>
#include <codecvt>
#include <chrono>

namespace ZorkLib {

	// ----------------
	// global functions
	// ----------------

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

		void TransformAdd(D2D_MATRIX_3X2_F transform);
		void TransformSet(D2D_MATRIX_3X2_F transform);
		D2D_MATRIX_3X2_F TransformGet();
		void TransformReset();

		Point GetRenderSize();

	private:

		DirectXAccess& m_DirectX;

	};

	// ------------
	// class Window
	// ------------

	class Window {

	public:

		// Constructors
		Window(std::wstring windowName);
		Window(Rectangle windowRect, std::wstring windowName);
		Window(Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight, std::wstring windowName);
		Window(Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight, std::wstring windowName, DWORD wndFlags, DWORD wndExFlags);

		// Destructor
		~Window();

		// Copy Constructors
		Window(const Window&) = delete;
		Window(const Window*) = delete;

		// Move Constructors
		Window(const Window&&) = delete;

		// Assignment Operators
		Window& operator=(const Window&) = delete;
		Window& operator=(const Window&& rval) = delete;

		// Alternative Constructors
		static std::shared_ptr<Window> TransparentWindow(std::wstring windowName);
		static std::shared_ptr<Window> TransparentWindow(std::wstring windowName, Rectangle windowRect);
		static std::shared_ptr<Window> TransparentWindow(std::wstring windowName, Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight);

		void HandleMessages();
		void Render();
		void WindowLoop();
		void WindowLoop(std::function<void(float)> Callback);
		void WindowLoop(std::function<void(std::chrono::steady_clock::duration)> Callback);
		void WindowLoop(std::function<bool(void)> ExitCondition);
		void WindowLoop(std::function<bool(void)> ExitCondition, std::function<void(float)> Callback);
		void WindowLoop(std::function<bool(void)> ExitCondition, std::function<void(std::chrono::steady_clock::duration)> Callback);
		void SetSimpleRenderFunction(std::function<void(SimpleRenderer&)> simpleRender);
		void SetDirectRenderFunction(std::function<void(DirectXAccess&)> directRender);
		void SetMouseEventFunction(std::function<void(UINT32, INT32, INT32, bool)> mouseEventFunction);
		void SetMouseWheelFunction(std::function<void(INT32)> mouseWheelFunction);
		void SetKeyEventFunction(std::function<void(UINT32, bool)> keyEventFunction);
		void SetWindowProcedure(std::function<std::pair<bool, LRESULT>(HWND, UINT32, WPARAM, LPARAM)> windowProc);
		void SetVsync(bool vsync);
		bool GetVsync();
		void SetCatchExceptions(bool use);
		bool GetCatchExceptions();
		Bitmap LoadBitmap(std::wstring fileName);
		void SaveContent(std::wstring fileName);
		Point GetCursorPos();
		Rectangle GetWindowRect();
		Point GetRenderSize();
		DirectXAccess* GetDirectXAccess();

	private:

		void DisplayHresultErrorMessageBox(HRESULT hr, std::wstring msg);

		LARGE_INTEGER m_TimerFreq;
		LARGE_INTEGER m_TimerNow, m_TimerPrev;
		std::chrono::steady_clock::time_point m_ChronoPrev, m_ChronoNow;

		UINT32 m_RenderWidth;
		UINT32 m_RenderHeight;
		UINT32 m_WindowWidth;
		UINT32 m_WindowHeight;

		bool m_Vsync;
		bool m_CatchExceptions;

		DirectXAccess m_DirectX;
		SimpleRenderer m_SimpleRenderer;
		std::function<void(SimpleRenderer&)> m_SimpleRenderFunction;
		std::function<void(DirectXAccess&)> m_DirectRenderFunction;
		std::function<void(UINT32, INT32, INT32, bool)> m_MouseEventFunction;
		std::function<void(INT32)> m_MouseWheelFunction;
		std::function<void(UINT32, bool)> m_KeyEventFunction;
		std::function<std::pair<bool, LRESULT>(HWND, UINT32, WPARAM, LPARAM)> m_WindowProcedure;

		LRESULT CALLBACK WndProc(HWND hWnd, UINT32 msg, WPARAM w, LPARAM l);
		static LRESULT CALLBACK StaticWindowProcedure(HWND hWnd, UINT32 msg, WPARAM w, LPARAM l);
		
	};

}
