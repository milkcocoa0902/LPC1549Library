/*
 * fileBase.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef SYSTEM_FILE_FILEBASE_HPP_
#define SYSTEM_FILE_FILEBASE_HPP_

#include "../../util/type.hpp"
#include <string>

namespace System{
	namespace File{
		using ExecuteFp = int(*)(Util::TextIterator_t, Util::TextIterator_t);
		using SetFp = int(*)(const std::string&);
		using GetFp = std::string(*)(void);


		enum class FileType: uint16_t {
			File_None = 0, //エラー用の空集合
			File_Directory,
			File_Execute, //通常の実行可能な関数
			File_Properties, //動的に値を取得するもの、与えるもの
			File_Int32,
			File_Float,
			File_String,
			File_Fix,
		};

		class FileBase {
		public:
			static constexpr uint32_t mMaxNumber = 64; //ファイルの最大数
			static constexpr size_t mMaxSize = 32; //派生先を含むクラスの最大サイズ[Byte]
			static constexpr size_t mMaxText = 256;//ファイルシステムで使う文字列の合計数[Byte]

		private:
			//std::string name;
			const char* mName;
			//本当はスマートポインタを使うはずだった..
			FileBase* mParent { nullptr };
			FileBase* mChild { nullptr };
			FileBase* mNext { nullptr };
			FileType mType { FileType::File_None };

		protected:
			static void* operator new(size_t);
			static void operator delete(void*);
			void SetType(FileType _flag) {
				mType = _flag;
			}

			void Add(FileBase*); //子要素として追加

		public:
			FileBase(const std::string& _name); //本来はprotected
			virtual ~FileBase();
			virtual int operator ()(Util::TextIterator_t _begin, Util::TextIterator_t _end);
			FileType GetFlag() const {
				return mType;
			}

			std::string GetName() const {
				return mName;
			}
			FileBase* SearchChilren(const std::string& _name);
			FileBase* Search(const Util::TextVector_t&);
			FileBase* Search(const std::string&);
			virtual std::string GetData();
			virtual int SetData(const std::string&);
			std::string GetPathName() const;
			std::string GetChildrenName() const;
			std::string GetAllName(unsigned int _sp = 0) const;
			//メモリーの配置状態の取得
			static size_t GetMemorySizeAll();
			static size_t GetMemorySizeUsed();
			static size_t GetMemorySizeFree();
			static size_t MaxItemSize();
			static size_t GetTextSizeUsed();
			static size_t GetTextSizeFree();
			static size_t GetTextSizeAll();

		private:
			std::string GetChildrenNameSub() const;
		};


		class Directory: public FileBase {
		private:
			Directory(const std::string& _name);

		public:
			virtual ~Directory() = default;
			static Directory* Create(const std::string& _name);
			void Add(FileBase* _p);
		};


		extern Directory *root, *current;
	}
}



#endif /* SYSTEM_FILE_FILEBASE_HPP_ */
