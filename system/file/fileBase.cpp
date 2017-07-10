/*
 * fileBase.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include <algorithm>
#include "fileBase.hpp"
#include "../console.hpp"
#include "../../util/mempool.hpp"
#include "../../util/text.hpp"

namespace System{
	namespace File{
		static Util::MemPool<FileBase::mMaxSize, FileBase::mMaxNumber> mPool;
		static Util::LockedPool<FileBase::mMaxText> mTextPool;
		static size_t mSize = 0;
		Directory *root { nullptr }, *current { nullptr };

		void* FileBase::operator new(size_t _sz) {
			if (_sz > mMaxSize) {
				return NULL; //クラスの大きさが不正
			}
			mSize = std::max(_sz, mSize);
			return mPool.CreatePointer();
		}

		void FileBase::operator delete(void* _ptr) {
			mPool.ReleasePointer(_ptr);
		}

		FileBase::FileBase(const std::string& _name): mName(mTextPool.Clone(_name.c_str())){

		}

		FileBase::~FileBase() {
			if (mNext != nullptr) {
				delete mNext;
				mNext = nullptr;
			}
			if (mChild != nullptr) {
				delete mChild;
				mChild = nullptr;
			}
		}

		void FileBase::Add(FileBase* _ptr) {
			//これは自身が親になる。
			if (_ptr == nullptr)
				return; //無視

			_ptr->mNext = mChild;
			mChild = _ptr;
			_ptr->mParent = this;
		}
		/*
		 string FileBase::operator()(std::vector<std::string>& s) {
		 return "NonSupport:Type=[" + ToStr((int) type) + "]+Mode=["
		 + ToStr((int) mode) + "]";
		 }
		 */
		int FileBase::operator()(Util::TextIterator_t, Util::TextIterator_t) {
			Console::WriteLine("AccessError");
			return -1;
		}

		std::string FileBase::GetData() {
			return "AccessError";
		}

		int FileBase::SetData(const std::string&) {
			Console::WriteLine("AccessError");
			return -2;
		}

		FileBase* FileBase::SearchChilren(const std::string& _name) {
			for (auto *it = mChild; it != nullptr; it = it->mNext) {
				if (it->mName == _name) {
					return it;
				}
			}
			return nullptr;
		}

		FileBase* FileBase::Search(const Util::TextVector_t& _lst) {
			FileBase* it = this;
			for (const std::string& cmp : _lst) {
				if (cmp == "..") {
					it = it->mParent;
				} else if (cmp == ".") {
					//何もしない
				} else {
					it = it->SearchChilren(cmp);
					if (it == nullptr)
						return nullptr;
				}
			}
			return it;
		}

		FileBase* FileBase::Search(const std::string& _path) {
			return Search(Util::Split(_path, "/"));
		}

		std::string FileBase::GetPathName() const {
			if (mParent != nullptr) {
				return mParent->GetPathName() + mName + '/';
			} else {
				return "/";
			}
		}

		std::string FileBase::GetChildrenName() const {
			if (mChild != nullptr)
				return mChild->GetChildrenNameSub();
			return "";
		}

		std::string FileBase::GetAllName(unsigned int _sp) const {
			std::string ans = Util::Space("|", _sp) + '+' + mName + Util::NewLine;
			for (auto it = mChild; it != nullptr; it = it->mNext) {
				ans += it->GetAllName(_sp + 1);
			}
			return ans;
		}

		size_t FileBase::GetMemorySizeAll() {
			return mPool.CountAreaByte();
		}
		size_t FileBase::GetMemorySizeUsed() {
			return mPool.CountUsedByte();
		}
		size_t FileBase::GetMemorySizeFree() {
			return mPool.CountFreeByte();
		}

		std::string FileBase::GetChildrenNameSub() const {
			if (mNext != nullptr) {
				return mNext->GetChildrenNameSub() + Util::Comma + mName;
			} else {
				return mName;
			}
		}

		size_t FileBase::MaxItemSize(){
			return mSize;
		}

		size_t FileBase::GetTextSizeUsed(){
			return mTextPool.CountUsedByte();
		}

		size_t FileBase::GetTextSizeFree(){
			return mTextPool.CountFreeByte();
		}

		size_t FileBase::GetTextSizeAll(){
			return mTextPool.CountAllByte();
		}

		Directory::Directory(const std::string& _name): FileBase(_name) {
			SetType(FileType::File_Directory);
		}

		Directory* Directory::Create(const std::string& _name) {
			auto *p = new Directory(_name);
			return p;
		}

		void Directory::Add(FileBase* _p) {
			FileBase::Add(_p);
		}
	}
}


