#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
using namespace std;

template<class T>
struct AVLTreeNode {
	// 三叉链结构
	AVLTreeNode(const T& data)
		: _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _data(data)
		, _bf(0)
	{}

	AVLTreeNode<T>* _pLeft; // 该节点的左孩子
	AVLTreeNode<T>* _pRight; // 该节点的右孩子
	AVLTreeNode<T>* _pParent; // 该节点的双亲
	T _data;
	int _bf; // 该节点的平衡因子
};

template<class T>
class AVLTree {
	typedef AVLTreeNode<T> Node;

public:
	bool insert(const T& data) {
		// 首先按照 二叉搜索树的方式 查找插入位置并插入节点
		if (_root == nullptr) {
			// 树为空 插入节点 直接将新节点作为树的根
			_root = new Node(data);
			_root->_bf = 0;		// 只有根节点的树，根节点平衡因子为 0

			return true;		// 插入成功，直接返回
		}

		// 走到这里就说明需要 查找插入位置 了
		Node* cur = _root;	// 从根节点开始比较
		Node* parent = nullptr;	// 需要记录父亲节点 供插入时连接
		while (cur) {
			// 循环结束的条件是 cur为空，cur为空时就说明 插入位置找到了
			if (cur->_data > data) {
				// 插入值比当前节点值 小，则向左孩子找
				parent = cur;
				cur = cur->_pLeft;
			}
			else if (cur->_data < data) {
				// 插入值比当前节点值 大，则向右孩子找
				parent = cur;
				cur = cur->_pRight;
			}
			else {
				// 走到这里 说明数中已存在相同数据
				return false;
			}
		}

		// 出循环之后，cur 即为数据需要插入的位置
		cur = new Node(data);
		// 将cur与树连接起来
		if (data > parent->_data) {
			parent->_pRight = cur;		// 插入数据比父亲节点数据大，则插入到父亲节点的右孩子
		}
		else if (data < parent->_data) {
			parent->_pLeft = cur;			// 插入数据比父亲节点数据小，则插入到父亲节点的左孩子
		}
		// 三叉链结构，cur节点虚存储父亲节点
		cur->_pParent = parent;

		// 代码走到这里，就说明 节点已经插入到树中 应该插入的位置了
		// 接下来要做的就是 分析平衡 调整平衡
		// 首先是分析平衡：
		//		需要根据节点的平衡因子来分析是否平衡
		//		而 已经在树中插入了新的节点，所以新节点的祖先节点的子树高度差一定会发生变化
		//		首先要做的就是 更新新节点的祖先节点的平衡因子
		// 怎么更新？
		//		首先要知道，如果插入新数据之前，此树已经是平衡二叉搜索树
		//					那么插入新数据之后，其祖先节点的平衡因子会发生什么变化？
		//			1. 祖先节点的平衡因子 变为 1
		//				此时只有一种可能：
		//					插入前，祖先节点左右子树的高度相等，新节点插在了祖先节点的 右子树 里
		//					_bf 由 0 -> 1
		//			2. 祖先节点的平衡因子 变为 -1
		//				此时同样只有一种可能：
		//					插入前，祖先节点左右子树的高度相等，新节点插在了祖先节点的 左子树 里
		//					_bf 由 0 -> -1
		//			3. 祖先节点的平衡因子 变为 0
		//				可能1：
		//					插入前，祖先节点 左 比 右子树 高度高 1，新节点插在了祖先节点的 右子树 里
		//					_bf 由 -1 -> 0
		//				可能2：
		//					插入前，祖先节点 右 比 左子树 高度高 1，新节点插在了祖先节点的 左子树 里
		//					_bf 由 1 -> 0
		//			4. 祖先节点的平衡因子 变为 2
		//					插入前，祖先节点 右 比 左子树 高度高 1，新节点依旧插在了祖先节点的 右子树 里
		//					_bf 由 1 -> 2
		//			5. 祖先节点的平衡因子 变为 -2
		//					插入前，祖先节点 左 比 右子树 高度高 1，新节点依旧插在了祖先节点的 左子树 里
		//					_bf 由 -1 -> -2
		//		祖先节点的平衡因子 需要 从新节点向上 循环 找祖先节点更新  这也是将AVL树节点设计成三叉链的原因
		//		那么 循环应该什么时候停止？
		//		首先要知道 从新节点 向上 更新祖先节点的平衡因子，是有可能更新到整棵树的根的，其次就是 当某祖先节点的平衡因子变为0时，停止更新
		//	
		//		祖先节点的平衡因子变为0 停止更新是因为 平衡因子变为0了，就说明以此节点为根的树的高度没有变化，只是把低的子树拔高了一层
		//																也就意味着 此节点的祖先节点的平衡因子不会变化
		//		而 如果某节点的平衡因子变为了 1 或 -1, 就说明 以此节点为根的树 高度发生了变化，进而会影响其祖先节点的平衡因子
		//		如果某节点的平衡因子变为了 -2 或 2，那么就需要 调整以此节点为根的树保持平衡了
		//		所以 从新节点 向上修改祖先节点的平衡因子的循环结束条件是 某祖先节点的平衡因子变为 0 或 已经向上到根
		while (parent) {
			if (cur == parent->_pLeft)
				parent->_bf--;			// 新节点在父亲节点的左孩子，则父亲节点的左子树高度+1，则父亲节点的平衡因子-1
			else
				parent->_bf++;

			// 更新完之后，就需要判断 需要继续更新 还是停止更新 或是调整平衡了
			if (parent->_bf == 0) {
				// 某祖先节点的平衡因子 从 -1 或 1 -> 0，说明 插入新节点使此祖先节点的左右子树高度相等了
				// 不会再影响更上边的节点，所以可以结束
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1) {
				// 某祖先节点的平衡因子从 0 变成了 -1 或 1，说明 此祖先节点的左右子树原先没有高度差，但是插入新节点使其左右子树有了高度差
				// 可能会继续影响更高节点的平衡 所以需要更新parent 和 cur，进而继续更新祖先节点的平衡因子
				cur = cur->_pParent;
				parent = parent->_pParent;
			}
			else if (parent->_bf == -2 || parent->_bf == 2) {
				// 当某祖先节点的平衡因子 从 -1 -> -2 或 从 1 -> 2，就说明 插入新节点使此祖先节点的高子树变得更高了
				// 进而已经影响了以此祖先节点为根的树的平衡，所以就需要调节节点进而维持平衡了
				// AVL树 调节平衡的方法是 旋转
				// 但是 根据树的形态以及插入节点的位置不同，会有不同的旋转方式，需要画图理解

				// 左单旋的情况
				if (parent->_bf == 2 && cur->_bf == 1) {
					RotateL(parent);
				}
				// 右单旋的情况
				else if (parent->_bf == -2 && cur->_bf == -1) {
					RotateR(parent);
				}
				// 左右双旋的情况
				else if (parent->_bf == -2 && cur->_bf == 1) {
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1) {
					RotateRL(parent);
				}

				break;
			}
			else {
				// 以上情况都是在保证插入新节点时，树已经是平衡二叉搜索树
				// 如果不是 则会走到此处 触发断言 进而发现错误
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
			// levelSize控制一层一层出
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

			// 上一层出完，下一层就都进队列
			levelSize = q.size();
		}

		return vv;
	}


private:
	/* 左单旋
		适用于 类似这种情况： 数字节点值为 平衡因子、h 和 h+1 表示一棵树 h是树高度
					1								2
				   / \							   / \
				  h   0		------->			  h   1
					 / \							 / \
					h   h							h  h+1
		h = 0, 表示 h表示的树为空, 树中数字表示此节点的平衡因子
					1								2
					 \							     \
					  0		------->			      1
													   \
														0
		h = 1, 表示 h表示的树高度为1, 树中数字表示此节点的平衡因子
					1											   2
				   / \											  / \
				  0   0					------->				 0   1
					 / \	(插入最下层两个叶子结点的任意位置)		/ \
					0   0										   0   1
																		\
																		 0
		h = 2, 表示 h表示的树高度为2, 树中数字表示此节点的平衡因子
					1												      2
				 /     \											   /     \
				0       0											  0       1
			   / \    /   \					------->				 / \    /   \
			  0   0  0     0	(插入最下层四个叶子结点的任意位置)	0   0  0     1
					/ \   / \											  / \   / \
				   0   0 0   0										     0   0 0   1
																					\
																					 0

		此种结构 h 不同, 有许多的不同的结构, 新节点也就可以插入不同的位置, 但是此结构同可以看为
		在某节点的 右子树插入新节点, 导致此节点平衡因子的绝对值从0 变为 1, 进而导致此节点的父亲节点平衡因子变为 2
		此时 此节点的父亲节点平衡因子失衡 需要旋转调节平衡
		而 AVL树 是平衡的二叉搜索树
		根据二叉搜索树的结构特点：左孩子小 右孩子大
					 a(2)
					/     \
				   b(h)   c(1)
						 /     \
						d(h)   e(h+1)
		即 b < a < d < c < e
		怎么调整才能使 这棵树平衡呢？
		将 d 作为 a的 右子树, 并将 a 作为 c的左子树
						  c(0)
						 /     \
						a(0)   e(h+1)
					   /     \
					  b(h)   d(h)
		依旧是 b < a < d < c < e
		并且已经将树调整平衡
		即 c 的 左子树高度为 h+1, 右子树高度同样为 h+1

		即 左单旋的方法是  将 此节点的 父亲节点 变为 此节点的左孩子, 将此节点 原左孩子 变为 原父亲节点 的右孩子
	*/
	void RotateL(Node* parent) {
		Node* subR = parent->_pRight;		// 此节点, 即不平衡节点的右孩子
		Node* subRL = subR->_pLeft;			// 此节点左孩子

		/* parent 可能是 整棵树的根, 也可能是某节点的子树根
		* 而 由于AVL树的节点是三叉链的结构, 所以改变节点的位置 需要改变此节点的父亲节点, 所以
		* 当 parent 是整棵树的根时, 即parent->_pParent 为空, 那么左旋时 就需要直接将 subR改为整棵树的根
		* 当 parent 是某节点的子树时, 就需要将 parent->_pParent 与 subR 连接起来
		* 所以 需要将 parent->_pParent 存储起来
		*/
		Node* ppNode = parent->_pParent;

		// 将 此节点的左孩子 变为 父亲节点的右孩子, 并将 此节点的父亲节点 变为 此节点的左孩子
		// 并记得 链接三叉链
		parent->_pRight = subRL;
		if (subRL)
			subRL->_pParent = parent;

		subR->_pLeft = parent;
		parent->_pParent = subR;

		// 改变不平衡节点 的 父亲节点的指向
		if (parent == _root) {
			_root = subR;
			_root->_pParent = nullptr;
		}
		else {
			if (parent == ppNode->_pLeft)		// 不平衡节点是其父亲节点的左孩子
				ppNode->_pLeft = subR;			// 把 subR 连接到 其父亲节点的左孩子上
			else
				ppNode->_pRight = subR;			// 把 subR 连接到 其父亲节点的右孩子上

			subR->_pParent = ppNode;		// 更新 subR 的父亲节点
		}


		/*
		*		a(2)											c(0)
			   /     \										   /     \
			  b(h)   c(1)									  a(0)   e(h+1)
					/     \									 /     \
			       d(h)   e(h+1)						    b(h)   d(h)
		*/
		// a 平衡因子变为 0, c 平衡因子变为 0

		parent->_bf = 0;
		subR->_bf = 0;
	}

	void RotateR(Node* parent) {
		Node* subL = parent->_pLeft;		// 此节点, 即不平衡节点的左孩子
		Node* subLR = subL->_pRight;		// 此节点右孩子

		Node* ppNode = parent->_pParent;

		// 将 此节点的右孩子 变为 父亲节点的左孩子, 并将 此节点的父亲节点 变为 此节点的右孩子
		// 并记得 链接三叉链
		parent->_pLeft = subLR;
		if (subLR)
			subLR->_pParent = parent;

		subL->_pRight = parent;
		parent->_pParent = subL;

		// 改变不平衡节点 的 父亲节点的指向
		if (parent == _root) {
			_root = subL;
			_root->_pParent = nullptr;
		}
		else {
			if (parent == ppNode->_pLeft)		// 不平衡节点是其父亲节点的左孩子
				ppNode->_pLeft = subL;			// 把 subL 连接到 其父亲节点的左孩子上
			else
				ppNode->_pRight = subL;			// 把 subL 连接到 其父亲节点的右孩子上

			subL->_pParent = ppNode;		// 更新 subL 的父亲节点
		}

		parent->_bf = 0;
		subL->_bf = 0;
	}

	void RotateLR(Node* parent) {
		Node* subL = parent->_pLeft;
		Node* subLR = subL->_pRight;
		int bf = subLR->_bf;
		// 左右双旋
		RotateL(parent->_pLeft);
		RotateR(parent);
		
		// 画图可以看出来 如果插入的位置不同 平衡因子的更新规则也不同
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
		// 右左双旋
		RotateR(parent->_pRight);
		RotateL(parent);
		
		// 画图可以看出来 如果插入的位置不同 平衡因子的更新规则也不同
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
		// 递归遍历二叉树, 判断平衡因子
		if (root == nullptr) {
			return true;
		}

		int leftHeight = _treeHeight(root->_pLeft);
		int rightHeight = _treeHeight(root->_pRight);
		int diff = rightHeight - leftHeight;

		if (abs(diff) > 1) {
			cout << root->_data << " :: 节点平衡因子异常" << endl;
			return false;
		}
		else if (diff != root->_bf) {
			cout << root->_data << " :: 节点平衡因子不符合实际" << endl;
			return false;
		}

		return _isBalanceTree(root->_pLeft) && _isBalanceTree(root->_pRight);
	}
	
	Node* _root = nullptr;
};