#pragma once
#include "RBTree.h"

namespace July {
	template<class Key>
	class set {
		typedef Key key_type;
		typedef Key value_type;

		// ȡkeyֵ�ķº���
		struct SetKeyOfValue {
			const key_type& operator() (const value_type& key) {
				return key;
			}
		};

	public:
		// ��װ������ĵ�����
		typedef typename RB_Tree<key_type, value_type, SetKeyOfValue>::const_iterator iterator;
		typedef typename RB_Tree<key_type, value_type, SetKeyOfValue>::const_iterator const_iterator;
		// ���� ��������Ҫ�����ⶨ�� ������ҪtypedefΪ ���õ�

		iterator begin() const {
			return _tree.begin();
		}

		iterator end() const {
			return _tree.end();
		}

		pair<iterator, bool> insert(const value_type& v) {
			auto ret = _tree.insert(v);

			return pair<iterator, bool>(iterator(ret.first._node), ret.second);
		}

		iterator find(const key_type& key) {
			return _tree.find(key);
		}

	private:
		RB_Tree<key_type, value_type, SetKeyOfValue> _tree;
	};


	void test_set1()
	{
		set<int> s;
		s.insert(8);
		s.insert(6);
		s.insert(11);
		s.insert(5);
		s.insert(6);
		s.insert(7);
		s.insert(10);
		s.insert(13);
		s.insert(12);
		s.insert(15);

		set<int>::iterator it = s.begin();
		while (it != s.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : s)
		{
			cout << e << " ";
		}
		cout << endl;
	}

}

