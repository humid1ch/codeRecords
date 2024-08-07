#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
using namespace std;

// ö�ٳ���, ��ʾ �� ��
enum Color {
    RED,
    BLACK
};

template<class T1>
struct RBTreeNode {
    // RedBlackTreeNode ������ڵ�
    RBTreeNode(const T1& data = T1())
        : _pLeft(nullptr), _pRight(nullptr), _pParent(nullptr)
        , _data(data)
        , _color(RED) 	// �½ڵ�Ĭ��Ϊ��ڵ� 
    {}

    RBTreeNode<T1>* _pLeft;			// �ڵ�����
    RBTreeNode<T1>* _pRight;		// �ڵ��Һ���
    RBTreeNode<T1>* _pParent;		// �ڵ㸸�׽ڵ�

    T1 _data;					     // �ڵ�����
    Color _color;					// �ڵ���ɫ
};

template<class T1>
class RBTree {
    typedef RBTreeNode<T1> Node;			// �Խڵ����ͽ���typedef

public:
    bool insert(const T1& data) {
        if (_root == nullptr) {
            // ��Ϊ��ʱ, �����½ڵ�
            _root = new Node(data);
            _root->_color = BLACK;			// ���ڵ�ҪΪ ��

            return true;
        }

        // ����Ϊ��, �ʹӸ��ڵ㿪ʼ��λ��
        Node* cur = _root;
        Node* parent = cur->_pParent;
        while (cur) {
            if (data > cur->_data) {
                // �������ݴ�, ������������
                parent = cur;
                cur = cur->_pRight;
            }
            else if (data < cur->_data) {
                // ��������С, ������������
                parent = cur;
                cur = cur->_pLeft;
            }
            else {
                // �����Ѵ��ڲ�������, ���� false ����ʧ��
                return false;
            }
        }

        // ��ѭ��֮��, cur����λ�þ��� �½ڵ���Ҫ�����λ��
        cur = new Node(data);
        cur->_color = RED;
        // parent �� cur��������
        if (data > parent->_data) {
            // ���ݴ�, ���½ڵ��Ǹ��׽ڵ���ҽڵ�
            parent->_pRight = cur;
        }
        else {
            // ����С, ���½ڵ��Ǹ��׽ڵ����ڵ�
            parent->_pLeft = cur;
        }
        cur->_pParent = parent;

        // ��������½ڵ�ʱ �Ѿ���¼�� cur �� parent�ڵ�
        while (parent && parent->_color == RED) {
            // ���׽ڵ����, �Ҹ��׽ڵ�ҲΪ��ɫʱ
            Node* grandFa = parent->_pParent; 			// ��¼���Ƚڵ�
            assert(grandFa);						 // �����游�ڵ����
            // ����游�ڵ㲻����, ��˵�� parent�ڵ������ĸ�, �ǲ����ܵ� ��Ϊ��������������Ǻ�ɫ��

            if (parent == grandFa->_pLeft) {
                // ���׽ڵ������Ƚڵ������
                Node* uncle = grandFa->_pRight;			// ��¼����ڵ�
                if (uncle && uncle->_color == RED) {
                    // ����ڵ���� ���Ǻ�ڵ�
                    parent->_color = uncle->_color = BLACK;			// ���׽ڵ� �� ����ڵ��Ϊ��ɫ
                    grandFa->_color = RED;						 // �游�ڵ� ��Ϊ��ɫ

                    cur = grandFa;								// ���� grandFa�ڵ�Ϊ�µ�cur�ڵ�
                    parent = cur->_pParent;						  // ���� �µ�parent�ڵ�
                }
                else {
                    // uncleΪ�� �� Ϊ��ʱ
                    if (cur == parent->_pLeft) {
                        // parent��grandFa������, cur��parent������, �� ֱ�ߵ����
                        rotateR(grandFa);				// �� ���Ƚڵ���ΪrotateR��parent, �ҵ���

                        parent->_color = BLACK;			 // ���� parent�ڵ���ɫΪ��
                        grandFa->_color = RED;			 // ���� grandFa�ڵ���ɫΪ��
                    }
                    else {
                        // parent��grandFa������, cur��parent���Һ���, �� ���ߵ����
                        rotateL(parent);				// �Ƚ� parent�ڵ���ΪrotateL��parent, ����
                        rotateR(grandFa); 				// �ٽ� grandFa�ڵ���ΪrotateR��parent, �ҵ���

                        cur->_color = BLACK;			// ���� cur�ڵ���ɫΪ��
                        grandFa->_color = RED;			// ���� grandFa�ڵ���ɫΪ��
                    }
                    // ����֮�� ����ѭ��
                    break;
                }
            }
            else {
                // ���׽ڵ������Ƚڵ���Һ���
                Node* uncle = grandFa->_pLeft;			// ��¼����ڵ�
                if (uncle && uncle->_color == RED) {
                    parent->_color = uncle->_color = BLACK;
                    grandFa->_color = RED;

                    cur = grandFa;
                    parent = cur->_pParent;
                }
                else {
                    // uncleΪ�� �� Ϊ��ʱ
                    if (cur == parent->_pRight) {
                        // parent��grandFa���Һ���, cur��parent���Һ���, �� ֱ�ߵ����
                        rotateL(grandFa);				// �� ���Ƚڵ���ΪrotateL��parent, ����

                        parent->_color = BLACK;			 // ���� parent�ڵ���ɫΪ��
                        grandFa->_color = RED;			 // ���� grandFa�ڵ���ɫΪ��
                    }
                    else {
                        // parent��grandFa���Һ���, cur��parent������, �� ���ߵ����
                        rotateR(parent);				// �Ƚ� parent�ڵ���ΪrotateR��parent, �ҵ���
                        rotateL(grandFa); 				// �ٽ� grandFa�ڵ���ΪrotateL��parent, ����

                        cur->_color = BLACK;			// ���� cur�ڵ���ɫΪ��
                        grandFa->_color = RED;			// ���� grandFa�ڵ���ɫΪ��
                    }
                    // ����֮�� ����ѭ��
                    break;
                }
            }
        }
        _root->_color = BLACK;				  // ������� �����¸��ڵ����ɫΪ��

        return true;						// �������, ����true
    }

    
    void treeHeight() {
        cout << "�·�����ȣ�" << _maxHeight(_root) << endl;
        cout << "���·�����ȣ�" << _minHeight(_root) << endl;
    }

    bool isRBTree() {
        if (_root == nullptr)
            return true;			// ���� Ҳ�ɿ��������

        if (_root->_color == RED)
            return false;			// ���ڵ�Ϊ��, �������Ǻ����

        // ȡ����һ��·���ϵĺڽڵ���Ŀ ��Ϊ�Ƚϻ�׼
        Node* pCur = _root;
        size_t blackCount = 0;
        while (pCur) {
            if (pCur->_color == BLACK)
                blackCount++;

            pCur = pCur->_pLeft;
        }

        size_t pathBlack = 0;
        return _pathBlackCom(_root, pathBlack, blackCount);
    }

    void inOrder() {
        _inOrder(_root);
        cout << endl;
    }

    void levelOrder() {
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
            for (auto e : levelV)
            {
                cout << e << " ";
            }
            cout << endl;

            // ��һ����꣬��һ��Ͷ�������
            levelSize = q.size();
        }
    }

private:
    void _inOrder(Node* pRoot) {
        if (pRoot == nullptr)
            return;

        _inOrder(pRoot->_pLeft);
        cout << pRoot->_data << " ";
        _inOrder(pRoot->_pRight);
    }

    void rotateL(Node* parent) {
        Node* subR = parent->_pRight;
        Node* subRL = subR->_pLeft;

        parent->_pRight = subRL;
        if (subRL)
            subRL->_pParent = parent;

        Node* ppNode = parent->_pParent;

        subR->_pLeft = parent;
        parent->_pParent = subR;

        if (parent == _root) {
            _root = subR;
            _root->_pParent = nullptr;
        }
        else {
            if (parent == ppNode->_pLeft)
                ppNode->_pLeft = subR;
            else
                ppNode->_pRight = subR;

            subR->_pParent = ppNode;
        }
    }

    void rotateR(Node* parent) {
        Node* subL = parent->_pLeft;
        Node* subLR = subL->_pRight;

        parent->_pLeft = subLR;
        if (subLR)
            subLR->_pParent = parent;

        Node* ppNode = parent->_pParent;

        subL->_pRight = parent;
        parent->_pParent = subL;

        if (parent == _root) {
            _root = subL;
            _root->_pParent = nullptr;
        }
        else {
            if (parent == ppNode->_pLeft)
                ppNode->_pLeft = subL;
            else
                ppNode->_pRight = subL;

            subL->_pParent = ppNode;
        }
    }

    bool _pathBlackCom(Node* pRoot, size_t pathBlack, const size_t& blackCount) {
        // pRootΪ��ʱ, �Ƚϼ���·���ͻ�׼·���ڽڵ���Ŀ
        if (pRoot == nullptr) {
            if (pathBlack != blackCount) {
                // �������·���ڽڵ���Ŀ �� ��׼·���ڽڵ���Ŀ����ʱ
                cout << "Υ�� ÿ��·���ڽڵ���Ŀ��� ����" << endl;
                return false;
            }
            return true;
        }

        if (pRoot->_color == BLACK)
            pathBlack++;

        if (pRoot->_color == RED && pRoot->_pParent && pRoot->_pParent->_color == RED) {
            // ��ǰ�ڵ� �Լ����׽ڵ�ͬʱΪ��ʱ
            cout << "Υ�� ���ܴ��������ĺ�ڵ� ����" << endl;
            return false;
        }

        return _pathBlackCom(pRoot->_pLeft, pathBlack, blackCount) &&
            _pathBlackCom(pRoot->_pRight, pathBlack, blackCount);		// �ݹ��ж���������
    }

    int _maxHeight(Node* pRoot) {
        if (pRoot == nullptr)
            return 0;

        int leftHeight = _maxHeight(pRoot->_pLeft);
        int rightHeight = _maxHeight(pRoot->_pRight);

        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }

    int _minHeight(Node* pRoot) {
        if (pRoot == nullptr)
            return 0;

        int leftHeight = _minHeight(pRoot->_pLeft);
        int rightHeight = _minHeight(pRoot->_pRight);

        return leftHeight < rightHeight ? leftHeight + 1 : rightHeight + 1;
    }

    Node* _root = nullptr;
};
