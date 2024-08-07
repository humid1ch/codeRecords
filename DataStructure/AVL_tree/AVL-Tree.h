#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
using namespace std;

template<class T>
struct AVLTreeNode {
	// �������ṹ
	AVLTreeNode(const T& data)
		: _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _data(data)
		, _bf(0)
	{}

	AVLTreeNode<T>* _pLeft; // �ýڵ������
	AVLTreeNode<T>* _pRight; // �ýڵ���Һ���
	AVLTreeNode<T>* _pParent; // �ýڵ��˫��
	T _data;
	int _bf; // �ýڵ��ƽ������
};

template<class T>
class AVLTree {
	typedef AVLTreeNode<T> Node;

public:
	bool insert(const T& data) {
		// ���Ȱ��� �����������ķ�ʽ ���Ҳ���λ�ò�����ڵ�
		if (_root == nullptr) {
			// ��Ϊ�� ����ڵ� ֱ�ӽ��½ڵ���Ϊ���ĸ�
			_root = new Node(data);
			_root->_bf = 0;		// ֻ�и��ڵ���������ڵ�ƽ������Ϊ 0

			return true;		// ����ɹ���ֱ�ӷ���
		}

		// �ߵ������˵����Ҫ ���Ҳ���λ�� ��
		Node* cur = _root;	// �Ӹ��ڵ㿪ʼ�Ƚ�
		Node* parent = nullptr;	// ��Ҫ��¼���׽ڵ� ������ʱ����
		while (cur) {
			// ѭ�������������� curΪ�գ�curΪ��ʱ��˵�� ����λ���ҵ���
			if (cur->_data > data) {
				// ����ֵ�ȵ�ǰ�ڵ�ֵ С������������
				parent = cur;
				cur = cur->_pLeft;
			}
			else if (cur->_data < data) {
				// ����ֵ�ȵ�ǰ�ڵ�ֵ �������Һ�����
				parent = cur;
				cur = cur->_pRight;
			}
			else {
				// �ߵ����� ˵�������Ѵ�����ͬ����
				return false;
			}
		}

		// ��ѭ��֮��cur ��Ϊ������Ҫ�����λ��
		cur = new Node(data);
		// ��cur������������
		if (data > parent->_data) {
			parent->_pRight = cur;		// �������ݱȸ��׽ڵ����ݴ�����뵽���׽ڵ���Һ���
		}
		else if (data < parent->_data) {
			parent->_pLeft = cur;			// �������ݱȸ��׽ڵ�����С������뵽���׽ڵ������
		}
		// �������ṹ��cur�ڵ���洢���׽ڵ�
		cur->_pParent = parent;

		// �����ߵ������˵�� �ڵ��Ѿ����뵽���� Ӧ�ò����λ����
		// ������Ҫ���ľ��� ����ƽ�� ����ƽ��
		// �����Ƿ���ƽ�⣺
		//		��Ҫ���ݽڵ��ƽ�������������Ƿ�ƽ��
		//		�� �Ѿ������в������µĽڵ㣬�����½ڵ�����Ƚڵ�������߶Ȳ�һ���ᷢ���仯
		//		����Ҫ���ľ��� �����½ڵ�����Ƚڵ��ƽ������
		// ��ô���£�
		//		����Ҫ֪�����������������֮ǰ�������Ѿ���ƽ�����������
		//					��ô����������֮�������Ƚڵ��ƽ�����ӻᷢ��ʲô�仯��
		//			1. ���Ƚڵ��ƽ������ ��Ϊ 1
		//				��ʱֻ��һ�ֿ��ܣ�
		//					����ǰ�����Ƚڵ����������ĸ߶���ȣ��½ڵ���������Ƚڵ�� ������ ��
		//					_bf �� 0 -> 1
		//			2. ���Ƚڵ��ƽ������ ��Ϊ -1
		//				��ʱͬ��ֻ��һ�ֿ��ܣ�
		//					����ǰ�����Ƚڵ����������ĸ߶���ȣ��½ڵ���������Ƚڵ�� ������ ��
		//					_bf �� 0 -> -1
		//			3. ���Ƚڵ��ƽ������ ��Ϊ 0
		//				����1��
		//					����ǰ�����Ƚڵ� �� �� ������ �߶ȸ� 1���½ڵ���������Ƚڵ�� ������ ��
		//					_bf �� -1 -> 0
		//				����2��
		//					����ǰ�����Ƚڵ� �� �� ������ �߶ȸ� 1���½ڵ���������Ƚڵ�� ������ ��
		//					_bf �� 1 -> 0
		//			4. ���Ƚڵ��ƽ������ ��Ϊ 2
		//					����ǰ�����Ƚڵ� �� �� ������ �߶ȸ� 1���½ڵ����ɲ��������Ƚڵ�� ������ ��
		//					_bf �� 1 -> 2
		//			5. ���Ƚڵ��ƽ������ ��Ϊ -2
		//					����ǰ�����Ƚڵ� �� �� ������ �߶ȸ� 1���½ڵ����ɲ��������Ƚڵ�� ������ ��
		//					_bf �� -1 -> -2
		//		���Ƚڵ��ƽ������ ��Ҫ ���½ڵ����� ѭ�� �����Ƚڵ����  ��Ҳ�ǽ�AVL���ڵ���Ƴ���������ԭ��
		//		��ô ѭ��Ӧ��ʲôʱ��ֹͣ��
		//		����Ҫ֪�� ���½ڵ� ���� �������Ƚڵ��ƽ�����ӣ����п��ܸ��µ��������ĸ��ģ���ξ��� ��ĳ���Ƚڵ��ƽ�����ӱ�Ϊ0ʱ��ֹͣ����
		//	
		//		���Ƚڵ��ƽ�����ӱ�Ϊ0 ֹͣ��������Ϊ ƽ�����ӱ�Ϊ0�ˣ���˵���Դ˽ڵ�Ϊ�������ĸ߶�û�б仯��ֻ�ǰѵ͵������θ���һ��
		//																Ҳ����ζ�� �˽ڵ�����Ƚڵ��ƽ�����Ӳ���仯
		//		�� ���ĳ�ڵ��ƽ�����ӱ�Ϊ�� 1 �� -1, ��˵�� �Դ˽ڵ�Ϊ������ �߶ȷ����˱仯��������Ӱ�������Ƚڵ��ƽ������
		//		���ĳ�ڵ��ƽ�����ӱ�Ϊ�� -2 �� 2����ô����Ҫ �����Դ˽ڵ�Ϊ����������ƽ����
		//		���� ���½ڵ� �����޸����Ƚڵ��ƽ�����ӵ�ѭ������������ ĳ���Ƚڵ��ƽ�����ӱ�Ϊ 0 �� �Ѿ����ϵ���
		while (parent) {
			if (cur == parent->_pLeft)
				parent->_bf--;			// �½ڵ��ڸ��׽ڵ�����ӣ����׽ڵ���������߶�+1�����׽ڵ��ƽ������-1
			else
				parent->_bf++;

			// ������֮�󣬾���Ҫ�ж� ��Ҫ�������� ����ֹͣ���� ���ǵ���ƽ����
			if (parent->_bf == 0) {
				// ĳ���Ƚڵ��ƽ������ �� -1 �� 1 -> 0��˵�� �����½ڵ�ʹ�����Ƚڵ�����������߶������
				// ������Ӱ����ϱߵĽڵ㣬���Կ��Խ���
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1) {
				// ĳ���Ƚڵ��ƽ�����Ӵ� 0 ����� -1 �� 1��˵�� �����Ƚڵ����������ԭ��û�и߶Ȳ���ǲ����½ڵ�ʹ�������������˸߶Ȳ�
				// ���ܻ����Ӱ����߽ڵ��ƽ�� ������Ҫ����parent �� cur�����������������Ƚڵ��ƽ������
				cur = cur->_pParent;
				parent = parent->_pParent;
			}
			else if (parent->_bf == -2 || parent->_bf == 2) {
				// ��ĳ���Ƚڵ��ƽ������ �� -1 -> -2 �� �� 1 -> 2����˵�� �����½ڵ�ʹ�����Ƚڵ�ĸ�������ø�����
				// �����Ѿ�Ӱ�����Դ����Ƚڵ�Ϊ��������ƽ�⣬���Ծ���Ҫ���ڽڵ����ά��ƽ����
				// AVL�� ����ƽ��ķ����� ��ת
				// ���� ����������̬�Լ�����ڵ��λ�ò�ͬ�����в�ͬ����ת��ʽ����Ҫ��ͼ���

				// ���������
				if (parent->_bf == 2 && cur->_bf == 1) {
					RotateL(parent);
				}
				// �ҵ��������
				else if (parent->_bf == -2 && cur->_bf == -1) {
					RotateR(parent);
				}
				// ����˫�������
				else if (parent->_bf == -2 && cur->_bf == 1) {
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1) {
					RotateRL(parent);
				}

				break;
			}
			else {
				// ������������ڱ�֤�����½ڵ�ʱ�����Ѿ���ƽ�����������
				// ������� ����ߵ��˴� �������� �������ִ���
				assert(false);
			}
		}
		
		return true;
	}

	bool isBalanceTree() {
		return _isBalanceTree(_root);
	}

	int treeHeight() {
		return _treeHeight(_root);
	}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	vector<vector<int>> levelOrder() {
		vector<vector<int>> vv;
		if (_root == nullptr)
			return vv;

		queue<Node*> q;
		int levelSize = 1;
		q.push(_root);

		while (!q.empty())
		{
			// levelSize����һ��һ���
			vector<int> levelV;
			while (levelSize--)
			{
				Node* front = q.front();
				q.pop();
				levelV.push_back(front->_data);
				if (front->_pLeft)
					q.push(front->_pLeft);

				if (front->_pRight)
					q.push(front->_pRight);
			}
			vv.push_back(levelV);
			for (auto e : levelV)
			{
				cout << e << " ";
			}
			cout << endl;

			// ��һ����꣬��һ��Ͷ�������
			levelSize = q.size();
		}

		return vv;
	}


private:
	/* ����
		������ ������������� ���ֽڵ�ֵΪ ƽ�����ӡ�h �� h+1 ��ʾһ���� h�����߶�
					1								2
				   / \							   / \
				  h   0		------->			  h   1
					 / \							 / \
					h   h							h  h+1
		h = 0, ��ʾ h��ʾ����Ϊ��, �������ֱ�ʾ�˽ڵ��ƽ������
					1								2
					 \							     \
					  0		------->			      1
													   \
														0
		h = 1, ��ʾ h��ʾ�����߶�Ϊ1, �������ֱ�ʾ�˽ڵ��ƽ������
					1											   2
				   / \											  / \
				  0   0					------->				 0   1
					 / \	(�������²�����Ҷ�ӽ�������λ��)		/ \
					0   0										   0   1
																		\
																		 0
		h = 2, ��ʾ h��ʾ�����߶�Ϊ2, �������ֱ�ʾ�˽ڵ��ƽ������
					1												      2
				 /     \											   /     \
				0       0											  0       1
			   / \    /   \					------->				 / \    /   \
			  0   0  0     0	(�������²��ĸ�Ҷ�ӽ�������λ��)	0   0  0     1
					/ \   / \											  / \   / \
				   0   0 0   0										     0   0 0   1
																					\
																					 0

		���ֽṹ h ��ͬ, �����Ĳ�ͬ�Ľṹ, �½ڵ�Ҳ�Ϳ��Բ��벻ͬ��λ��, ���Ǵ˽ṹͬ���Կ�Ϊ
		��ĳ�ڵ�� �����������½ڵ�, ���´˽ڵ�ƽ�����ӵľ���ֵ��0 ��Ϊ 1, �������´˽ڵ�ĸ��׽ڵ�ƽ�����ӱ�Ϊ 2
		��ʱ �˽ڵ�ĸ��׽ڵ�ƽ������ʧ�� ��Ҫ��ת����ƽ��
		�� AVL�� ��ƽ��Ķ���������
		���ݶ����������Ľṹ�ص㣺����С �Һ��Ӵ�
					 a(2)
					/     \
				   b(h)   c(1)
						 /     \
						d(h)   e(h+1)
		�� b < a < d < c < e
		��ô��������ʹ �����ƽ���أ�
		�� d ��Ϊ a�� ������, ���� a ��Ϊ c��������
						  c(0)
						 /     \
						a(0)   e(h+1)
					   /     \
					  b(h)   d(h)
		������ b < a < d < c < e
		�����Ѿ���������ƽ��
		�� c �� �������߶�Ϊ h+1, �������߶�ͬ��Ϊ h+1

		�� �����ķ�����  �� �˽ڵ�� ���׽ڵ� ��Ϊ �˽ڵ������, ���˽ڵ� ԭ���� ��Ϊ ԭ���׽ڵ� ���Һ���
	*/
	void RotateL(Node* parent) {
		Node* subR = parent->_pRight;		// �˽ڵ�, ����ƽ��ڵ���Һ���
		Node* subRL = subR->_pLeft;			// �˽ڵ�����

		/* parent ������ �������ĸ�, Ҳ������ĳ�ڵ��������
		* �� ����AVL���Ľڵ����������Ľṹ, ���Ըı�ڵ��λ�� ��Ҫ�ı�˽ڵ�ĸ��׽ڵ�, ����
		* �� parent ���������ĸ�ʱ, ��parent->_pParent Ϊ��, ��ô����ʱ ����Ҫֱ�ӽ� subR��Ϊ�������ĸ�
		* �� parent ��ĳ�ڵ������ʱ, ����Ҫ�� parent->_pParent �� subR ��������
		* ���� ��Ҫ�� parent->_pParent �洢����
		*/
		Node* ppNode = parent->_pParent;

		// �� �˽ڵ������ ��Ϊ ���׽ڵ���Һ���, ���� �˽ڵ�ĸ��׽ڵ� ��Ϊ �˽ڵ������
		// ���ǵ� ����������
		parent->_pRight = subRL;
		if (subRL)
			subRL->_pParent = parent;

		subR->_pLeft = parent;
		parent->_pParent = subR;

		// �ı䲻ƽ��ڵ� �� ���׽ڵ��ָ��
		if (parent == _root) {
			_root = subR;
			_root->_pParent = nullptr;
		}
		else {
			if (parent == ppNode->_pLeft)		// ��ƽ��ڵ����丸�׽ڵ������
				ppNode->_pLeft = subR;			// �� subR ���ӵ� �丸�׽ڵ��������
			else
				ppNode->_pRight = subR;			// �� subR ���ӵ� �丸�׽ڵ���Һ�����

			subR->_pParent = ppNode;		// ���� subR �ĸ��׽ڵ�
		}


		/*
		*		a(2)											c(0)
			   /     \										   /     \
			  b(h)   c(1)									  a(0)   e(h+1)
					/     \									 /     \
			       d(h)   e(h+1)						    b(h)   d(h)
		*/
		// a ƽ�����ӱ�Ϊ 0, c ƽ�����ӱ�Ϊ 0

		parent->_bf = 0;
		subR->_bf = 0;
	}

	void RotateR(Node* parent) {
		Node* subL = parent->_pLeft;		// �˽ڵ�, ����ƽ��ڵ������
		Node* subLR = subL->_pRight;		// �˽ڵ��Һ���

		Node* ppNode = parent->_pParent;

		// �� �˽ڵ���Һ��� ��Ϊ ���׽ڵ������, ���� �˽ڵ�ĸ��׽ڵ� ��Ϊ �˽ڵ���Һ���
		// ���ǵ� ����������
		parent->_pLeft = subLR;
		if (subLR)
			subLR->_pParent = parent;

		subL->_pRight = parent;
		parent->_pParent = subL;

		// �ı䲻ƽ��ڵ� �� ���׽ڵ��ָ��
		if (parent == _root) {
			_root = subL;
			_root->_pParent = nullptr;
		}
		else {
			if (parent == ppNode->_pLeft)		// ��ƽ��ڵ����丸�׽ڵ������
				ppNode->_pLeft = subL;			// �� subL ���ӵ� �丸�׽ڵ��������
			else
				ppNode->_pRight = subL;			// �� subL ���ӵ� �丸�׽ڵ���Һ�����

			subL->_pParent = ppNode;		// ���� subL �ĸ��׽ڵ�
		}

		parent->_bf = 0;
		subL->_bf = 0;
	}

	void RotateLR(Node* parent) {
		Node* subL = parent->_pLeft;
		Node* subLR = subL->_pRight;
		int bf = subLR->_bf;
		// ����˫��
		RotateL(parent->_pLeft);
		RotateR(parent);
		
		// ��ͼ���Կ����� ��������λ�ò�ͬ ƽ�����ӵĸ��¹���Ҳ��ͬ
		if (bf == 0) {
			parent->_bf = 0;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else if (bf == 1) {
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;
		}
		else if (bf == -1) {
			parent->_bf = 1;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else {
			assert(false);
		}
	}
	void RotateRL(Node* parent) {
		Node* subR = parent->_pRight;
		Node* subRL = subR->_pLeft;
		int bf = subRL->_bf;
		// ����˫��
		RotateR(parent->_pRight);
		RotateL(parent);
		
		// ��ͼ���Կ����� ��������λ�ò�ͬ ƽ�����ӵĸ��¹���Ҳ��ͬ
		if (bf == 0) {
			parent->_bf = 0;
			subR->_bf = 0;
			subRL->_bf = 0;
		}
		else if (bf == 1) {
			parent->_bf = -1;
			subR->_bf = 0;
			subRL->_bf = 0;
		}
		else if (bf == -1) {
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
		else {
			assert(false);
		}
	}

	int _treeHeight(Node* root) {
		if (root == nullptr) {
			return 0;
		}

		int leftHeight = _treeHeight(root->_pLeft);
		int rightHeight = _treeHeight(root->_pRight);

		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	bool _isBalanceTree(Node* root) {
		// �ݹ����������, �ж�ƽ������
		if (root == nullptr) {
			return true;
		}

		int leftHeight = _treeHeight(root->_pLeft);
		int rightHeight = _treeHeight(root->_pRight);
		int diff = rightHeight - leftHeight;

		if (abs(diff) > 1) {
			cout << root->_data << " :: �ڵ�ƽ�������쳣" << endl;
			return false;
		}
		else if (diff != root->_bf) {
			cout << root->_data << " :: �ڵ�ƽ�����Ӳ�����ʵ��" << endl;
			return false;
		}

		return _isBalanceTree(root->_pLeft) && _isBalanceTree(root->_pRight);
	}
	
	Node* _root = nullptr;
};