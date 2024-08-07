#include "K-V-BinarySearchTree.h"

void TestKVBSTree1()
{
	KVBSTree<string, string> dict;
	dict.Insert("insert", "����");
	dict.Insert("erase", "ɾ��");
	dict.Insert("left", "���");
	dict.Insert("string", "�ַ���");

	string str;
	while (cin >> str)
	{
		auto ret = dict.Find(str);
		if (ret)
		{
			cout << str << ":" << ret->_val << endl;
		}
		else
		{
			cout << "����ƴд����" << endl;
		}
	}

	string strs[] = { "ƻ��", "����", "ƻ��", "ӣ��", "ƻ��", "ӣ��", "ƻ��", "ӣ��", "ƻ��", "����", "ӣ��", "ƻ��", "����"
	, "ӣ��", "ƻ��", "����", "ӣ��", "ƻ��", "����", "ӣ��", "ƻ��", "����", "ӣ��", "ƻ��", "����", "ӣ��", "ƻ��", "����", "ӣ��"
	, "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" , "ƻ��"
	, "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" , "ƻ��", "����", "ӣ��" };
	// ͳ��ˮ�����ֵĴ�
	// ˼·�ǣ�
	// ����strs��ͬʱ���������Ƿ���ڵ�ǰˮ��
	//	1. �����ڣ��� ����ˮ�� �Ҷ�ӦvalΪ1
	//	2. ���ڣ��� ��Ӧ val++
	// strs����һ��֮�����о�ͳ���� �����༰����
	//
	KVBSTree<string, int> countTree;
	for (auto str : strs)
	{
		auto ret = countTree.Find(str);
		if (ret == NULL)
		{
			countTree.Insert(str, 1);
		}
		else
		{
			ret->_val++;
		}
	}
	countTree.InOrder();
}

void TestKVBSTree2()
{
	KVBSTree<int, char> kvBst1;
	kvBst1.Insert(1, 'a');
	kvBst1.Insert(3, 'c');
	kvBst1.Insert(2, 'b');
	kvBst1.Insert(9, 'i');
	kvBst1.Insert(4, 'd');
	kvBst1.Insert(8, 'g');
	kvBst1.Insert(6, 'f');
	kvBst1.Insert(5, 'e');
	kvBst1.Insert(10, 'j');
	kvBst1.InOrder();

	KVBSTree<int, char> kvBst2(kvBst1);
	kvBst2.InOrder();
	kvBst2.Erase(1);
	kvBst2.Erase(5);
	kvBst2.Erase(9);
	kvBst2.InOrder();


	KVBSTree<int, char> kvBst3;
	kvBst3 = kvBst2;
	kvBst3.InOrder();
	kvBst3.Insert(5, 'e');
	kvBst3.Insert(9, 'i');
	kvBst3.InOrder();
}
int main()
{
	TestKVBSTree1();
	TestKVBSTree2();

	return 0;
}