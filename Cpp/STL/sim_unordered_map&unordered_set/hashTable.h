#pragma once
#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::pair;
using std::make_pair;

namespace July {

	template<class Key>
	struct defaultHashFunc {
		size_t operator()(const Key& key) {
			return (size_t)key;
		}
	};
	template<>
	struct defaultHashFunc<string> {
		size_t operator()(const string& str) {
			size_t hash = 0;
			for (auto c : str) {
				hash = hash * 131 + c;
			}

			return hash;
		}
	};

	// ���ݽڵ�
	template<class T>
	struct hashNode {
		T _data;
		hashNode<T>* _next;

		hashNode(const T& data)
			:_data(data)
			,_next(nullptr)
		{}
	};

	template<class Key, class T, class KeyOfT, class hashFunc>
	class hashTable;
	// ���������� ����ʹ���˹�ϣ�����ָ��, ��ϣ������ͬ������ʹ���˵�������, ���� ��Ҫ�ڵ�������ǰ������ϣ����
	// ����������� ������ ��ϣ������, ����Ҫ�ڹ�ϣ����ǰ������������
	
	template<class Key, class T, class KeyOfT, class hashFunc>
	class __hashTableIterator {
		typedef hashNode<T> Node;
		typedef __hashTableIterator<Key, T, KeyOfT, hashFunc> Self;
	public:
		__hashTableIterator() {}

		__hashTableIterator(Node* node, hashTable<Key, T, KeyOfT, hashFunc>* pht)
			:_node(node)
			,_pht(pht)
		{}

		Self& operator++() {
			// ��ϣ�������++ ��ͷ��β���ƶ������� �ӵ�һ��Ͱ��ͷ�ڵ㿪ʼ �� ���һ��Ͱ��β�ڵ�
			// ���Ե�����ָ��Ͱ�з����һ�����ݽڵ��ĳ���ڵ�ʱ, ++ ֱ������ _next�ڵ���
			// ����������ǰ�ڵ��� Ͱ�����һ�����ݽڵ�, �� ++֮�� ������Ӧ��ָ����һ��Ͱ��ͷ�ڵ�
			// ����������һ��Ͱ, Ҳ�ǵ������� ����Ҫ��һ������
			if (_node->_next) {				// _node->next��Ϊ�� ����ʾ _node��Ϊ���һ�����ݽڵ�
				_node = _node->_next;
			}
			else {
				// �� ������ָ��Ͱ�����һ�����ݽڵ�ʱ, ����Ҫ����һ��Ͱ
				hashFunc hf;
				KeyOfT kot;

				// ���㵱ǰ����������Ͱλ��
				size_t hashI = hf(kot(_node->_data)) % _pht->_tables.size();
				// ������Ҫ���� �������ڵ����ڹ�ϣ���˽�г�Ա���� _tables. ���� ��Ҫ�� ������������Ϊ��ϣ�������Ԫ��
				hashI++;
				for (; hashI < _pht->_tables.size(); hashI++) {		// �������Ͱλ��
					if (_pht->_tables[hashI]) {							// ����ҵ���һ���ǿ�Ͱ��λ��, ��¼Ͱ��ͷ�ڵ�, ����ѭ��
						_node = _pht->_tables[hashI];
						break;
					}
				}

				// ��� �����ѭ����������, hashI �� == _pht->_tables.size()
				// ��ʱ ��˵�� ������ָ���λ�õĺ���û�зǿ�Ͱ�� �� ������ָ��ľ��ǹ�ϣ�������һ�����ݽڵ�
				// ��ʱ�� ++ ��ʹ���������� nullptr
				if (hashI == _pht->_tables.size()) {
					_node = nullptr;
				}
			}

			return *this;				// ���ظ��º�ĵ�����
		}
		
		Node& operator*() {
			return _node->_data;
		}

		Node* operator->() {
			return &_node->_data;
		}

		bool operator!=(const Self& htIt) const {
			return _node != htIt._node;
		}

		bool operator==(const Self& htIt) const {
			return _node == htIt._node;
		}
	private:
		Node* _node;
		hashTable<Key, T, KeyOfT, hashFunc>* _pht;							// ��Ϊ������ ��Ҫ�ڹ�ϣ�����ж��ƶ�, ���Ե�������Ҫ�洢��ϣ���ָ��
	};

	// ��ϣ�� ģ������ֱ�Ϊ Key:�ؼ�������  T:��������	 KeyOfT:������ȡ�ؼ��ֵķº���  hashFunc:����Key��ϣֵ�ķº���
	template<class Key, class T, class KeyOfT, class hashFunc>
	class hashTable {
		typedef hashNode<T> Node;
		
		template<class Key, class T, class KeyOfT, class hashFunc>
		friend class __hashTableIterator;
	public:
		typedef __hashTableIterator<Key, T, KeyOfT, hashFunc> iterator;

		iterator end() {
			return iterator(nullptr, this);
		}

		iterator begin() {
			// �ҵ���һ��Ͱ��ͷ�ڵ� ʵ���������� ������
			for (size_t i = 0; i < _tables.size(); i++) {
				Node* cur = _tables[i];
				if (cur) {
					return iterator(cur, this);
				}
			}

			return end();
		}

		~hashTable() {
			for (size_t i = 0; i < _tables.size(); i++) {
				Node* cur = _tables[i];
				while (cur) {
					Node* next = cur->_next;
					delete cur;

					cur = next;
				}
				
				_tables[i] = nullptr;
			}
		}
		
		// ����������ص��� �������ݵ����ڽڵ�ĵ�����, �Լ�������
		// ���� ��pair������ֵ, first�洢������, second �洢bool
		//bool insert(const T& data) {
		pair<iterator, bool> insert(const T& data) {
			hashFunc hf;						// ʵ���� ���� Key �Ĺ�ϣֵ�ķº����� ����
			KeyOfT kot;							// ʵ���� ȡ�ؼ��ֵķº����� ����

			/*if (find(kot(data))) {					// �жϹ�ϣ�����Ƿ� �Ѿ����ڴ�����
				return false;
			}*/
			iterator pos = find(kot(data));
			if (pos != end())
			{
				return make_pair(pos, false);
			}


			// �� �������� == 1 ʱ, ���ݹ�ϣ��
			if (_tables.size() == _n) {
				size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;

				vector<Node*> newTables;
				newTables.resize(newSize, nullptr);
				for (int i = 0; i < _tables.size(); i++) {
					Node* cur = _tables[i];
					while (cur) {
						Node* next = cur->_next;

						size_t hashI = hf(kot(cur->_data)) % newSize;
						cur->_next = newTables[hashI];
						newTables[hashI] = cur;

						cur = next;
					}
					_tables[i] = nullptr;
				}

				//	������� �����¾�����
				_tables.swap(newTables);
			}

			// ����Ͱ��λ��
			size_t hashI = hf(kot(data)) % _tables.size();
			
			// �������ݽڵ� ��ͷ����Ͱ��
			Node* newNode = new Node(data);
			newNode->_next = _tables[hashI];
			_tables[hashI] = newNode;

			++_n;

			return make_pair(iterator(newNode, this), true);
		}

		// Node* find(const Key& key)
		iterator find(const Key& key) {
			if (_n == 0) {							// _n Ϊ 0, ��ʾ����û������
				// return nullptr;
				return iterator(nullptr, this);		// ���ص�����
			}

			hashFunc hf;
			KeyOfT kot;

			size_t hashI = hf(key) % _tables.size();
			Node* cur = _tables[hashI];
			while (cur) {
				if (kot(cur->_data) == key) {
					//return cur;
					return iterator(cur, this);
				}

				cur = cur->_next;
			}

			return iterator(nullptr, this);
		}

		bool erase(const Key& key) {
			if (_n == 0) {
				return false;
			}

			hashFunc hf;
			KeyOfT kot;

			size_t hashI = hf(key) % _tables.size();
			Node* cur = _tables[hashI];
			Node* prev = nullptr;
			while (cur) {
				if (kot(cur->_data) == key) {
					if (cur == _tables[hashI]) {					// cur �ǵ�ǰͰ��ͷ�ڵ�
						_tables[hashI] = cur->_next;
					}
					else {
						prev->_next = cur->_next;					// ����prev->_next Ϊ cur->_next
					}

					delete cur;
					--_n;
				}

				prev = cur;
				cur = cur->_next;
			}

			return false;
		}

	private:
		vector<Node*> _tables;
		size_t _n = 0;
	};
}
