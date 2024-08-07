#pragma once

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

template<class K>
struct BinarySearchTreeNode
{
	BinarySearchTreeNode<K>* _left = nullptr;
	BinarySearchTreeNode<K>* _right = nullptr;

	K _key;

	BinarySearchTreeNode(const K& key)
		:_left(nullptr)
		,_right(nullptr)
		,_key(key)
	{}
};

// ��������������߱ȸ�С���ұ߱ȸ���
template<class K>
class BSTree
{
	typedef BinarySearchTreeNode<K> Node;
public:
	BSTree()
		:_root(nullptr)
	{}

	BSTree(const BSTree<K>& tree)
	{
		_root = CopyTree(tree._root);
	}

	// ��ֵ���غ������ִ�д��
	BSTree<K>& operator=(BSTree<K> tree)
	{
		// ��ֵ���� ��������
		// ֱ�ӽ����������� �� ��Ҫ��ֵ���� ������
		std::swap(_root, tree._root);

		return *this;
	}
	// �������� �ݹ�������� ɾ���ڵ�
	// Ϊʲô�Ǻ�������� ��Ϊ ������Ա�֤����ɾ���Ľڵ�ʱ���ýڵ�û���ӽڵ�
	~BSTree()
	{
		DestroyTree(_root);
		_root = nullptr;
	}

	bool Insert(const K& key)
	{
		if (_root == nullptr)			// ��������û�����ݵ�ʱ��
		{
			_root = new Node(key);
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;

		while (cur)
		{
			if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else					// ������������������� ��ͬ������
			{
				return false;
			}
		}
		// �� cur Ϊ��ʱ��ѭ��������curλ��������������Ҫ�����λ�ã��� parent ������ �� �Һ���
		// ȷ������ ���� �Һ��� ��Ҫ�ٱȽ�һ��
		cur = new Node(key);			// curλ�� ��һ���½ڵ�
		if (parent->_key > key)			// �ٽ�curλ���� parent�ڵ���������
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}

		return true;
	}

	//const Node* Find(const K& key)		// ������ؽڵ㣬����Ҫ const���νڵ㣬��Ϊ�����������������޸ģ����ƻ��ṹ
	bool Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key < key)
			{
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				cur = cur->_left;
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	// ɾ���ڵ�Ƚ��Ѹ㣬��Ϊ�����������Ľṹ�� ������С����������
	// ֱ��ɾ���ڵ���ܻ���ɽṹ�ƻ�������ɾ���ڵ���Ҫ����һ��
	// ����Ҫ֪�� ɾ���ڵ���ܻ����������������
	// 1. ��ɾ���ڵ�����������
	//	  �������ֱ��ɾ����Ȼ�󽫸��׽ڵ�� _left �� _right(��Ҫ�жϱ�ɾ�ڵ��Ǹ��׽ڵ�����ӻ����Һ���) �ÿվ�����
	//	  ���˱�ɾ�ڵ�����Һ��Ӹպ��ǿգ�����ֱ�ӽ����׽ڵ��_left �� _right ��Ϊ ��ɾ�ڵ�����Һ��Ӿ���
	// 2. ��ɾ���ڵ�ֻ��������
	//	  ֻ�����������ͱ�ʾ ���׽ڵ���Ҫ���������������� ��ɾ�����֮ǰ Ҫ�� ���׽ڵ�� _left �� _right ���ӵ� ��ɾ�ڵ����������
	// 3. ��ɾ���ڵ�ֻ��������
	//	  �� ��2������෴
	// 4. ��ɾ�ڵ�ͬʱ������������
	//	  ��ʱ��ɾ���ڵ����Ҫ����ķ������ˡ�
	//	  ��ɾ���ڵ�ͬʱ����������������ɾ�����֮���ܸı�����������Ľṹ
	//	  ��ѵ�˼·���ǣ���һ�����ʵĽڵ��뱻ɾ���ڵ㽻��λ�ã�Ȼ����ɾ��������
	//    ������ʵĽڵ���Ҫ���㼸��Ҫ��
	//		1. ����ͬʱӵ����������
	//		2. ����֮�󣬲��ܸı�����������Ľṹ
	//	  ��������������������֮���������ڵ�������ģ�
	//		1. �����������ڵ�
	//		   ��Ϊ ���������Ǳȸ��׽ڵ�С�ģ��������������ڵ����¸��ף����ɿ��Ա�֤ �¸��׽ڵ���������� ��������С
	//		2. ����������С�ڵ�
	//		   ͬ�����������Ǳȸ��׽ڵ��ģ�������������С�ڵ����¸��ף����ɿ��Ա�֤ �¸��׽ڵ���������� ��������С
	//	  
	// ������ɾ����˼·֮�󣬾Ϳ��Ը��� ͼƬ��д�����ˣ�
	//
	bool Erase(const K& key)
	{ 
		Node* parent = nullptr;					// ���ڼ�¼ɾ���ڵ�ĸ��׽ڵ�
		Node* cur = _root;						// ���ڼ�¼ɾ���ڵ�
		while (cur)
		{
			if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			// ����Ĺ������� ��key���� ����Ҫ��ɾ����ֵ�Ľڵ�
			else
			{
				if (cur->_left == nullptr)	// ������Ϊ�գ��� ֻ�������������Ը��ڵ���Ҫ���ӱ�ɾ���ڵ��������
				{
					if (cur == _root)
					{
						_root = cur->_right;	// ɾ���Ľڵ��� _root��ֱ�ӽ� cur->_right ��Ϊ _root �Ϳ�����
					}
					else
					{
						if (cur == parent->_right)		// cur Ϊ���ڵ��������
						{
							parent->_right = cur->_right;	// ���ڵ����������Ҫ����
						}
						else							// cur Ϊ���ڵ��������
						{
							parent->_left = cur->_right;	// ���ڵ����������Ҫ����
						}
					}
					delete cur;
				}
				else if (cur->_left == nullptr)		// ������Ϊ�գ��� ֻ�������������Ը��ڵ���Ҫ���ӱ�ɾ���ڵ��������
				{
					if (cur == _root)
					{
						_root = cur->_left;	// ɾ���Ľڵ��� _root��ֱ�ӽ� cur->_left ��Ϊ _root �Ϳ�����
					}
					else
					{
						if (cur == parent->_right)		// cur Ϊ���ڵ��������
						{
							parent->_right = cur->_left;	// ���ڵ����������Ҫ����
						}
						else							// cur Ϊ���ڵ��������
						{
							parent->_left = cur->_left;	// ���ڵ����������Ҫ����
						}
					}
					delete cur;
				}
				// ����ɾ���ڵ�ͬʱӵ����������ʱ������Ҫ�� �����������ڵ� �� ����������С�ڵ� �뱻ɾ���ڵ㽻��λ��
				// Ȼ�����ɾ��
				// ��ô ��ô�����ҵ� �����������ڵ� �� ����������С�ڵ��أ�
				// ���ݶ��������� ������С ������������ʣ����Է�����
				//		1. �����������ڵ� ������������û���������Ľڵ�
				//		2. �������е���С�ڵ㣬������������û���������Ľڵ�
				// ����ֻ��Ҫѡ��һ������Ϳ����ҵ���
				else									// cur �����ҽڵ�ʱ����Ҫ���ڵ�Ȼ��ɾ��
				{
					// ����ѡ�� ���������е���С�ڵ�
					// ��Ϊ��Ҫɾ��cur ����cur��parent�����ܸı䣬������Ҫ�����µı���ȥ��
					Node* minParent = cur;
					Node* minRight = cur->_right;
					while (minRight->_left)
					{
						minParent = minRight;
						minRight = minRight->_left;
					}

					std::swap(minRight->_key, cur->_key);		// ɾ��ĳ��ָ��ֵ�Ľڵ㣬�ڱ�����û���ظ����� �������ݾͿ���
					if (minRight == minParent->_left)
					{
						minParent->_left = minRight->_right;
					}
					else
					{
						minParent->_right = minRight->_right;
					}

					delete minRight;
				}

				return true;		// ɾ���ɹ��ͷ��� true
			}
		}

		return false;		// ����ѭ���ͱ�ʾɾ��ʧ�� û�ҵ�ָ��ֵ�Ľڵ㣬���Է��� false
	}

	//==================================================================================================
	//================================= �����Ƕ��������������ķǵݹ�� ======================================
	//==================================================================================================

	bool FindR(const K& key)
	{
		return _FindR(_root, key);
	}

	bool InsertR(const K& key)
	{
		return _InsertR(_root, key);
	}

	bool EraseR(const K& key)
	{
		return _EraseR(_root, key);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}	

private:

	// ���ڿ������캯����Ҫ����
	//	1. ���ڵ㷵�ظ�_root
	//	2. ���������� ��Ҫ���½ڵ����ӵ�����
	// ������Ҫ���ؽڵ�ָ������
	//
	Node* CopyTree(Node* root)
	{
		if (root == nullptr)
			return nullptr;

		Node* copyNode = new Node(root->_key);
		copyNode->_left = CopyTree(root->_left);
		copyNode->_right = CopyTree(root->_right);

		return copyNode;		// ���ؽڵ㽫���� ������������
	}

	void DestroyTree(Node* root)
	{
		if (root == nullptr)
			return;

		// ���������������� �ٷ���
		DestroyTree(root->_left);
		DestroyTree(root->_right);
		delete root;
	}

	// �� ���������˼·���ƣ��� ���ָ������� ����ʣ�¼�¼parent�Ĺ���
	// Ȼ������з��������ɾ��
	bool _EraseR(Node*& root, const K& key)
	{
		if (root == nullptr)
			return false;

		if (root->_key > key)
		{
			return _EraseR(root->_left, key);
		}
		else if (root->_key < key)
		{
			return _EraseR(root->_right, key);
		}
		else			// �ҵ��ˣ���ʼ�ж� ɾ���ڵ�
		{
			Node* del = root;	// ��¼��ɾ���ڵ�
			if (root->_left == nullptr)		// ֻ��������
			{
				root = root->_right;
			}
			else if (root->_right == nullptr)	// ֻ��������
			{
				root = root->_left;
			}
			// �ڵݹ��㷨�У� ɾ���� ���������Ľڵ� ����ȥ����Ҫɾ���Ľڵ�ģ�
			//	1. �����������ڵ㣺��������û���������Ľڵ�
			//	2. ����������С�ڵ㣺��������û���������Ľڵ�
			// �ҵ���֮������Ҫɾ���Ľڵ㽻��ֵ
			// ���� ��������ɾ������Ӧ����ôд�أ�
			// ��ʵ ֱ���� �����м����ݹ�Ϳ����ˣ�
			//	�� ������������С�ڵ�Ϊ������ɾ���ڵ�������������С�ڵ㽻��λ��֮���������Ķ����������Ľṹ��û�б��ƻ���
			//	��Ϊ ��ɾ���ڵ�һ���Ǳ� ���������е����нڵ�С�ģ���ô��ɾ���ڵ������������е�����ڵ㽻��λ�� ��ɾ���Ľڵ������ж�����С�ڵ�
			//	�� �����ɾ���ڵ� �� ���������е���С�ڵ㽻��λ�� ��������� ��С�ڵ��λ�ã�û���ƻ��������Ķ����������Ľṹ
			// ���� ��ɾ���ڵ� �� ���ʽڵ㽻��λ��֮�������ڵ��� �Ķ����������Ľṹû�б��ƻ���ֻ��Ҫ������ִ�еݹ�ɾ���Ϳ�����
			// 
			else						// ������������
			{
				Node* minRight = root->_right;
				while (minRight->_left)
				{
					minRight = minRight->_left;
				}

				std::swap(minRight->_key, root->_key);			// ����λ��֮���������Ľṹ��ʵ�Ǳ��ƻ��˵ģ����ܶ�������ִ�в���

				return _EraseR(root->_right, key);			// �������ṹû�б��ƻ�������ֱ�Ӷ���ݹ�ɾ��
			}

			delete del;
			return true;
		}
	}

	// ������� �½ڵ���Ҫ�븸�ڵ����ӣ����Եݹ��������Ҫ��¼ parent�ڵ�
	// �����ǵݹ�д�����ᴴ����ͬ��ջ֡��������һ������ջ֡�д����ֲ��������洢parent�ڵ����в�ͨ��
	// ��һ��˼·�ǣ����һ�� ������������ parent �ڵ㣬�����ﵽ��¼parent�ڵ������
	// ����һ��˼·���ǣ���ԭ���� �ڵ�ָ����Ϊ��������Ϊ �ڵ�ָ������� ��Ϊ����
	// ����������Ч��� �Ҳ������ڵ�����⣬�� �ڵ�ָ������� ���Բ����Ҹ��ڵ�
	// ���� ��������ʱ�� _InsertR(root->_right, key) ���뺯�� root->_right ��Ϊ root
	// ������ ���ã����Խ��뺯��֮�� root ֱ��Ϊ ���ڵ����������ֱ�Ӿ����븸�ڵ����ŵģ�����Ҫ������¼������
	//
	bool _InsertR(Node*& root, const K& key)
	{
		if (root == nullptr)			// rootΪ��ʱ����Ϊ��Ҫ����ڵ��λ��
		{
			root = new Node(key);			// ��ʱ�� root ���� parent�ĺ��ӣ������ٴ�����
			return true;
		}

		if (root->_key < key)				// key�� ����������λ��
		{
			return _InsertR(root->_right, key);
		} 
		else if (root->_key > key)			// keyС ����������λ��
		{
			return _InsertR(root->_left, key);
		}
		else	
		{
			return false;					// �ҵ��� key��ֵͬ�Ľڵ㣬ȡ�����룬����ʧ��
		}
	}

	// ����Key �ݹ�͸�����һ��
	bool _FindR(Node* root, const K& key)
	{
		if (root == nullptr)
			return false;

		if (root->_key < key)	// key�󣬾����������ݹ����
		{
			return _FindR(root->_right, key);
		}
		else if (root->_key > key)		// keyС�������������ݹ����
		{
			return _FindR(root->_left, key);
		}
		else		// �ҵ� �ͷ���true
		{
			return true;
		}
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	Node* _root;
};
