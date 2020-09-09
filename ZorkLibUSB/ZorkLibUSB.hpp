#pragma once

#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "libusb64d.lib")
#else
#pragma comment(lib, "libusb32d.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "libusb64.lib")
#else
#pragma comment(lib, "libusb32.lib")
#endif
#endif

#ifndef ZORKLIB_HEADER_USB
#define ZORKLIB_HEADER_USB
#endif

#include <libusb.h>
#include <basetsd.h>
#include <sstream>
#include <string>

namespace ZorkLib {

	namespace Exception {

		struct USBNoDevice {
			USBNoDevice(std::wstring devStr) : m_DeviceString(devStr) {}
			std::wstring m_DeviceString;
		};

		struct USBError {
			USBError(std::wstring errMsg) : m_ErrorMessage(errMsg){}
			std::wstring m_ErrorMessage;
		};

	}

	class USBDevice {

	public:

		// -----------------

		// Constructors
		USBDevice(UINT16 vid, UINT16 pid);
		USBDevice(UINT16 vid, UINT16 pid, std::wstring serial);
		USBDevice(UINT16 vid, std::wstring serial);
		USBDevice(std::wstring serial);

		// Destructor
		~USBDevice();

		// CopyMove Constructors
		USBDevice(const USBDevice* other);
		USBDevice(const USBDevice& other);
		USBDevice(USBDevice&& other) noexcept;

		// Assignment
		USBDevice& operator=(const USBDevice* other);
		USBDevice& operator=(const USBDevice& other);
		USBDevice& operator=(USBDevice&& other) noexcept;

		// ----------------

		// USB Functionality
		INT32 ControlTransfer(UINT8 bmRequestType, UINT8 bRequest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8* buffer = nullptr, UINT32 timeout = 1000);
		INT32 Write(UINT8 endpoint, UINT8* buffer, int length, int& actual, UINT32 timeout = 1000);
		INT32 Read(UINT8 endpoint, UINT8* buffer, int length, int& actual, UINT32 timeout = 1000);

	private:

		USBDevice(UINT16 vid, UINT16 pid, bool useVid, bool usePid, std::wstring serial, bool useSerial);

		void InitializeDevice();

		libusb_device* m_pDevice;
		libusb_device_handle* m_pHandle;
		libusb_device_descriptor m_DeviceDesc;
		libusb_config_descriptor m_ConfigDesc;
		size_t* m_pReferences;

	};

}