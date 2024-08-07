#pragma once

#include <iostream>
#include <cassert>

namespace July
{
	template<class Iterator, class Ref, class Ptr>
	struct reverseIterator
	{
		typedef reverseIterator<Iterator, Ref, Ptr> Self;

		Iterator _it;

		// ���캯��
		// ����������ĳ�Ա��������������������캯�������ڶ���ʵ����ʱ���õ�
		// ��������������� ��� rbegin() �� rend() �У����캯���Ĵ���Ҳ��
		// ��Ҫ������������ĳ�Ա������ʼ��Ϊ ����ö������Ӧ�����������
		reverseIterator(Iterator it)
			:_it(it)
		{}

		// * ����������
		// ������ *�����÷�������������ṩ�Է���ֵ���޸ģ�����Ҫ��&����
		// ���ص��� ��ǰλ�������������ǰһλ�Ľ����ã�ʵ�ʷ��ص���ʲô��Ҫ�� �����������ʵ��
		// Ϊʲô�������������ǰһλ��
		// �� rbegin() ��ȡ�ķ��������Ϊ������ȡ�������һ��Ԫ�ص���һλ�����������������ݷ�Χ�ڣ����ֱ�ӶԵ�ǰλ�õ���������������ã��ᷢ������ǰһλ���������������ݷ�Χ
		Ref operator*()
		{
			Iterator tmp(_it);

			return *(--tmp);
		}

		// -> ����������
		// -> һ������ָ����������Է���Ӧ����ָ��
		// ���ص�Ҳ�� ��ǰλ�������������Ǩ��λ�ý����õ�ȡ��ַ
		Ptr operator->()
		{
			return &(operator*()); 		// ���� *����
		}
		// ǰ��++����
		// ǰ�� ++\-- ��ֱ�Ӹı�ԭֵ���Բ���Ҫ�������ҷ��صĶ����޸ĺ�ĵ�����
		// ���� ++\-- �����޸�ǰ�ĵ�������������Ҫ�����޸�ǰ�ĵ�����
		Self& operator++()
		{
			--_it; 			// ��������� ++��ʵ�� --

			return *this;
		}
		// ����++
		Self operator++(int)
		{
			Self tmp(_it); 		// �����޸�ǰ�ĵ�����
			--_it;

			return tmp;  			// ������ʱ�������� ��ֵ����
		}
		// ǰ��--
		Self& operator--()
		{
			++_it; 			// ��������� --��ʵ�� ++

			return *this;
		}
		// ����--
		Self operator--(int)
		{
			Self tmp(_it);
			++_it;

			return tmp;
		}

		// != �� == ������
		bool operator!=(const Self& sit)
		{
			return _it != sit._it;
		}
		bool operator==(const Self& sit)
		{
			return _it == sit._it;
		}
	};

	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		typedef reverseIterator<iterator, T&, T*> reverse_iterator;		// �� const���������
		typedef reverseIterator<const_iterator, const T&, const T*> const_reverse_iterator;		//  const���������

		// ���캯��
		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstoage(nullptr)
		{}

		// vector����ʵ���˿�����n��val (val ��T���͵�����) �������Ĺ��캯��
		vector(size_t n, const T& val = T())
			: _start(nullptr)
			, _finish(nullptr)
			, _endofstoage(nullptr)
		{
			reserve(n);		// ��reserve
			while (_finish < _endofstoage)
			{
				push_back(val);
			}
		}
		// Ϊ��� vector(int, int) ���ʹ���ʱ
		// ��ȥ���� template <class InputIterator>
		//        vector(InputIterator first, InputIterator last)������
		// ��Ϊ��int int ����ͬ���͵Ĵ��Σ�����ʱ��������������ʺϵĴ������ͣ����������һ���������� int, T�Ĺ��캯��
		// �򴫲� int int ����ʱ���ᷢ�������� ��n��val (val ��T���͵�����) �������Ĺ��캯��
		vector(int n, const T& val = T())
			: _start(nullptr)
			, _finish(nullptr)
			, _endofstoage(nullptr)
		{
			reserve(n);		// ��reserve
			while (_finish < _endofstoage)
			{
				push_back(val);
			}
		}

		// vector����ʵ���� ���������� iterator �������Ĺ��캯��
		template <class InputIterator>
		vector(InputIterator first, InputIterator last)
			: _start(nullptr)
			, _finish(nullptr)
			, _endofstoage(nullptr)
		{
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		void swap(vector<T>& v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_endofstoage, v._endofstoage);
		}

		// = ��ֵ���غ������ִ�д��
		// �� string��ֵ�����ִ�д��һ�������ô�ֵ���ο���һ����ʱ����������
		// ֱ�ӽ�this�����ʱ������������
		vector<T>& operator=(vector<T> v)
		{
			swap(v);
			return *this;
		}

		/*vector<T>& operator=(vector<T> v)
		{
			swap(v);

			return *this;
		}*/
		//  �ִ�д�Ŀ������� ��ʹ�������Ǹ� �� iterator ���캯�� �������tmp
		//  ���� this �� tmp ��swap��������, ����ֻ������������Ҫ�Լ�ʵ��swap�����ܵ��ÿ�swap
		vector(const vector<T>& v)
			: _start(nullptr)
			, _finish(nullptr)
			, _endofstoage(nullptr)
		{
			vector<T> tmp = vector(v.begin(), v.end());
			swap(tmp);
		}

		// ��������
		~vector()
		{
			if (_start)
			{
				delete[] _start;
				_start = _finish = _endofstoage = nullptr;
			}
		}

		void clear()
		{
			delete[] _start;
			_start = _finish = nullptr;
		}

		iterator begin()
		{
			return _start;
		}
		iterator end()
		{
			return _finish;
		}
		const_iterator begin() const
		{
			return _start;
		}
		const_iterator end() const
		{
			return _finish;
		}
		reverse_iterator rbegin()
		{
			return _finish;
		}
		reverse_iterator rend()
		{
			return _start;
		}
		const_reverse_iterator rbegin() const
		{
			return _finish;
		}
		const_reverse_iterator rend() const
		{
			return _start;
		}

		size_t size() const
		{
			return _finish - _start;
		}
		size_t capacity() const
		{
			return _endofstoage - _start;
		}

		// reserve ����
		void reserve(size_t n)
		{
			// ��Ҫ�ȼ�¼ԭ��size�Ĵ�С
			// ��Ϊ vector��ʵ�ֿ�� ��ʾ���顢�����С�����������ı������� iterator
			// vector����Ч��������������Ӧ���� _finish - _start�����ǿ����¿ռ�֮��_finish ���Ϊ nullptr���޷���� 
			// ���� vector������֮�󣬲����� stringһ��ֱ����Ԫ�ص�ַ+_size �ı��¶���Ĵ�С
			// ������Ҫ��ǰ��¼ԭ�������ݴ�С������֮����ʹ�� _start + sz ��� _finish; 
			size_t sz = size();
			if (_start + n > _endofstoage)
			{
				T* tmp = new T[n];
				if (_start)	// ԭ������������ ����Ҫ�������ݵ������ݿռ��У������ͷ�ԭ�������ݿռ�
				{
					// ������ memcpy ���ܻᷢ����ǳ���������⣬������ TΪ��Ҫ��̬�����ڴ������ʱ
					// ���黭ͼ�����Է���
					// ���Բ����� memcpyֱ�ӿ������ݣ�ֻ�ܶԵ����ռ����ݣ��������и��� ��ֵ
					//  
					// memcpy(tmp, _start, size() * sizeof(T));
					for (int i = 0; i < size(); i++)
					{
						tmp[i] = _start[i];
					}
					delete[] _start;
				}
				// �������ݿռ��_start
				_start = tmp;
			}
			// ���� _finish = _finish - _start,��Ϊ��ǰ��_finish �� _start ����ͬһƬ�ռ� 
			_finish = _start + sz;
			_endofstoage = _start + n;
		}
		// T() ���������������������ڽ����䶨���һ��
		// T��ģ�������Ҳ����ζ����C++�г����Զ����������⣬��ʵ��������Ҳ�й��캯�����ɼ��Ѳ���
		void resize(size_t n, T val = T())
		{
			if (n > capacity())
			{
				reserve(n);
			}
			else if (n > size() && n < capacity())
			{
				while (_finish < _start + n)
				{
					*_finish = val;
					_finish++;
				}
			}
			else
			{
				_finish = _start + n;
			}
		}

		void push_back(const T& val)
		{
			if (_finish == _endofstoage)
			{
				size_t newCapacity = !_endofstoage ? 4 : capacity() * 2;
				reserve(newCapacity);
			}
			*_finish = val;
			_finish++;
		}

		void pop_back()
		{
			if (_finish > _start)
				--_finish;
		}

		T& operator[](size_t pos)
		{
			assert(_start + pos < _finish);

			return _start[pos];
			//return *(_start + pos);
		}
		const T& operator[](size_t pos) const
		{
			assert(_start + pos < _finish);

			return _start[pos];
			//return *(_start + pos);
		}

		// vector �е� insert �� string�е� insert������ͬ��ʹ�õ��� iterator ��Ϊ����λ�õ�ַ,���з���ֵ ҲΪ iterator ���͵�
		// vector�еķ���ֵ��Ϊ�˽�� ������ʧЧ������ �����ٿ���
		// insert��ʵ�� ��Ҫ�ƶ����ݣ����� iterator��Ϊposλ�ã��Ϳ��Բ�����stringһ��ע�� size_t pos = -1������
		iterator insert(iterator pos, const T& val)
		{
			assert(pos >= _start && pos <= _finish);

			if (_finish == _endofstoage)
			{
				size_t posForS = pos - _start;
				size_t newCapacity = !_endofstoage ? 4 : capacity() * 2;
				reserve(newCapacity);

				pos = _start + posForS;
			}

			iterator end = _finish - 1;
			while (end >= pos)
			{
				*(end + 1) = *end;
				end--;
			}

			*pos = val;
			_finish++;

			// ģ��ʵ��STL���е� vector������Ҫ��ܡ�����������ֵ����ͬ
			// STL��vector��insert ���ص�λ���ǵ�һ���²���Ԫ�ص�λ�ã��� pos
			return pos;
		}

		iterator erase(iterator pos)
		{
			assert(pos >= _start && pos <= _finish);

			iterator begin = pos + 1;
			while (begin < _finish)
			{
				*(begin - 1) = *begin;
				begin++;
			}

			_finish--;

			// vector���е� erase���� ���һ����ɾ��Ԫ�ص���һ��Ԫ�ص�λ�ã���ʵ����posλ��
			return pos;
		}

		vector<vector<int>> generate(int numRows)
		{
			vector<vector<int>> vv;
			vv.resize(numRows);
			for (int i = 0; i < vv.size(); i++)
			{
				// ÿ��vector<int> ��size�������ҳ�ʼ��Ϊ��
				vv[i].resize(i + 1, 0);

				// ÿ�еĵ�һ�������һ��Ԫ�ظ�ֵΪ1
				*(vv[i].begin()) = 1;
				*(vv[i].end()) = 1;
			}

			for (int i = 0; i < vv.size(); i++)
			{
				for (int j = 0; j < vv[i].size(); j++)
				{
					// vector<int> ���������ݱ���ʼ��Ϊ0
					// ���������Ѿ�������������������1
					// ������ֵΪ0����Ҫ����
					if (vv[i][j] == 0)
					{
						vv[i][j] = vv[i - 1][j - 1] + vv[i - 1][j];
					}
				}
			}

			return vv;
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _endofstoage;
	};
}
