#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

namespace July
{
	class string
	{
		friend istream& operator>>(istream& in, string& s);
		friend istream& operator<<(istream& in, string& s);
	public:
		/*
		// ���캯��
		// �ַ���ָ�벻���� nullptr, ��ʼ��Ҳ����������, ��Ҫ���㶯̬����
		// ������Ҫ�ڹ��캯���ڲ�, new һ����С��Ϊ 1��С���ַ�����(�ַ���)
		// str[0] = '\0' ���Ա�ʾ����Ϊ��, ��Ϊ�ַ���������־Ϊ '\0'
		string()
			:_size(0)
			,_capacity(0)
		{
			_str = new char[1];
			_str[0] = '\0';
		}
		// string ���캯������Ҫ ʵ�ִ��ַ���ʵ��������
		// _capacity �� _size �Ĵ�Сֱ���� �ַ����ĳ��ȸ�ֵ
		// �����ڹ��캯���ڲ���Ҫ��new һ����С��Ϊ _capacity + 1��С���ַ�����(�ַ���)
		// _capacity �� _size ����ʾ��Ч���ݣ����� + 1��Ϊ '\0'����
		string(const char* str)
			:_size(strlen(str))
			,_capacity(_size)
		// ��ʼ���б�ĳ�ʼ��˳���ǰ�������˳��ִ�еģ�����������Ҫ _capacity ���� _size ֮��
		{
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}*/

		// ��ʵ �����������캯�����Ժϲ�Ϊһ�����캯��
		string(const char* str = "")
			:_size(strlen(str))
			, _capacity(_size)
		{
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}
		// ֻ��Ҫ�� �����ַ��� ���һ��ȱʡ���� ""���Ϳ���ʵ�ֲ�����ʱ��Ĭ�ϲ�����ʵ��һ����Ч����Ϊ0���ַ���

		// string��� ��������Ҫʵ��Ϊ ���
		// ��Ϊ����ǳ�����ǶԶ���ÿ�ֽڿ����ģ� �Զ�̬���ٵ��ڴ�ռ�ǳ�������ᵼ�²�ͬ�����е���ָ�빲ָͬ��ͬһ��ռ�
		// ���������е�����ָ��ָ��ͬһ���ڴ�ռ䣬�ᵼ�����������
		// 1. ͬһ��ռ�ᱻ�������Σ����´�����Ϊ��һ������֮���ǿ�ռ���Ѿ����ڲ���ϵͳ�ˣ��޷��ٲ���
		// 2. һ������ı��������ݣ���һ���������������Ҳ��ı�
		// ���� �����࿽�����캯�� ��Ҫʵ�����

		// ��ôstring ��������ʵ���أ�
		// �ȷ��� ���������������ʲô��
		// ��һ���Ѿ����ڵĶ���ʵ��������ʼ��һ������������ȫһ�����¶���
		// ������ʵ�ǳ��ļ�

		// �������촫ͳд��
		/*string(const string& s)
			:_size(s._size)
			, _capacity(s._capacity)
		{
			_str = new char[_capacity + 1];
			strcpy(_str, s._str);
		}*/
		void swap(string& s)
		{
			std::swap(_str, s._str);
			std::swap(_size, s._size);
			std::swap(_capacity, s._capacity);
		}
		// ���������ִ�д��
		// �ɴ��������ַ����������ֲ������ٽ�this��ֲ���������ݽ�������ɿ�������
		string(const string& s)
		{
			string tmp(s._str);
			swap(tmp);
		}

		// ���������ֵ ���и���ֵ���غ���
		// ��ֵ���غ�����ʹ�ý���� �������캯���ǳ�����
		// ������һ������������������ݣ�������һ���Ѿ�ʵ�����Ķ����У�ʹ����������������ͬ
		// ��Ҫʵ���������ã���õ�д��֮һ���ǣ�����һ���µ���ʱ�ַ����� strcpy �����ַ���
		// �ٽ���Ҫ��ֵ�Ķ�����ַ��� delete�����ٽ���ʱ�ַ�����ֵ����
		// ��Ϊ�����ֱ�Ӹ�ԭ�����ԭ�ַ����ռ� ��ֵ�����ԭ�ַ����ռ�С����ᷢ������
		// ���ԭ�ַ����ռ���� ����˷ѿռ�

		// ��ֵ���ش�ͳд��
		/*string& operator=(const string& s)
		{
			if (this != &s)
			{
				char* tmp = new char[s._capacity + 1];
				strcpy(tmp, s._str);
				delete[] _str;
				_str = tmp;
				_size = s._size;
				_capacity = s._capacity;
			}

			return *this;
		}*/
		// �������ֹ ���Ҹ�ֵ������뽫 delete[] _str; �Ĳ��� ���� strcpy(tmp, s._str) ֮��
		// ��Ϊ ������Ҹ�ֵ�������s ��Ϊ �Լ������ã�delete[] _str Ҳ�� delete s._str����ᵼ�� strcpy һ�����ֵ�������¸��ƴ���
		// ���� Ҫô��ֹ ���Ҹ�ֵ��Ҫô delete[] _str ���� strcpy(tmp, s._str) ֮��

		// ��ֵ�����ִ�д��
		// ��ֵ���صĵ�һ���ִ�д�����뿽��������ִ�д�� ����һ��
		/*string& operator=(const string& s)
		{
			if (this != &s)		// ��ֹ���Ҹ�ֵ
			{
				string tmp(s._str);
				swap(tmp);
			}

			return *this;
		}*/
		// ��ֵ���صĵڶ���д�������Ӽ�
		// �����˴�ֵ���� ���ʵ�ν�����ʱ���������ԣ�ֱ���������ʱ���������ݽ���
		string& operator=(string s)
		{
			swap(s);

			return *this;
		}

		// ������������ԱȽϼ���
		~string()
		{
			if (_str)	// ���ȱ�֤ _str ���ǿ�ָ��
			{
				delete[] _str;
				_str = nullptr;
				_size = _capacity = 0;
			}
		}

		// ��������ʵ��
		// �� string �� vector�У���������ʵ����ָ��
		typedef char* iterator;				// ��ͨ������
		typedef const char* const_iterator;		// const ���͵�����

		// begin()���� ����string���ַ������ַ���ַ
		iterator begin()
		{
			return _str;
		}
		const_iterator begin() const
		{
			return _str;
		}
		// end()���� ����string���ַ�����β��'\0'��ַ
		iterator end()
		{
			return _str + _size;
		}
		const_iterator end() const
		{
			return _str + _size;
		}

		// ���� c_str ���ض������ַ����� C��ָ����ʽ
		// ���տ��е����� const����
		const char* c_str() const
		{
			return _str;
		}

		// [] ���������أ�ʵ���±���ʡ��޸��ַ�������
		char& operator[](size_t pos)
		{
			assert(pos < _size);		// �±�Ҫ�Ϸ�

			return _str[pos];
		}
		// ����һ��ֻ�����ܵ� [] ������������
		const char& operator[](size_t pos) const
		{
			assert(pos < _size);		// �±�Ҫ�Ϸ�

			return _str[pos];
		}

		// size() ����string��Ч�ַ���С
		size_t size() const		// ���޸�ԭ����ľ������� const
		{
			return _size;
		}

		// capacity() ���� string ��Ч�ַ�����
		size_t capacity() const
		{
			return _capacity;
		}

		// ���������ǳ���Ҫ�ĺ���ʵ��
		// reserve �� resize
		// reserve ��Ҫʵ�� ָ����С���ݣ����ǲ������ݣ��Ҳ��ı�ʵ��sizeֻ�ı� capacity
		// C++ ��new ȡ���� realloc���� ��˼·����Ϊ reallocʵ�ֻ�����Щ�鷳
		// ����C++ ��һ�����ֱ�����¿���һ���¿ռ� ������ԭ�ռ�
		void reserve(size_t n)
		{
			if (n > _capacity)			// n �� _capacity��Ÿı�������������
			{
				char* tmp = new char[n + 1];
				strcpy(tmp, _str);		// ԭ�ռ����ݴ����¿ռ���
				delete[] _str;

				_str = tmp;
				_capacity = n;			// _capacity ��С��Ϊn
			}
		}

		// resize ��Ҫʵ�� ��
		// 1. �� n < _size ʱ, _size ��Ϊ n, ����Ҫ�� _size λ�÷���'\0' �ض�ԭ�ַ���
		// 2. �� n > _size ʱ, _size ��Ϊ n, ������ԭ����֮�������Ŀռ��ʼ��Ϊ '\0' �� ָ���ַ�
		//	  
		// �ܽ�, resize �ı䵱ǰ�ַ�����Ч����, 
		// n �ȵ�ǰ����С, ��ض��ַ���;
		// n �ȵ�ǰ���ȴ�,��������С, ������, ֻ����Ч�ַ������ȸ�Ϊ n
		// n �ȵ�ǰ���ȴ�,����������, ������,_capacity��_size����Ҫ��Ϊn, ���� _capacityҪ >= n
		// ����, �ı� �ַ������Ȼ�ı��ԭ�ַ����Ŀռ�
		void resize(size_t n, char ch = '\0')
		{
			if (n < _size)		// �ض��ַ���
			{
				_size = n;
				_str[_size] = '\0';
			}
			else
			{
				if (n > _capacity)		//n �� _capacity��ʱ, ��Ҫ���� 
				{
					reserve(n);
				}

				for (size_t i = _size; i < n; i++)		// �� _size λ������ʼ�� �� n-1, ��Ϊ nλ��, ��Ҫ����'\0' ��ʾ�ַ�������
				{
					_str[i] = ch;
				}
				_size = n;;
				_str[n] = '\0';
			}
		}

		// β���ַ�����  push_back
		// ��������������
		void push_back(char ch)
		{
			if (_size == _capacity)
			{
				reserve(_capacity == 0 ? 4 : _capacity * 2);	// �ж�_capacity�Ƿ�Ϊ���� ��ֹ���ַ��� ���ݴ���
			}

			_str[_size] = ch;
			_size++;
			_str[_size] = '\0';		// β���꣬�ǵò�'\0'

			/*insert(_size, ch);*/	// ���� insert
		}

		void append(const char* str)
		{
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(_capacity == 0 ? len : _capacity + len);
			}

			strcpy(_str + _size, str);	// �� _str + _size λ�ÿ�ʼ���� str
			_size = _size + len;
			// strcpy �´�� �ַ���ĩβ�� '\0' ���Բ����ֶ���'\0'

			/*insert(_size, str);*/	// ���� insert
		}

		// insert ��ָ��λ�ò��� �ַ����ַ���������
		// insert ��ʵ����һ���ǳ���Ҫ�Ĳ������: Ų������
		// �����ǲ���ʲô, ֻҪ����β�嶼��ҪŲ������
		// ����ǲ���һ���ַ����� posλ�ü���������ݣ�����Ҫ���Ų�� 1 λ
		// ������ַ��������, ����Ҫ���Ų�� n λ
		// 
		// ����ʵ���ַ��Ĳ���
		string& insert(size_t pos, char ch)
		{
			assert(pos <= _size);		// posλ�� ��Ҫ�Ϸ�

			if (_size == _capacity)
			{
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}

			size_t end = _size + 1;
			// 1. endλ���� _size+1, Ų�����ݵĲ�����ǽ� end-1 λ�õ�����һ��һ��Ų��end
			// 2. ��� endλ���� _size, ��Ų�����ݵĲ����ǽ� endλ�õ����� Ų�� end+1
			// (���ַ��� �� '\0' ��ʼŲ��)
			// ���濴 ���ַ�������һ�������� ���end�ĳ�ʼλ����_size, ��Ҫ���⴦�� posλ��Ϊ0�����
			// ��Ϊ ��posλ��Ϊ��ʱ, ������ end�Ľ���λ��Ӧ���� -1, ���� end�� size_t ���͵�����, ����end��Զ����Ϊ-1, ������ѭ��
			// �� ����һ �������������������һ ��� posΪ0, ѭ������ʱ end == pos, ���˳�ѭ��
			while (end > pos)
			{
				_str[end] = _str[end - 1];
				end--;
			}
			_str[pos] = ch;
			_size++;

			return *this;
		}
		// insert �����ַ���ʵ��
		// insert�����ַ��� ���ݾͲ���һλһλ�����Ų����
		// 
		string& insert(size_t pos, const char* str)
		{
			assert(pos <= _size);

			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(_capacity == 0 ? len : _capacity + len);
			}

			size_t end = _size + len;
			while (end > pos + len - 1)		// ���Ի�ͼ�ж�һ��end����λ��
			{
				_str[end] = _str[end - len];
				end--;
			}
			strncpy(_str + pos, str, len);		// ʹ��strncpy ���Է�ֹ�ַ�����'\0' Ҳ��������ȥ
			_size += len;

			return *this;
		}
		// insert ���� ����  ֱ�Ӹ���insert�����ַ����Ϳ���
		string& insert(size_t pos, const string& s)
		{
			assert(pos <= _size);

			insert(pos, s._str);

			return *this;
		}
		/*ʵ���� insert֮��, append �� push_back ����ֱ�Ӹ��� insert*/


		// ����ʵ�� += �����ض�����ֱ�Ӹ��� append ���� push_back ���� insert
		string& operator+=(const string& s)
		{
			insert(_size, s);		// += ������ insert
			return *this;
		}
		string& operator+=(const char* str)
		{
			append(str);			// += �ַ������� append
			return *this;
		}
		string& operator+=(char ch)
		{
			push_back(ch);		// += �ַ� ���� push_back
			return *this;
		}

		// earse ��������, ��posλ�� ���ɾ��ָ������
		// ���������ָ������, ��� pos ���ɾ��
		string& earse(size_t pos, size_t len = npos)
		{
			assert(pos < _size);
			
			// ����posλ�����ɾ���ĳ��ȴ��� �ַ�������ʱ, ���ÿ���Ų������ ֱ���޸� _size �� ��β�� '\0'
			if (len == npos || pos + len >= _size)		
			{
				_str[pos] = '\0';
				_size = pos;
			}
			else
			{
				size_t begin = pos + len;
				while (begin <= _size)
				{
					_str[begin - len] = _str[begin];
					begin++;
				}
				_size -= len;
			}

			return *this;
		}

		// find �������ַ�Ȼ�󷵻���λ��
		// �ҵ��ͷ���λ�� �Ҳ����ͷ��� npos
		size_t find(char ch, size_t pos = 0)
		{
			assert(pos < _size);

			for (; pos < _size; pos++)
			{
				if (_str[pos] = ch)
					return pos;
			}

			return npos;
		}

		// ����find���ַ���
		size_t find(const char* str, size_t pos = 0)
		{
			assert(pos < _size);

			//ֱ����strstr()���ַ����ĵ�ַ���ҵ��ͷ����ַ������ַ��ĵ�ַ���Ҳ����ͷ��ؿ�ָ��
			// �ҵ��Ļ�  �ҵ���str�ĵ�ַ - ԭ�ַ����ĵ�ַ �����ַ������ַ�����string�е�λ��
			const char* ps = strstr(_str + pos, str);
			if (ps != nullptr)
			{
				return ps - _str;
			}

			return npos;
		}

		void clear()
		{
			resize(0);
		}

	private:
		char* _str;
		size_t _size;
		size_t _capacity;

		const static size_t npos;
	};

	const size_t string::npos = -1;

	// Ȼ����� �����С�ıȽ���
	// ����֮ǰ�������˼·��ֻ��Ҫʵ�������߼��жϵ����غ����������ж�ֻ��Ҫ���þͿ�����

	// ʵ�� > �� == ����strcmp�����ַ������Ƚ�
	bool operator>(const string& s1, const string& s2)
	{
		return strcmp(s1.c_str(), s2.c_str()) > 0;
	}
	bool operator==(const string& s1, const string& s2)
	{
		return strcmp(s1.c_str(), s2.c_str()) == 0;
	}
	bool operator>=(const string& s1, const string& s2)
	{
		return s1 > s2 || s1 == s2;
	}
	bool operator<(const string& s1, const string& s2)
	{
		return !(s1 >= s2);
	}
	bool operator<=(const string& s1, const string& s2)
	{
		return s1 < s2 || s1 == s2;
	}
	bool operator!=(const string& s1, const string& s2)
	{
		return !(s1 == s2);
	}

	// ���о��� string �������������ȡ������������
	// �����������룬ʵ�ֶ�string���ַ��������
	ostream& operator<<(ostream& out, const string& s)
	{
		// ��ʵ����string����ַ�������
		// ���ַ���ʹ�����ַ�������
		// ����ʹ�� ��Χfor �Ƚϼ� (ʹ��֮ǰ��Ҫʵ�� ������)
		for (auto ch : s)
		{
			out << ch;
		}

		return out;
	}

	// ����ȡ���������� ��ʵ��string���ַ�����д��
	istream& operator>>(istream& in, string& s)
	{
		// C++���� string ��� cin Ч���� ��ȫ��ԭ�ַ����滻Ϊд����ַ��������滻�Ļ���_size
		// �� ���ԭ���� _capacity ����������������ַ����������ݣ�����������ɣ���_capacity ����
		// ���� cin >> �Ĳ��� ���� ' ' �� '\n' Ϊ������־
		// �����������:
		// ���� ʵ�ֵ�Ч���� ��ȫ��ԭ�ַ����滻Ϊд����ַ���, ������Ҫ����� string ���е�����
		// C++���� string �� clear()���� ������ַ������ݵģ�������Ҫ�Լ�ʵ��
		// Ȼ����� ����һ��char���� ��ѭ������������ַ� �� += ��s�ַ�����(�ַ�������clear�����������+=)
		// ����ע�⣬����������ַ������� cin>> ���գ���Ϊ cinҲ���ܽ���' ' �� '\n'�����Իᵼ���޷��жϽ�β
		// ��������ַ� ���õ����������һ����Ա���� get() ����������� �����ַ�û������
		// ��Ϊ ch������Ҫ�����ж�ѭ������������ch��һ�ν����޷���ѭ���ڽ��գ�������Ҫ��ѭ�����һ�ν����ַ�
		// Ȼ��������ѭ���Ϳ�����
		/*char ch;
		s.clear();
		ch = in.get();
		while (ch != ' ' && ch != '\n')
		{
			s += ch;
			ch = in.get();
		}

		return in;*/


		// ���� >> �����أ�����һ�������Ǹ��������Ż��汾
		// ���ʹ���������ַ��� ����һ���ܳ����ַ���, ������һ��һ���ַ����ղ� += ��, �����Ƶ�������ݣ��ͻ����������Դ
		// ������һ���Ż�����
		// ��������ַ����ȴ���һ���ַ������У��ַ�����������һ���� += ��string�ַ�����
		// ����ַ�����δ��ʱ�����Ѿ����л��߽��յ���' ' ��ֱ�ӽ���ǰ���ַ��������� += �� string�ַ�����
		// ���� ������Ч���Ż� ������һ�����ַ���ʱ Ƶ�����ݵ�����
		char ch;
		s.clear();		// ����stringԭ�ַ���
		ch = in.get();
		char tmp[128] = { '\0' };		//�ַ��������ݳ�ʼ��Ϊ '\0'����С�ʺϾͿ���
		size_t i = 0;			// ���������ַ������±긳ֵ���Լ���¼�ַ��������ַ�����
		while (ch != ' ' && ch != '\n')
		{
			tmp[i++] = ch;
			if (i == 127)	// �ַ������СΪ 128, �����ַ����Ļ�, ��������Ч�ַ������� 127, ��Ϊ���һλ'\0'��Ϊ������־
			{
				// �ַ�����һ��, �ͽ��ַ������е����� += ��string����, Ȼ������ַ�����
				s += tmp;
				memset(tmp, '\0', 128);	// ʹ�� memset ���ַ��������пռ�����Ϊ '\0' �Է��´� += �����
				i = 0;
			}
			ch = in.get();
		}
		s += tmp;		// ѭ������, �ٽ��ַ������л��е����� += ��string����

		return in;
	}

	// �������˽�г�Ա�����⺯�� ����Ϊ��Ԫ����
}
