/*
 * uart.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include <ring_buffer.hpp>
#include <chip.hpp>

#include "uart.hpp"
#include "usb.hpp"



namespace Driver{
		constexpr uint32_t Serial::UartChNum;
		constexpr size_t Serial::TxSize, Serial::RxSize; //バッファーのサイズ
		RINGBUFF_T Serial::TxBuf[3], Serial::RxBuf[3];
		char Serial::TxRaw[3][TxSize], Serial::RxRaw[3][RxSize];
		LPC_USART_T* Serial::UartBase[3] = {LPC_USART0, LPC_USART1, LPC_USART2};

		Serial::Serial(std::pair<uint8_t, uint8_t> _tx, std::pair<uint8_t, uint8_t> _rx, uint32_t _id, uint32_t _baud):
						tx(Driver::GPIO::Digital{_tx.first, _tx.second}),
						rx(Driver::GPIO::Digital{_rx.first, _rx.second}),
						id(_id),
						baud(_baud)
		{
			tx(GPIO::DIRECTION_OUTPUT)((CHIP_SWM_PIN_FIXED_T)(SWM_UART0_TXD_O + 0x11 * id));
			rx(GPIO::DIRECTION_INPUT)((CHIP_SWM_PIN_FIXED_T)(SWM_UART0_RXD_I + 0x11 * id));
			//Clock Supply
			Chip_Clock_SetUARTBaseClockRate(Chip_Clock_GetMainClockRate(), false);
			Chip_Clock_SetUARTFRGDivider(1);
			Chip_UART_Init(UartBase[id]);
			//Configuration
			Chip_UART_ConfigData(UartBase[id], UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
			Chip_UART_SetBaud(UartBase[id], baud);
			//Buffer Initialize
			RingBuffer_Init(&TxBuf[id], &TxRaw[id][0], sizeof(char), TxSize);
			RingBuffer_Init(&RxBuf[id], &RxRaw[id][0], sizeof(char), RxSize);
			//割り込み処理
			Chip_UART_IntEnable(UartBase[id], UART_INTEN_RXRDY | UART_INTEN_TXRDY);
			NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + id));

			Chip_UART_TXEnable(UartBase[id]);
			Chip_UART_Enable(UartBase[id]);

		}

		void Serial::Write(char _c) {
			while(IsFull());
			Chip_UART_SendRB(UartBase[id], &TxBuf[id], &_c, sizeof(char));
		}

		void Serial::Write(const uint8_t* _data, size_t _sz) {
			Chip_UART_SendRB(UartBase[id], &TxBuf[id], _data, _sz);
		}

		void Serial::Write(const char* _str) {
			Write((uint8_t*)_str, strlen(_str));
		}

		void Serial::Write(const std::vector<uint8_t>& _data) {
			Write(_data.data(), _data.size());
		}

		void Serial::Write(const std::string& _text) {
			Write((uint8_t*)_text.data(), _text.length());
		}

		void Serial::WriteLine() {
			Write('\r');
		}

		void Serial::WriteLine(const std::string& line) {
			Write(line);
			WriteLine();
		}

		void Serial::WriteLine(const char * line) {
			Write(line);
			WriteLine();
		}

		void Serial::WriteLine(const std::vector<uint8_t>& bytes) {
			Write(bytes);
			WriteLine();
		}

		char Serial::ReadByte() {
			char c;
			while (RingBuffer_IsEmpty(&RxBuf[id])){
				__NOP();
			}
			Chip_UART_ReadRB(UartBase[id], &RxBuf[id], &c, sizeof(char));
			return c;
		}

		std::string Serial::Read(size_t _sz) {
			std::string s = "";
			while (_sz--){
				s += ReadByte();
			}
			return s;
		}

		std::string Serial::Read() {
			std::string s = "";
			while (!IsEmpty()){
				s += ReadByte();
			}
			return s;
		}

		std::string Serial::ReadLine() {
			std::string s = "";
			char c;
			while ((c = ReadByte()) != '\r'){
				s += c;
			}
			return s;
		}

		bool Serial::IsEmpty(){
			return RingBuffer_IsEmpty(&RxBuf[id]);
		}

		bool Serial::IsFull(){
			return RingBuffer_IsFull(&TxBuf[id]);
		}

		void Serial::Claer(){
			RingBuffer_Flush(&RxBuf[id]);
		}

		bool Serial::IsBusy(){
			return !RingBuffer_IsEmpty(&TxBuf[id]);
		}

		bool Serial::IsExist(char _c){
			auto* data = (char*)RxRaw[id];
			for (unsigned int i = RxBuf[id].tail; i != RxBuf[id].head; i = (i + 1) % RxBuf[id].count){
				if (data[i]==_c)
					return true;
			}
			return false;
		}

		void Serial::setBaud(uint32_t _baud){
			baud = _baud;
			Chip_UART_SetBaud(UartBase[id], baud);
		}

		extern "C" void UART0_IRQHandler(void) {
			Serial::IRQHandler(0);
		}

		extern "C" void UART1_IRQHandler(void) {
			Serial::IRQHandler(1);
		}

		extern "C" void UART2_IRQHandler(void) {
			Serial::IRQHandler(2);
		}
}

