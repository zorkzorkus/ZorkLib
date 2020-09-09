#include "ZorkLibWindow.hpp"

namespace ZorkLib {

	// ----------------
	// global functions
	// ----------------

	Bitmap LoadBitmap(std::wstring filePath, DirectXAccess* pDirectX) {
		HRESULT hr = S_OK;
		IWICBitmapDecoder* pDecoder;
		IWICFormatConverter* pConverter;
		IWICBitmapFrameDecode* pSource;
		Bitmap bmp;

		hr = pDirectX->pIWICFactory->CreateDecoderFromFilename(filePath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
		Exception::AbortIfFailure(hr, L"Failed to open file:\n  " + filePath);
		hr = pDecoder->GetFrame(0, &pSource);
		Exception::AbortIfFailure(hr, L"Failure while loading bitmap.");
		hr = pDirectX->pIWICFactory->CreateFormatConverter(&pConverter);
		Exception::AbortIfFailure(hr, L"Failure while loading bitmap.");
		hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		Exception::AbortIfFailure(hr, L"Failure while loading bitmap.");
		hr = (pDirectX->pContext)->CreateBitmapFromWicBitmap(pConverter, &bmp);
		Exception::AbortIfFailure(hr, L"Failure while loading bitmap.");

		pDecoder->Release();
		pConverter->Release();
		pSource->Release();

		return bmp;
	}

	// --------------------
	// class SimpleRenderer
	// --------------------

	SimpleRenderer::SimpleRenderer(DirectXAccess& rDirectX) : m_DirectX(rDirectX) {}

	ID2D1SolidColorBrush* SimpleRenderer::ToColorBrush(Color color) {
		UINT32 colorHex = color.GetHex();
		if (m_DirectX.BrushMap.find(colorHex) == m_DirectX.BrushMap.end()) {
			m_DirectX.BrushMap[colorHex] = nullptr;
			m_DirectX.pContext->CreateSolidColorBrush(color, &m_DirectX.BrushMap[colorHex]);
		}
		return m_DirectX.BrushMap[colorHex];
	}

	void SimpleRenderer::DrawRectangle(Rectangle rect, Color color, float angle /* = 0.f */, float strokeWidth /* = 1.f */) {
		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, rect.Center()));
			m_DirectX.pContext->DrawRectangle(rect, ToColorBrush(color), strokeWidth);
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		} else {
			m_DirectX.pContext->DrawRectangle(rect, ToColorBrush(color), strokeWidth);
		}
	}

	void SimpleRenderer::DrawRectangleOuter(Rectangle rect, Color color, float angle /* = 0.f */, float strokeWidth /* = 1.f */) {
		rect.ShiftOut(strokeWidth * 0.5f, strokeWidth * 0.5f);
		m_DirectX.pContext->DrawRectangle(rect, ToColorBrush(color), strokeWidth);
	}

	void SimpleRenderer::DrawRectangleInner(Rectangle rect, Color color, float angle /* = 0.f */, float strokeWidth /* = 1.f */) {
		rect.ShiftIn(strokeWidth * 0.5f, strokeWidth * 0.5f);
		m_DirectX.pContext->DrawRectangle(rect, ToColorBrush(color), strokeWidth);
	}

	void SimpleRenderer::DrawLine(Line line, Color color, float strokeWidth /* = 1.f */) {
		m_DirectX.pContext->DrawLine(line.a, line.b, ToColorBrush(color), strokeWidth);
	}

	void SimpleRenderer::DrawEllipse(Ellipse ellipse, Color color, float angle /* = 0.f */, float strokeWidth /* = 1.f */) {
		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, ellipse.center));
			m_DirectX.pContext->DrawEllipse(ellipse, ToColorBrush(color), strokeWidth);
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		} else {
			m_DirectX.pContext->DrawEllipse(ellipse, ToColorBrush(color), strokeWidth);
		}
	}

	void SimpleRenderer::FillRectangle(Rectangle rect, Color color, float angle /* = 0.f */) {
		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, rect.Center()));
			m_DirectX.pContext->FillRectangle(rect, ToColorBrush(color));
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		} else {
			m_DirectX.pContext->FillRectangle(rect, ToColorBrush(color));
		}
	}

	void SimpleRenderer::FillEllipse(Ellipse e, Color c, float angle /* = 0.f */) {
		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, e.center));
			m_DirectX.pContext->FillEllipse(e, ToColorBrush(c));
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		} else {
			m_DirectX.pContext->FillEllipse(e, ToColorBrush(c));
		}
	}

	void SimpleRenderer::Fill(Color c) {
		m_DirectX.pContext->Clear(c);
	}

	void SimpleRenderer::RenderBitmap(Bitmap bmp, Rectangle rect, float angle /* = 0.f */, float opacity /* = 1.f */) {
		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, rect.Center()));
			m_DirectX.pContext->DrawBitmap(bmp, rect, opacity);
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		} else {
			m_DirectX.pContext->DrawBitmap(bmp, rect, opacity);
		}
	}

	void SimpleRenderer::RenderText(std::wstring text, Rectangle r, UINT32 size, Color c, float angle /* = 0.f */, TextAlignmentEnum tae /* = Center */) {
		if (size > gc_MaxFontSize) {
			throw std::exception("FontSize to big!");
		}

		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, r.Center()));
		}

		switch (tae) {
			case TextAlignmentEnum::TopLeft:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case TextAlignmentEnum::Top:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case TextAlignmentEnum::TopRight:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case TextAlignmentEnum::Left:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case TextAlignmentEnum::Center:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case TextAlignmentEnum::Right:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case TextAlignmentEnum::BottomLeft:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
			case TextAlignmentEnum::Bottom:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
			case TextAlignmentEnum::BottomRight:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
		}

		m_DirectX.pContext->DrawTextW(text.c_str(), text.size(), m_DirectX.pTextFormats[size], r, ToColorBrush(c));

		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		}

	}

	void SimpleRenderer::RenderTextClipping(std::wstring text, Rectangle r, UINT32 size, Color c, float angle /* = 0.f */, TextAlignmentEnum tae /* = Center */) {
		if (size > gc_MaxFontSize) {
			throw std::exception("FontSize to big!");
		}

		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, r.Center()));
		}

		switch (tae) {
			case TextAlignmentEnum::TopLeft:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case TextAlignmentEnum::Top:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case TextAlignmentEnum::TopRight:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				break;
			case TextAlignmentEnum::Left:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case TextAlignmentEnum::Center:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case TextAlignmentEnum::Right:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				break;
			case TextAlignmentEnum::BottomLeft:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
			case TextAlignmentEnum::Bottom:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
			case TextAlignmentEnum::BottomRight:
				m_DirectX.pTextFormats[size]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				m_DirectX.pTextFormats[size]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				break;
		}

		m_DirectX.pContext->DrawTextW(text.c_str(), text.size(), m_DirectX.pTextFormats[size], r, ToColorBrush(c), D2D1_DRAW_TEXT_OPTIONS_CLIP);

		if (angle != 0.f) {
			m_DirectX.pContext->SetTransform(D2D1::Matrix3x2F::Identity());
		}

	}

	// ------------
	// class Window
	// ------------

	Window::Window(std::wstring windowName) : Window(Rectangle::RectMainScreen(), windowName) {}

	Window::Window(Rectangle windowRect, std::wstring windowName) : Window(windowRect, static_cast<UINT32>(windowRect.Width()), static_cast<UINT32>(windowRect.Height()), windowName) {}

	Window::Window(Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight, std::wstring windowName) : Window(windowRect, renderWidth, renderHeight, windowName, WS_POPUP, NULL) {}

	Window::Window(Rectangle windowRect, UINT32 renderWidth, UINT32 renderHeight, std::wstring windowName, DWORD wndFlags, DWORD wndExFlags) : m_SimpleRenderer(m_DirectX), m_DirectRenderFunction(nullptr), m_KeyEventFunction(nullptr), m_MouseEventFunction(nullptr), m_SimpleRenderFunction(nullptr), m_Vsync(false), m_CatchExceptions(true) {

		try {

			HRESULT hr = S_OK;
			CoInitializeEx(0, 0);
			QueryPerformanceFrequency(&m_TimerFreq);

			if (m_MouseEventFunction) {
				m_MouseEventFunction = 0;
			}

			// Window
			WNDCLASSEX w = {sizeof(WNDCLASSEX)};
			w.lpfnWndProc = Window::StaticWindowProcedure;
			w.style = NULL;
			w.cbClsExtra = 0;
			w.cbWndExtra = sizeof(LONG_PTR);
			w.hInstance = NULL;
			w.hbrBackground = NULL;
			w.lpszMenuName = NULL;
			w.hCursor = LoadCursor(NULL, IDI_APPLICATION);
			w.lpszClassName = windowName.c_str();

			RegisterClassEx(&w);

			INT32 windowX = static_cast<INT32>(windowRect.left);
			INT32 windowY = static_cast<INT32>(windowRect.top);
			INT32 windowWidth = static_cast<INT32>(windowRect.Width());
			INT32 windowHeight = static_cast<INT32>(windowRect.Height());
			m_DirectX.hWnd = CreateWindowEx(wndExFlags, windowName.c_str(), windowName.c_str(), wndFlags, windowX, windowY, windowWidth, windowHeight, 0, 0, 0, (LPVOID) this);

			Exception::AbortIfFailure(static_cast<bool>(m_DirectX.hWnd), L"Failed to create Window.");

			// DirectX
			DXGI_MODE_DESC mode;
			mode.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			mode.Width = renderWidth;
			mode.Height = renderHeight;
			mode.RefreshRate.Denominator = 1;
			mode.RefreshRate.Numerator = 0;
			mode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			mode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;

			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
			swapChainDesc.BufferCount = 2;
			swapChainDesc.BufferDesc = mode;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.Flags = NULL;
			swapChainDesc.OutputWindow = m_DirectX.hWnd;
			swapChainDesc.SampleDesc.Count = 8;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
			swapChainDesc.Windowed = true;

			D3D_FEATURE_LEVEL featureLevel[] = {
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};

			hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED, featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_DirectX.pSwapChain, &m_DirectX.pD3Device, 0, &m_DirectX.pD3DeviceContext);
			Exception::AbortIfFailure(hr, L"Failed to create Window and SwapChain.");
			hr = m_DirectX.pD3Device->QueryInterface(IID_PPV_ARGS(&m_DirectX.pDXGIDevice));
			Exception::AbortIfFailure(hr, L"Failed to create DXGIDevice.");
			hr = m_DirectX.pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_DirectX.pBackBuffer));
			Exception::AbortIfFailure(hr, L"Failed to create BackBuffer.");

			D2D1_FACTORY_OPTIONS d2FactoryOptions;
#ifdef _DEBUG 
			d2FactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
			d2FactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_DirectX.pD2Factory);
			Exception::AbortIfFailure(hr, L"Failed to create D2Factory.");
			hr = m_DirectX.pD2Factory->CreateDevice(m_DirectX.pDXGIDevice, &m_DirectX.pDevice);
			Exception::AbortIfFailure(hr, L"Failed to create D2Device.");
			hr = m_DirectX.pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_DirectX.pContext);
			Exception::AbortIfFailure(hr, L"Failed to create RenderContext.");
			m_DirectX.pContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
			D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
			hr = m_DirectX.pContext->CreateBitmapFromDxgiSurface(m_DirectX.pBackBuffer, bitmapProperties, &m_DirectX.pTargetBitmap);
			Exception::AbortIfFailure(hr, L"Failed to create TargetBitmap.");
			m_DirectX.pContext->SetTarget(m_DirectX.pTargetBitmap);

			hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_DirectX.pIWICFactory));
			Exception::AbortIfFailure(hr, L"Failed to create IWICFactory.");

			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_DirectX.pWriteFactory));
			Exception::AbortIfFailure(hr, L"Failed to create WriteFactory.");

			for (UINT32 i = 0; i <= gc_MaxFontSize; ++i) {
				if (i == 0) {
					m_DirectX.pTextFormats.push_back(nullptr);
					continue;
				}
				IDWriteTextFormat3* pFormat;
				hr = m_DirectX.pWriteFactory->CreateTextFormat(L"Consolas", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, static_cast<FLOAT>(i), L"de-DE", (IDWriteTextFormat**) (&pFormat));
				Exception::AbortIfFailure(hr, L"Failed to create TextFormat.");
				m_DirectX.pTextFormats.push_back(pFormat);
			}

			ShowWindow(m_DirectX.hWnd, SW_SHOW);

		} catch (Exception::HResult & ex) {
			if (m_CatchExceptions) {
				DisplayHresultErrorMessageBox(ex.hr, ex.Message);
			} else {
				throw ex;
			}
		} catch (Exception::Exception & ex) {
			if (m_CatchExceptions) {
				MessageBox(NULL, ex.Message.c_str(), L"Error", MB_OK | MB_ICONERROR);
			} else {
				throw ex;
			}
		}

	}

	void Window::HandleMessages() {
		MSG msg;
		if (PeekMessage(&msg, m_DirectX.hWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Render() {
		m_DirectX.pContext->BeginDraw();
		if (m_SimpleRenderFunction) m_SimpleRenderFunction(m_SimpleRenderer);
		if (m_DirectRenderFunction) m_DirectRenderFunction(m_DirectX);
		HRESULT hr = m_DirectX.pContext->EndDraw();
		Exception::AbortIfFailure(hr, L"Failure when drawing.");
		hr = (m_Vsync) ? m_DirectX.pSwapChain->Present(1, 0) : m_DirectX.pSwapChain->Present(0, 0);
		Exception::AbortIfFailure(hr, L"Failed when rendering.");
	}

	void Window::WindowLoop() {
		WindowLoop([] {return ZorkLib::IsKeyDown(VK_ESCAPE); });
	}

	void Window::WindowLoop(std::function<void(float)> Callback) {
		WindowLoop([] {return ZorkLib::IsKeyDown(VK_ESCAPE); }, Callback);
	}

	void Window::WindowLoop(std::function<void(std::chrono::steady_clock::duration)> Callback) {
		WindowLoop([] {return ZorkLib::IsKeyDown(VK_ESCAPE); }, Callback);
	}

	void Window::WindowLoop(std::function<bool(void)> ExitCondition) {
		try {
			while (!ExitCondition()) {
				HandleMessages();
				Render();
			}
		} catch (Exception::HResult & ex) {
			if (m_CatchExceptions) {
				DisplayHresultErrorMessageBox(ex.hr, ex.Message);
			} else {
				throw ex;
			}
		} catch (Exception::Exception & ex) {
			if (m_CatchExceptions) {
				MessageBox(NULL, ex.Message.c_str(), L"Error", MB_OK | MB_ICONERROR);
			} else {
				throw ex;
			}
		}

	}

	void Window::WindowLoop(std::function<bool(void)> ExitCondition, std::function<void(float)> Callback) {
		try {
			QueryPerformanceCounter(&m_TimerPrev);
			while (!ExitCondition()) {
				QueryPerformanceCounter(&m_TimerNow);
				float elap = static_cast<float>(m_TimerNow.QuadPart - m_TimerPrev.QuadPart) / static_cast<float>(m_TimerFreq.QuadPart);
				m_TimerPrev = m_TimerNow;
				HandleMessages();
				Callback(elap);
				Render();
			}

		} catch (Exception::HResult & ex) {
			if (m_CatchExceptions) {
				DisplayHresultErrorMessageBox(ex.hr, ex.Message);
			} else {
				throw ex;
			}
		} catch (Exception::Exception & ex) {
			if (m_CatchExceptions) {
				MessageBox(NULL, ex.Message.c_str(), L"Error", MB_OK | MB_ICONERROR);
			} else {
				throw ex;
			}
		}

	}

	void Window::WindowLoop(std::function<bool(void)> ExitCondition, std::function<void(std::chrono::steady_clock::duration)> Callback) {
		try {
			m_ChronoPrev = std::chrono::steady_clock::now();
			while (!ExitCondition()) {
				m_ChronoNow = std::chrono::steady_clock::now();
				auto elap = m_ChronoNow - m_ChronoPrev;
				m_ChronoPrev = m_ChronoNow;
				HandleMessages();
				Callback(elap);
				Render();
			}

		} catch (Exception::HResult & ex) {
			if (m_CatchExceptions) {
				DisplayHresultErrorMessageBox(ex.hr, ex.Message);
			} else {
				throw ex;
			}
		} catch (Exception::Exception & ex) {
			if (m_CatchExceptions) {
				MessageBox(NULL, ex.Message.c_str(), L"Error", MB_OK | MB_ICONERROR);
			} else {
				throw ex;
			}
		}

	}

	void Window::SetSimpleRenderFunction(std::function<void(SimpleRenderer&)> simpleRender) {
		m_SimpleRenderFunction = simpleRender;
	}

	void Window::SetDirectRenderFunction(std::function<void(DirectXAccess&)> directRender) {
		m_DirectRenderFunction = directRender;
	}

	void Window::SetMouseEventFunction(std::function<void(UINT32, INT32, INT32, bool)> mouseEventFunction) {
		m_MouseEventFunction = mouseEventFunction;
	}

	void Window::SetMouseWheelFunction(std::function<void(INT32)> mouseWheelFunction) {
		m_MouseWheelFunction = mouseWheelFunction;
	}

	void Window::SetKeyEventFunction(std::function<void(UINT32, bool)> keyEventFunction) {
		m_KeyEventFunction = keyEventFunction;
	}

	void Window::SetWindowProcedure(std::function<std::pair<bool, LRESULT>(HWND, UINT32, WPARAM, LPARAM)> windowProc) {
		m_WindowProcedure = windowProc;
	}

	void Window::SetVsync(bool vsync) {
		m_Vsync = vsync;
	}

	bool Window::GetVsync() {
		return m_Vsync;
	}

	void Window::SetCatchExceptions(bool use) {
		m_CatchExceptions = use;
	}

	bool Window::GetCatchExceptions() {
		return m_CatchExceptions;
	}

	Bitmap Window::LoadBitmap(std::wstring fileName) {
		try {
			return ZorkLib::LoadBitmap(fileName, &m_DirectX);
		} catch (Exception::HResult & ex) {
			if (m_CatchExceptions) {
				DisplayHresultErrorMessageBox(ex.hr, ex.Message);
			} else {
				throw ex;
			}
		} catch (Exception::Exception & ex) {
			if (m_CatchExceptions) {
				MessageBox(NULL, ex.Message.c_str(), L"Error", MB_OK | MB_ICONERROR);
			} else {
				throw ex;
			}
		}
		return nullptr;
	}

	void Window::SaveContent(std::wstring fileName) {

		try {

			HRESULT hr;

			IWICBitmapEncoder* pEncoder;
			IStream* stream;
			IWICBitmapFrameEncode* pFrame;
			IWICImageEncoder* imgEnc;

			if (fileName.find(L".bmp") != fileName.npos) {
				hr = m_DirectX.pIWICFactory->CreateEncoder(GUID_ContainerFormatBmp, NULL, &pEncoder);
				Exception::AbortIfFailure(hr, L"Failed to create Encoder.");
			} else if (fileName.find(L".png") != fileName.npos) {
				hr = m_DirectX.pIWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
				Exception::AbortIfFailure(hr, L"Failed to create Encoder.");
			} else if (fileName.find(L".jpg") != fileName.npos) {
				hr = m_DirectX.pIWICFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &pEncoder);
				Exception::AbortIfFailure(hr, L"Failed to create Encoder.");
			} else {
				// TODO: print as binary blob?
				throw Exception::Unimplemented(L"Window::SaveContent needs to have file extension .jpg .bmp or .png");
				return;
			}

			hr = SHCreateStreamOnFile(fileName.c_str(), STGM_CREATE | STGM_READWRITE, &stream);
			Exception::AbortIfFailure(hr, L"Failure while saving content.");
			hr = pEncoder->Initialize(stream, WICBitmapEncoderNoCache);
			Exception::AbortIfFailure(hr, L"Failure while saving content.");
			hr = pEncoder->CreateNewFrame(&pFrame, NULL);
			Exception::AbortIfFailure(hr, L"Failure while saving content.");

			hr = pFrame->Initialize(0);
			Exception::AbortIfFailure(hr, L"Failure while saving content.");
			m_DirectX.pIWICFactory->CreateImageEncoder(m_DirectX.pDevice, &imgEnc);
			imgEnc->WriteFrame(m_DirectX.pTargetBitmap, pFrame, nullptr);

			hr = pEncoder->Commit();
			Exception::AbortIfFailure(hr, L"Failure while saving content.");
			hr = pFrame->Commit();
			Exception::AbortIfFailure(hr, L"Failure while saving content.");
			hr = stream->Commit(STGC_DEFAULT);
			Exception::AbortIfFailure(hr, L"Failure while saving content.");

			imgEnc->Release();
			pFrame->Release();
			stream->Release();
			pEncoder->Release();

		} catch (Exception::HResult & ex) {
			if (m_CatchExceptions) {
				DisplayHresultErrorMessageBox(ex.hr, ex.Message);
			} else {
				throw ex;
			}
		} catch (Exception::Exception & ex) {
			if (m_CatchExceptions) {
				MessageBox(NULL, ex.Message.c_str(), L"Error", MB_OK | MB_ICONERROR);
			} else {
				throw ex;
			}
		}

	}


	Point Window::GetCursorPos() {
		POINT p;
		::GetCursorPos(&p);
		ScreenToClient(m_DirectX.hWnd, &p);
		return Point(p.x, p.y);
	}

	DirectXAccess* Window::GetDirectXAccess() {
		return &m_DirectX;
	}

	void Window::DisplayHresultErrorMessageBox(HRESULT hr, std::wstring msg) {
		wchar_t errNr[20] = {0};
		auto aError = std::system_category().message(hr);
		auto wError = Utility::StringToWideString(aError);
		_itow_s(hr, errNr, 16);
		std::wstring err = msg + L"\n0x" + errNr + L": " + wError;
		MessageBox(NULL, err.c_str(), L"Error", MB_OK | MB_ICONERROR);
	}

	LRESULT Window::WndProc(HWND hWnd, UINT32 msg, WPARAM w, LPARAM l) {

		INT32 cursorX = GET_X_LPARAM(l);
		INT32 cursorY = GET_Y_LPARAM(l);

		if (m_WindowProcedure) {
			auto res = m_WindowProcedure(hWnd, msg, w, l);
			if (res.first) {
				return res.second;
			}
		}

		switch (msg) {

			case WM_KEYDOWN:
				if (m_KeyEventFunction) {
					m_KeyEventFunction(w, true);
				}
				return 0L;
			case WM_KEYUP:
				if (m_KeyEventFunction) {
					m_KeyEventFunction(w, false);
				}
				return 0L;
			case WM_LBUTTONDOWN:
				if (m_MouseEventFunction) {
					m_MouseEventFunction(VK_LBUTTON, cursorX, cursorY, true);
				}
				return 0L;
			case WM_LBUTTONUP:
				if (m_MouseEventFunction) {
					m_MouseEventFunction(VK_LBUTTON, cursorX, cursorY, false);
				}
				return 0L;
			case WM_RBUTTONDOWN:
				if (m_MouseEventFunction) {
					m_MouseEventFunction(VK_RBUTTON, cursorX, cursorY, true);
				}
				return 0L;
			case WM_RBUTTONUP:
				if (m_MouseEventFunction) {
					m_MouseEventFunction(VK_RBUTTON, cursorX, cursorY, false);
				}
				return 0L;
			case WM_MBUTTONDOWN:
				if (m_MouseEventFunction) {
					m_MouseEventFunction(VK_MBUTTON, cursorX, cursorY, true);
				}
				return 0L;
			case WM_MBUTTONUP:
				if (m_MouseEventFunction) {
					m_MouseEventFunction(VK_MBUTTON, cursorX, cursorY, false);
				}
				return 0L;
			case WM_XBUTTONDOWN:
				if (m_MouseEventFunction) {
					m_MouseEventFunction((w &= MK_XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2, cursorX, cursorY, true);
				}
				return 0L;
			case WM_XBUTTONUP:
				if (m_MouseEventFunction) {
					m_MouseEventFunction((w &= MK_XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2, cursorX, cursorY, false);
				}
				return 0L;
			case WM_MOUSEWHEEL:
				if (m_MouseWheelFunction) {
					INT32 turns = GET_WHEEL_DELTA_WPARAM(w) / WHEEL_DELTA;
					m_MouseWheelFunction(turns);
				}
				return 0L;
		}

		return DefWindowProc(hWnd, msg, w, l);
	}

	LRESULT Window::StaticWindowProcedure(HWND hWnd, UINT32 msg, WPARAM w, LPARAM l) {
		if (msg == WM_NCCREATE) {
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(l);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) cs->lpCreateParams);
			return TRUE;
		} else {
			LONG_PTR wndPtr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
			Window* pWindow = reinterpret_cast<Window*>(wndPtr);
			Exception::AbortIfFailure(static_cast<bool>(pWindow), L"Failed to restore window pointer.");
			return pWindow->WndProc(hWnd, msg, w, l);
		}

	}

}