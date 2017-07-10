/*
 * memPool.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_MEMPOOL_HPP_
#define UTIL_MEMPOOL_HPP_


#include <stdint.h>
#include <stddef.h>
#include <string.h>

namespace Util{

	//[begin,end)の中に入っているかの確認
	static inline bool InRange(void* _cmp, void* _begin, void* _end){
		bool low = (_begin <= _cmp);
		bool high = (_cmp < _end);
		return (low && high);
	}

	/* size 確保する大きさ
	 * num　確保する個数
	 */
	template<size_t _size, unsigned _num> class MemPool {
		union item {
			uint16_t dummy[_size];
			item* next;
		};
		item data[_num];
		item *top; //使っていない要素の単方向リスト

	private:
		void Push(item* it) {
			it->next = top;
			top = it;
		}

		item* Pop() {
			item* temp = top;
			top = top->next;
			temp->next = nullptr;/*バグ防止*/
			return temp;
		}

	public:
		bool IsFull() const {
			return top == nullptr;
		}

		MemPool() {
			//全ての単方向リストを連結する
			for (unsigned int i = 0; i < _num; i++) {
				Push(&data[i]);
			}
		}

		MemPool(const MemPool&) = delete;

		void* CreatePointer() {
			return Pop();
		}

		void ReleasePointer(void* _ptr) {
			if (_ptr != nullptr) {
				Push((item*)_ptr);
			}
		}

		int CountArea() const {
			return _num;
		}

		int CountAreaByte() const {
			return _num * _size;
		}

		unsigned int CountFree() const {
			unsigned int n = 0;
			for (item* it = top; it != nullptr; it = it->next) {
				n++;
			}
			return n;
		}

		size_t CountFreeByte() const {
			return _size * CountFree();
		}

		unsigned int CountUsed() const {
			return _num - CountFree();
		}

		size_t CountUsedByte() const {
			return _size * CountUsed();
		}

	};

	/* 固定用メモリーダンプ
	 */
	template <size_t _size>class LockedPool{
		size_t pos;
		char mem[_size];
	public:
		LockedPool(){
			pos=0;
		}

		void* CreatePointer(size_t _sz){
			const size_t next = (pos + _sz);
			const size_t now = pos;
			if (next < _size){
				pos = next;
				return &mem[now];
			}else{
				return nullptr;
			}
		}

		void ReleasePointer(void*& _ptr){
			//解放処理としては何もしない。
			_ptr = nullptr;
		}

		const char* Clone(const char* _str){
			char* ptr = (char*)CreatePointer(strlen(_str) + 1);
			strcpy(ptr, _str);
			return ptr;
		}

		size_t CountUsedByte()const{
			return pos;
		}

		size_t CountFreeByte()const{
			return _size - pos;
		}

		size_t CountAllByte()const{
			return _size;
		}

	};


}


#endif /* UTIL_MEMPOOL_HPP_ */
