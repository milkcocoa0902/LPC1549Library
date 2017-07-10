/*
 * xport.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */


#include "console.hpp"


#include "../util/simpleBuf.hpp"
#include "../util/bit.hpp"
#include "../driver/comm/usb.hpp"
#include <ring_buffer.hpp>

#include "../driver/comm/uart.hpp"

namespace System{
	namespace Console{
		const Stream Stream::None = 0, Stream::USB = 1, Stream::UART = 2, Stream::USB_UART = 3;
		static Stream stream;
		static Util::SimpleBuffer<TxBufferSize> TxBuf;
		Driver::Serial serial;

		void Init() {
			TxBuf.Reset();
			stream = Stream::UART;
			serial = Driver::Serial({0, 18}, {0, 13}, 0);

		}

		Stream GetStream() {
			return stream;
		}

		void SetStream(Stream _port) {
			stream = _port;
		}

		bool IsEnableUSB() {
			return Driver::USB::IsConnected();
		}

		void Write(char _c) {
			TxBuf.Write((uint8_t)_c);
			if (TxBuf.GetDepth() > TxBufferLimit) {
				Flush();
			}
		}

		void Write(const uint8_t* _data, size_t _sz) {
			if (TxBuf.GetDepth() + _sz > TxBufferSize) {
				Flush();
			}

			TxBuf.Write((uint8_t*)_data, std::min(_sz, TxBufferSize));

			if (TxBuf.GetDepth() > TxBufferLimit) {
				Flush();
			}
		}

		void Write(const char* _text) {
			Write((uint8_t*)_text, strlen(_text));
		}

		void Write(const std::string& _text) {
			Write((uint8_t*)_text.data(), _text.length());
		}

		void Write(const std::vector<char>& _vec) {
			Write((uint8_t*)_vec.data(), _vec.size());
		}

		void WriteLine() {
			Write(Util::NewLine);
		}

		void WriteLine(const char* _str) {
			Write(_str);
			WriteLine();
		}

		void WriteLine(const std::string& _text) {
			Write(_text + Util::NewLine);
		}

		bool IsEmpty() {
			bool flag = true;
			if (stream.IsUart()) {
				flag &= serial.IsEmpty();
			}
			if (stream.IsUSB()) {
				flag &= Driver::USB::IsEmpty();
			}
			return flag;
		}

		void Clear() {
			if (stream.IsUart()) {
				serial.Claer();
			}
			if (stream.IsUSB()) {
				Driver::USB::Clear();
			}
		}

		void Flush() {
			if (stream.IsUSB()) {
				Driver::USB::Write(TxBuf.GetData(), TxBuf.GetDepth());
			}
			if (stream.IsUart()) {
				serial.Write(TxBuf.GetData(),TxBuf.GetDepth());
			}
			TxBuf.Reset();
		}

		bool IsExist(char _c) {
			if (stream.IsSingle()) {
				if (stream.IsUSB()) {
					return Driver::USB::IsExist(_c);
				}
				if (stream.IsUart()) {
					return serial.IsExist(_c);
				}
			}
			return false;
		}

		std::string ReadLine() {
			if (stream.IsSingle()) {
				if (stream.IsUSB()) {
					return Driver::USB::ReadLine();
				}
				if (stream.IsUart()) {
					return serial.ReadLine();
				}
			}
			return "";
		}

		std::string Read() {
			if (stream.IsSingle()) {
				if (stream.IsUSB()) {
					return Driver::USB::Read();
				}
				if (stream.IsUart()) {
					return serial.Read();
				}
			}
			return "";
		}
	}
}

