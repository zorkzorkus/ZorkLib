#include "ZorkLibUSB.hpp"

namespace ZorkLib {

	USBDevice::USBDevice(UINT16 vid, UINT16 pid) : USBDevice(vid, pid, true, true, L"", false) {}

	USBDevice::USBDevice(UINT16 vid, UINT16 pid, std::wstring serial) : USBDevice(vid, pid, true, true, serial, true) {}

	USBDevice::USBDevice(UINT16 vid, std::wstring serial) : USBDevice(vid, 0, true, false, L"", false) {}

	USBDevice::USBDevice(std::wstring serial) : USBDevice(0, 0, false, false, serial, true) {}

	USBDevice::~USBDevice() {
		*(m_pReferences) -= 1;
		if (*m_pReferences == 0) {

			delete m_pReferences;
			m_pReferences = nullptr;

			for (int i = 0; i < m_ConfigDesc.bNumInterfaces; ++i) {
				libusb_attach_kernel_driver(m_pHandle, i);
				libusb_release_interface(m_pHandle, i);
			}

			libusb_close(m_pHandle);
			libusb_unref_device(m_pDevice);
			m_pHandle = nullptr;
			m_pDevice = nullptr;
		}
	}

	USBDevice::USBDevice(const USBDevice * other) {
		m_pDevice = other->m_pDevice;
		m_pHandle = other->m_pHandle;
		m_pReferences = other->m_pReferences;
		++(*m_pReferences);
	}

	USBDevice::USBDevice(const USBDevice & other) {
		m_pDevice = other.m_pDevice;
		m_pHandle = other.m_pHandle;
		m_pReferences = other.m_pReferences;
		++(*m_pReferences);
	}

	USBDevice& USBDevice::operator=(const USBDevice* other) {
		this->~USBDevice();
		m_pDevice = other->m_pDevice;
		m_pHandle = other->m_pHandle;
		m_pReferences = other->m_pReferences;
		++(*m_pReferences);
		return *this;
	}

	USBDevice& USBDevice::operator=(const USBDevice & other) {
		this->~USBDevice();
		m_pDevice = other.m_pDevice;
		m_pHandle = other.m_pHandle;
		m_pReferences = other.m_pReferences;
		++(*m_pReferences);
		return *this;
	}

	INT32 USBDevice::ControlTransfer(UINT8* buffer, int length) {
		// TODO
		//libusb_control_transfer(m_pHandle)
		return INT32();
	}

	INT32 USBDevice::Write(UINT8 endpoint, UINT8 * buffer, int length, int& actual, UINT32 timeout) {
		return libusb_bulk_transfer(m_pHandle, endpoint & 0x7F, buffer, length, &actual, timeout);
	}

	INT32 USBDevice::Read(UINT8 endpoint, UINT8 * buffer, int length, int & actual, UINT32 timeout) {
		return libusb_bulk_transfer(m_pHandle, endpoint | 0x80, buffer, length, &actual, timeout);
	}

	USBDevice::USBDevice(UINT16 vid, UINT16 pid, bool useVid, bool usePid, std::wstring serial, bool useSerial) {

		std::wstring deviceString;

		if (useVid) {
			std::wstringstream wss;
			wss << std::hex << vid;
			deviceString += L"VID=" + wss.str();
		}

		if (usePid) {
			if (!deviceString.empty()) deviceString += L" ";
			std::wstringstream wss;
			wss << std::hex << pid;
			deviceString += L"PID=" + wss.str();
		}

		if (useSerial) {
			if (!deviceString.empty()) deviceString += L" ";
			deviceString += L"SERIAL=" + serial;
		}

		auto init = libusb_init(nullptr);
		if (init != LIBUSB_SUCCESS) {
			throw Exception::USBError(L"Initialize failed");
		}

		libusb_device** deviceList;

		auto deviceListLength = libusb_get_device_list(nullptr, &deviceList);

		if (deviceListLength <= 0) {
			throw Exception::USBNoDevice(deviceString);
		}

		for (ssize_t i = 0; i < deviceListLength; ++i) {
			if (libusb_get_device_descriptor(deviceList[i], &m_DeviceDesc) < 0) {
				continue;
			}
			uint8_t buffer[64];
			libusb_device_handle* pHandle;
			if (libusb_open(deviceList[i], &pHandle) < 0) {
				continue;
			}
			if (libusb_get_string_descriptor(pHandle, m_DeviceDesc.iSerialNumber, 0, buffer, 64) < 0) {
				continue;
			}
			uint8_t stringLength = buffer[0];
			memset(buffer + stringLength, 0, 64 - stringLength);
			std::wstring serialFromBuffer(reinterpret_cast<const wchar_t*>(buffer + 2));
			if ((serial == serialFromBuffer || !useSerial) && (m_DeviceDesc.idVendor == vid || !useVid) && (m_DeviceDesc.idProduct == pid || !usePid)) {
				m_pDevice = deviceList[i];
				m_pHandle = pHandle;
				m_pReferences = new size_t(1);
				InitializeDevice();
				libusb_free_device_list(deviceList, 1);
				return;
			}
			libusb_close(pHandle);

		}

		libusb_free_device_list(deviceList, 1);
		throw Exception::USBNoDevice(deviceString);
	}

	void USBDevice::InitializeDevice() {

		libusb_config_descriptor* pConfig;
		libusb_get_active_config_descriptor(m_pDevice, &pConfig);
		memcpy(&m_ConfigDesc, pConfig, sizeof(libusb_config_descriptor));
		libusb_free_config_descriptor(pConfig);

		for (int i = 0; i < m_ConfigDesc.bNumInterfaces; ++i) {
			libusb_detach_kernel_driver(m_pHandle, i);
			libusb_claim_interface(m_pHandle, i);
		}

	}

};
