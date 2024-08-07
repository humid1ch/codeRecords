#pragma once
#include <iostream>
#include <cassert>
using namespace std;

// ö�ٳ���, ��ʾ �� ��
enum Color {
    RED,
    BLACK
};

template<class T>
struct RBTreeNode {
    // RedBlackTreeNode ������ڵ�
    RBTreeNode(const T& data = T())
        : _pLeft(nullptr), _pRight(nullptr), _pParent(nullptr)
        , _data(data)
        , _color(RED) 	// �½ڵ�Ĭ��Ϊ��ڵ� 
    {}

    RBTreeNode<T>* _pLeft;			// �ڵ�����
    RBTreeNode<T>* _pRight;		    // �ڵ��Һ���
    RBTreeNode<T>* _pParent;		// �ڵ㸸�׽ڵ�

    T _data;					     // �ڵ�����
    Color _color;					// �ڵ���ɫ
};

template<class Type, class Ref, class Ptr>			// ģ������ֱ��� ԭ���ͣ��������ͣ�ָ������
struct _RB_Tree_Iterator {
    typedef RBTreeNode<Type> Node;
    typedef _RB_Tree_Iterator<Type, Ref, Ptr> Self;

    _RB_Tree_Iterator(Node* node)
        : _node(node)
    {}

    Ref operator*() {
        return _node->_data;					// * ������ ���ؽڵ��е����ݡ�����ֵ���������� ��Ϊ��Ҫ�ṩ�޸Ĺ���
    }

    Ptr operator->() {
        return &_node->_data;					// -> ���ؽڵ������ݵĵ�ַ������ֵ��ָ������
    }
    
    Self& operator++() {        // �� int����, ǰ��++
        if (_node->_pRight == nullptr) {        // ������ָ��ڵ� û�� �Һ���
            // ��Ҫ�Ӵ˽ڵ������� ��һ�������Һ��ӵĽڵ�ĸ��׽ڵ�
            Node* cur = _node;
            Node* parent = cur->_pParent;

            while (parent && cur == parent->_pRight) {      // �����׽ڵ����, ��cur�ڵ㻹�����Һ���ʱ ѭ������
                cur = cur->_pParent;
                parent = parent->_pParent;
            }

            // �ߵ����� cur�ڵ���� ��һ����Ϊ�Һ��ӵĽڵ�
            _node = parent;
            // ��� ������++ ֮ǰ ָ������������һ���ڵ�, �� ִ����������֮��
            // cur���ߵ� �������ĸ��ڵ�, �����ڵ��parentΪ��
            // ���� ��������Ϊ��
        }
        else {              // ������ָ��Ľڵ� ����������
            // ��������������ڵ�
            Node* farLeft = _node->_pRight;
            while (farLeft->_pLeft) {
                farLeft = farLeft->_pLeft;
            }

            _node = farLeft;
        }

        return *this;                       // �����µĵ�����
    }

    Self& operator++(int) {                 // ����++
        Self tmp(*this);                    // �����˵�����

        ++(*this);                          // ����ǰ��++

        return tmp;                         // ����++ǰ �����ĵ�����
    }

    Self& operator--() {                     // �� int���� ǰ��--
        if (_node->_left == nullptr) {          // ��������ָ��Ľڵ� ���������� ʱ
            // ��Ҫ�Ӵ˽ڵ������� ��һ���������� �Ľڵ�ĸ��׽ڵ�
            Node* cur = _node;
            Node* parent = cur->_pParent;

            while (parent && cur == parent->_pLeft) {
                cur = cur->_pParent;
                parent = parent->_pParent;
            }

            // �ߵ����� cur�ڵ���� ��һ����Ϊ���ӵĽڵ�
            _node = parent;
        }
        else {               // ������ָ��ڵ� ���� ����ʱ
            // �������������ҽڵ�
            Node* subRight = _node->_pLeft;
            while (subRight->_pRight) {
                subRight = subRight->_pRight;
            }

            _node = subRight;
        }

        return *this;
    }

    Self& operator--(int) {
        Self tmp(*this);

        --(*this);

        return tmp;
    }

    bool operator!=(const Self& It) const {
        return _node != It._node;
    }

    bool operator==(const Self& It) const {
        return _node == It._node;
    }

    // ������Ҫ���������, ��������Ϊ���е�
    Node* _node;
};

template<class Key, class Value_type, class KeyOfValue>
class RB_Tree {
    typedef RBTreeNode<Value_type> Node;			// �Խڵ����ͽ���typedef

public:
    // ������Ҫ������ʹ��  ���� typedef Ϊ������
    typedef _RB_Tree_Iterator<Value_type, Value_type&, Value_type*> iterator;
    typedef _RB_Tree_Iterator<Value_type, const Value_type&, const Value_type*> const_iterator;

    iterator begin() {          // begin ȡ��������ڵ�
        Node* farLeft = _root;
        while (farLeft && farLeft->_pLeft) {
            // �ж� farLeft�Ƿ���� ��Ϊ�˷�ֹ ��Ϊ��ʱ farLeft->_pLeft ����ʹ�ÿ�ָ��
            farLeft = farLeft->_pLeft;
        }

        return iterator(farLeft);
    }
    iterator end() {
        return iterator(nullptr);
    }

    const_iterator begin() const {
        Node* farLeft = _root;
        while (farLeft && farLeft->_pLeft) {
            farLeft = farLeft->_pLeft;
        }

        return const_iterator(farLeft);
    }
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    iterator find(const Key& key) {
        //  ���ҽڵ� �ǰ��� key ���ҵ�
        Node* cur = _root;
        KeyOfValue KOV;
        while (cur) {
            if (KOV(cur->_data) < key) {
                cur = cur->_pRight;
            }
            else if (KOV(cur->_data) > key) {
                cur = cur->_pLeft;
            }
            else {
                return iterator(cur);
            }
        }

        return end();           // �ߵ��� ��û���ҵ�
    }
       
    // ��������֮�� Ҫ�����½ڵ�ĵ����� �� ������
    pair<iterator, bool> insert(const Value_type& data) {
        if (_root == nullptr) {
            // ��Ϊ��ʱ, �����½ڵ�
            _root = new Node(data);
            _root->_color = BLACK;			// ���ڵ�ҪΪ ��

            return make_pair(iterator(_root), true);
        }
        
        KeyOfValue KOV;
        // ����Ϊ��, �ʹӸ��ڵ㿪ʼ��λ��
        Node* cur = _root;
        Node* parent = cur->_pParent;
        while (cur) {
            if (KOV(data) > KOV(cur->_data)) {
                // �������ݴ�, ������������
                parent = cur;
                cur = cur->_pRight;
            }
            else if (KOV(data) < KOV(cur->_data)) {
                // ��������С, ������������
                parent = cur;
                cur = cur->_pLeft;
            }
            else {
                // �����Ѵ��ڲ�������, ���� false ����ʧ��
                return make_pair(iterator(cur), false);
            }
        }

        // ��ѭ��֮��, cur����λ�þ��� �½ڵ���Ҫ�����λ��
        cur = new Node(data);

        Node* newNode = cur;                // ��¼�½ڵ�λ�� �Է���

        cur->_color = RED;
        // parent �� cur��������
        if (KOV(data) > KOV(parent->_data)) {
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

        return make_pair(iterator(newNode), true);
    }

private:
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

    Node* _root = nullptr;
};
