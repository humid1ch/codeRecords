#pragma once
#include <vector>
#include <string>
#include <iostream>
using std::string;
using std::pair;
using std::make_pair;
using std::vector;

namespace July {

	// ö�ٱ��� ��ʾ״̬
	enum State {
		EMPTY,
		EXIST,
		DELETE
	};

	// ��ϣ����������
	template<class K, class V>
	struct HashData {
		pair<K, V> _data;
		State _state = EMPTY;
	};

	template<class K, class V>
	class HashTable {
	public:
		typedef HashData<K, V> Data;

		bool insert(const pair<K, V>& kv) {
			if (_tables.size() == 0 || _n * 10 / _tables.size() >= 7) {				// �� ��ϣ��������Ϊ0, �� ��ϣ���ڴ洢�����ݸ����Ѿ��ﵽ����������70% ��������
				size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;			// ������Ϊ10 �� ԭ���� 2��

				// ��ϣ�����ݲ����� ������ԭ���Ļ����ϸı�������ô��, ������Ҫ���µ�������Ϊ capacity, �Թ�ϣ���ڵ����ݽ�������ӳ��
				HashTable<K, V> newHashTable;
				newHashTable._tables.resize(newSize);				// ����vector��resize�ӿڽ��¹�ϣ���������ΪnewSize

				// ����ԭ���Ĺ�ϣ��, ��ԭ���Ĺ�ϣ���ڵ�����ӳ�䵽 �µĹ�ϣ����
				for (auto& e : _tables) {
					if (e._state == EXIST) {						// �� ԭ��ϣ���д��ڵ����� ӳ�䵽�¹�ϣ����
						newHashTable.insert(e._data);
					}
				}

				// ��������ӳ�����֮��, �� ������ϣ���_tables ���н���
				// �Ϳ��Խ���ǰ�Ĺ�ϣ������Ϊ�µĹ�ϣ��
				_tables.swap(newHashTable._tables);
			}

			size_t startI = kv.first % _tables.size();				// ��ʼ̽���λ��

			size_t hashI = startI;
			size_t i = 1;
			while (_tables[hashI]._state == EXIST) {				// ������ϣ��ͻ�ͼ������̽��
				hashI = startI + i;
				i++;
				hashI %= _tables.size();							// ������Ҫ̽���λ��
			}

			// ̽�⵽��λ��֮��
			_tables[hashI]._data = kv;
			_tables[hashI]._state = EXIST;
			_n++;

			return true;
		}

		Data* find(const K& key) {
			if (_tables.size() == 0) {
				return nullptr;						// ��ϣ��Ϊ�� ����ʧ��
			}

			size_t startI = key % _tables.size();

			size_t hashI = startI;
			size_t i = 1;
			while (_tables[hashI]._state != EMPTY) {				// �� ��ǰ̽��λ�ò�Ϊ��ʱ, ����ѭ�� ��������
				if (_tables[hashI]._state != DELETE && _tables[hashI]._data.first == key) {
					// �� ��ǰ̽��λ������û�б�ɾ�� �� ������������ʱ �ͷ��ص�ǰλ�õ�ַ
					return &_tables[hashI];
				}

				// ��ǰλ�õ����� ���� ���ҵ����� �͸���̽��λ�� ��������
				hashI = startI + i;
				i++;
				hashI %= _tables.size();
			}

			return nullptr;					// �߳�ѭ�� �ͱ�ʾû���ҵ� 
		}
		
		bool erase(const K& key) {
			Data* ret = find(key);
			if (Data) {
				ret->_state = DELETE;
				_n--;
				
				return true;
			}
			else {
				return false;
			}
		}	

	private:
		vector<Data> _tables;
		size_t _n = 0;
	};
}


namespace Bucket {
	template<class Key>
	struct defaultHashFcn {							// Ĭ�ϵķº���, �������ֱ��ת���� size_t������
		size_t operator()(const Key& k) {
			return (size_t)k;
		}
	};
	
	template<>
	struct defaultHashFcn<string> {					// Ĭ�Ϸº��� ���string���͵��ػ�
		size_t operator()(const string& str) {
			size_t hash = 0;
			for (auto c : str) {
				hash = hash * 131 + c;
			}

			return hash;
		}
	};

	/*template<class Key>
	struct stringHashFcn {
		size_t operator()(const Key& str) {
			size_t hash = 0;
			for (auto c : str) {
				hash = hash * 131 + c;
			}

			return hash;
		}
	};*/
	
	// ���ݽڵ� �ṹ
	template<class K, class V>
	struct HashNode {
		pair<K, V> _data;
		HashNode<K, V>* _next;

		HashNode(const pair<K, V>& kv) 
			:_data(kv)
			,_next(nullptr)
		{}
	};

	template<class K, class V, class HashFcn = defaultHashFcn<K>>
	class HashTable {
		typedef HashNode<K, V> Node;
	public:
		// ���� �����ڴ洢���������Զ����, ������Ҫ�������������������ͷ�
		~HashTable() {
			// ������ϣͰ �ͷŽڵ�
			for (int i = 0; i < _tables.size(); i++) {
				Node* cur = _tables[i];
				while (cur) {
					Node* next = cur->_next;
					delete cur;
					cur = next;
				}

				_tables[i] = nullptr;
			}
		}

		bool insert(const pair<K, V>& kv) {
			if (find(kv.first)) {
				return false;						// ��� �����ظ��洢����
			}
			HashFcn hf;
			// ���� ����������== 1ʱ ����
			/*if (_tables.size() == _n) {
				size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;

				// ���ַ��� �ǳ���Ӱ��Ч��
				// ��Ϊ���ַ��� ������ insert�ӿ�, �ڿ�ɢ�еĴ�����ʹ��insert�ӿ� �ᴴ���µĽڵ�, �������� 
				// ����, ���ڴ������µ� ��ϣͰ, ������ ����ѭ��������ʱ�� �µ���ʱ�Ĺ�ϣͰ�ᱻ�ͷ� �ͷ���Ҫ������ϣͰ ����Ӱ��Ч��
				HashTable<K, V> newHashTable;
				newHashTable._tables.resize(newSize, nullptr);

				// ����ԭ��ϣͰ ���¹�ϣͰ������ӳ��
				for (int i = 0; i < _tables.size(); i++) {
					Node* cur = _tables[i];
					while (cur) {
						newHashTable.insert(cur->_data);				// �¹�ϣͰ�в��� ��ǰ�ڵ�Ĵ洢����
						cur = cur->_next;
					}
				}

				_tables.swap(newHashTable._tables);
			}*/

			// ��������ݷ��� �ǳ���Ӱ��Ч���ҿ�������
			// �������һ�ַ���, ������insert, �������µĹ�ϣͰ �������µĽڵ� Ҳ����Ҫ�ͷ��µĿռ�
			// ���� ֱ�ӽ� �������еĽڵ� ���¼���ӳ�䵽�µ�������, �ٽ� ��ϣͰ���������Ϊ������Ϳ�����
			if (_tables.size() == _n) {
				size_t newSize = _tables.size() == 0 ? 10 : _tables.size() * 2;

				vector<Node*> newTable;
				newTable.resize(newSize, nullptr);
				for (int i = 0; i < _tables.size(); i++) {
					Node* cur = _tables[i];
					while (cur) {
						Node* next = cur->_next;				// ����� ʹcur->_next ��� �����д洢��ͷ�ڵ�, ���� ��һ���ڵ���ǰ��¼

						size_t hashI = hf(cur->_data.first) % newTable.size();					// ����������������� ��Ӧ�Ĺ�ϣ��ַ
						cur->_next = newTable[hashI];										// ����Ľڵ� �� newTable�洢��ͷ�ڵ�������
						newTable[hashI] = cur;

						cur = next;						// ���� cur
					}

					_tables[i] = nullptr;				// ԭ����������ѱ�ת�Ƶ��������� ��ԭ����������ÿ�
				}

				_tables.swap(newTable);
			}
			
			// �������, �����������
			//size_t hashI = kv.first % _tables.size();
			size_t hashI = hf(kv.first) % _tables.size();
			// �ҵ����������еڼ���λ�ò�������
			// Ȼ�����ͷ��
			Node* newNode = new Node(kv);						// ��kv���ݴ����µĽڵ�
			newNode->_next = _tables[hashI];					// �������д洢�������ͷ�ڵ��������½ڵ�֮��
			_tables[hashI] = newNode;							// �½ڵ�洢��������

			_n++;

			return true;
		}

		Node* find(const K& key) {
			if (_tables.size() == 0) {
				return nullptr;
			}

			//size_t hashI = key % _tables.size();
			HashFcn hf;
			size_t hashI = hf(key) % _tables.size();
			Node* cur = _tables[hashI];
			while (cur) {
				if (cur->_data.first == key) {
					return cur;									// �ҵ����ؽڵ�
				}

				cur = cur->_next;
			}

			return nullptr;
		}

		bool erase(const K& key) {
			/*
			// һ��˼·
			HashFcn hf;
			size_t hashI = hf(key) % _tables.size();
			Node* cur = _tables[hashI];
			Node* prev = nullptr;
			while (cur) {
				if (cur->_data.first == key) {
					if (cur == _tables[hashI]) {
						//	��Ҫɾ���Ľڵ��� �����д洢�����ͷ�ڵ�
						_tables[hashI] = cur->_next;
					}
					else {
						prev->_next = cur->_next;
					}
					delete cur;

					return true;
				}
				prev = cur;
				cur = cur->_next;
			}*/
			// ����˼·, ��ֹ��ȡ prev �Լ� ʹ��next��������Ϊ�ж�����
			// ���������滻:
			//		Ҫɾ�� cur�ڵ������, �ͽ�cur������ �� cur->next������ ���ǵ�, �ٽ� cur�� cur->next->next ��������֮�� �ͷ�cur->next�ڵ�
			// ��ֻ��һ��˼·, ������˵���ַ�������, ��ʵ�����ַ�����û��������һ�ַ�����, ��Ϊ ���ַ��� һ����������ݵĿ���
			// ���������ݹ��� ����ʱ, ��������
			//size_t hashI = key % _tables.size();
			HashFcn hf;
			size_t hashI = hf(key) % _tables.size();
			Node* cur = _tables[hashI];
			while (cur) {
				if (cur->_data.first == key) {
					if (cur->_next == nullptr) {
						// �����ǰ�ڵ��ǵ������е����һ���ڵ�ʱ
						// ��û����һ������, ���Բ�������һ���ڵ�������ݵ����
						// ���ǿ����� ͷ�ڵ�������ݵ����, Ȼ��ͷɾ
						Node* head = _tables[hashI];
						cur->_data = head->_data;
						_tables[hashI] = head->_next;
						delete head;
						head = nullptr;
					}
					else {
						cur->_data = cur->_next->_data;
						cur->_next = cur->_next->_next;
						delete cur->_next;
					}
					_n--;

					return true;
				}
				cur = cur->_next;
			}

			return false;
		}

	private:
		vector<Node*> _tables;
		size_t _n = 0;					// ��¼����洢���ݵ�����
	};
}	
