/*
 * file.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef SYSTEM_FILE_FILE_HPP_
#define SYSTEM_FILE_FILE_HPP_

#include <stdint.h>
#include <string>
#include "../../util/type.hpp"
#include "fileBase.hpp"
#include "../../util/math/fix.hpp"

namespace System{
	namespace File{


		template<class Func> //インライン展開しやすいように変更
		class Execute: public FileBase {
		private:
			Func m_func;
			Execute(const std::string& _name, const Func& _func) :
					FileBase(_name), m_func(_func) {
				SetType(FileType::File_Execute);
			}
		public:
			static Execute<Func>* Create(const std::string& _filename,
					const Func& _func) {
				return new Execute<Func>(_filename, _func);
			}
			virtual int operator()(Util::TextIterator_t _begin, Util::TextIterator_t _end) {
				return m_func(_begin, _end);
			}
		};

		//互換性を保つために作成した関数ポインター版(非推奨)
		template<> class Execute<ExecuteFp> : public FileBase {
			ExecuteFp m_func;
			Execute(const std::string& _name, ExecuteFp _func) :
					FileBase(_name), m_func(_func) {
				SetType(FileType::File_Execute);

			}
		public:
			static Execute<ExecuteFp>* Create(const std::string& _filename,
					const ExecuteFp& _func) {
				return new Execute<ExecuteFp>(_filename, _func);
			}
			virtual int operator()(Util::TextIterator_t _begin, Util::TextIterator_t _end) {
				return m_func(_begin, _end);
			}
		};
		//毎回書くのが面倒なので省略形
		template<class T> FileBase* CreateExecute(const std::string& _filename,
				const T& _obj) {
			return Execute<T>::Create(_filename, _obj);
		}

		class Integer: public FileBase {
		private:
			int32_t* data;
			Integer(const std::string& _filename, int32_t* _d);
		public:
			static Integer* Create(const std::string& _filename, int32_t* _d = nullptr);
			virtual std::string GetData();
			virtual int SetData(const std::string&);
		};

		class Float: public FileBase {
		private:
			float* data;
			Float(const std::string& _filename, float* _f);
		public:
			static Float* Create(const std::string& _filename, float* _f = nullptr);
			virtual std::string GetData();
			virtual int SetData(const std::string&);
		};

		class String: public FileBase {
		private:
			std::string* data;
			String(const std::string& _filename, std::string* _str = nullptr);
		public:
			static String* Create(const std::string& _filename, std::string* _str);
			virtual std::string GetData();
			virtual int SetData(const std::string&);
		};

		class Fix: public FileBase {
		private:
			Util::Math::fix32* data;
			Fix(const std::string& _filename, Util::Math::fix32* _f = nullptr);
		public:
			static Fix* Create(const std::string& _filename, Util::Math::fix32* _f);
			virtual std::string GetData();
			virtual int SetData(const std::string&);
		};

		template<class FSet, class FGet> class Property: public FileBase {
		private:
			FSet m_set;
			FGet m_get;
			Property(const std::string& _filename, const FSet& _set, const FGet& _get) :
					FileBase(_filename), m_set(_set), m_get(_get) {
					SetType(FileType::File_Properties);

			}
		public:
			static Property<FSet, FGet>* Create(const std::string& _filename,
					const FSet& _set, const FGet& _get) {
				return new Property<FSet, FGet>(_filename, _set, _get);
			}
			virtual std::string GetData() {
				return m_get();
			}
			virtual int SetData(const std::string& _value) {
				return m_set(_value);
			}
		};

		//関数ポインタ版[互換性のため]
		template <>class Property<SetFp,GetFp>: public FileBase {
		private:
			SetFp m_set;
			GetFp m_get;
			Property(const std::string& _filename, const SetFp& _set, const GetFp& _get) :
					FileBase(_filename), m_set(_set), m_get(_get) {
					SetType(FileType::File_Properties);

			}
		public:
			static Property<SetFp, GetFp>* Create(const std::string& _filename,
					const SetFp& _set, const GetFp& _get) {
				return new Property<SetFp, GetFp>(_filename, _set, _get);
			}
			virtual std::string GetData() {
				return m_get();
			}
			virtual int SetData(const std::string& _value) {
				return m_set(_value);
			}
		};


		template<class FGet> class ReadOnlyProperty: public FileBase {
		private:
			FGet m_get;
			ReadOnlyProperty(const std::string _filename, const FGet& _get) :
					FileBase(_filename), m_get(_get) {
				SetType(FileType::File_Properties);

			}
		public:
			static ReadOnlyProperty<FGet>* Create(const std::string & _filename,
					const FGet& _get) {
				return new ReadOnlyProperty<FGet>(_filename, _get);
			}
			virtual std::string GetData() {
				return m_get();
			}
		};

		template <>class ReadOnlyProperty<GetFp>: public FileBase {
		private:
			GetFp m_get;
			ReadOnlyProperty(const std::string _filename, const GetFp& _get) :
					FileBase(_filename), m_get(_get) {
				SetType(FileType::File_Properties);

			}
		public:
			static ReadOnlyProperty<GetFp>* Create(const std::string & _filename,
					const GetFp& _get) {
				return new ReadOnlyProperty<GetFp>(_filename, _get);
			}
			virtual std::string GetData() {
				return m_get();
			}
		};

		template<class FSet> class WriteOnlyProperty: public FileBase {
		private:
			FSet m_set;
			WriteOnlyProperty(const std::string& _name, const FSet& _set) :
					FileBase(_name), m_set(_set) {
				SetType(FileType::File_Properties);

			}
		public:
			static WriteOnlyProperty<FSet>* Create(const std::string & _filename,
					const FSet& _set) {
				return new WriteOnlyProperty<FSet*>(_filename, _set);
			}
			virtual std::string SetData(const std::string& _value){
				return m_set(_value);
			}
		};

		template<> class WriteOnlyProperty<SetFp>: public FileBase {
		private:
			SetFp m_set;
			WriteOnlyProperty(const std::string& _name, const SetFp& _set) :
					FileBase(_name), m_set(_set) {
				SetType(FileType::File_Properties);

			}
		public:
			static WriteOnlyProperty<SetFp>* Create(const std::string & _filename,
					const SetFp& _set) {
				return new WriteOnlyProperty<SetFp>(_filename, _set);
			}
			virtual int SetData(const std::string& _value){
				return m_set(_value);
			}
		};

		template<class FSet, class FGet> Property<FSet, FGet>* CreateProperty(
				const std::string & _name, const FSet& _set, const FGet& _get) {
			return new Property<FSet, FGet>(_name, _set, _get);
		}

		template<class FGet> ReadOnlyProperty<FGet>*CreateReadOnlyProperty(
				const std::string& _name, const FGet& _get) {
			return ReadOnlyProperty<FGet>::Create(_name, _get);
		}

		template<class FSet> WriteOnlyProperty<FSet>*CreateWriteOnlyProperty(
				const std::string& _name, const FSet& _set) {
			return WriteOnlyProperty<FSet>::Create(_name, _set);
		}

	}
}


#endif /* SYSTEM_FILE_FILE_HPP_ */
