/*
 * xport.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef DRIVER_XPORT_HPP_
#define DRIVER_XPORT_HPP_

#include <stdint.h>
#include <string>
#include "../util/text.hpp"
#include "../util/bit.hpp"

namespace System{
	namespace Console{
	//入出力先に切り替えようの列挙体もどき
		struct Stream {
		private:
			uint32_t mData;
		public:
			const static Stream None, USB, UART, USB_UART;
			Stream(){}
			constexpr Stream(uint32_t _data) :
					mData(_data) {
			}

			constexpr Stream(const Stream&) = default;

			inline Stream operator =(Stream _inst) {
				return Stream(mData = _inst.mData);
			}

			//集合和
			inline Stream operator +(Stream _inst) const {
				return Stream(mData | _inst.mData);
			}
			inline Stream operator +=(Stream _inst) {
				return Stream(mData |= _inst.mData);
			}
			//集合差
			inline Stream operator -(Stream _inst) const {
				return Stream(mData & ~_inst.mData);
			}

			inline Stream operator -=(Stream _inst) {
				return Stream(mData &= ~_inst.mData);
			}

			bool IsUSB() const {
				return mData & USB.mData;
			}
			bool IsUart() const {
				return mData & UART.mData;
			}
			bool IsClosed() const {
				return mData == None.mData;
			}
			bool IsSingle()const{
				return Util::BitCount(mData) == 1;
			}
		};

		void Init();

		//Portを開放する  (失敗時には負数)
		Stream GetStream();
		void SetStream(Stream);
		bool IsEnableUSB();

		constexpr size_t TxBufferSize = 256;//書き込みバッファー
		constexpr size_t TxBufferLimit = 192;//Flushサイズ
		constexpr size_t RxBufferSize = 256;//読み込みバッファのサイズ
		constexpr size_t RxBufferLimit = 64;//読み取りサイズ

		bool IsEmpty();
		void Write(char);
		void Write(const uint8_t* _data, size_t _sz);
		void Write(const char*);
		void Write(const std::string& _text);
		void Write(const std::vector<char>& _vec);
		void WriteLine();
		void WriteLine(const char*);
		void WriteLine(const std::string& _text);

		std::string ReadLine();
		std::string Read();

		void Clear();
		void Flush(); //送信同期

		bool IsExist(char);
		static inline bool IsLine(){
			return IsExist(Util::NewLine);
		}
	}
}


#endif /* DRIVER_XPORT_HPP_ */
