#include "unordered_map.h"
#include "unordered_set.h"
#include <unordered_map>
#include <unordered_set>
using std::unordered_map;
using std::unordered_set;

void myUnorderTest() {
	July::unordered_map<string, int> fruitTable;
	July::unordered_set<string> table;

	fruitTable.insert(make_pair("ƻ��", 1));
	fruitTable.insert(make_pair("��ƻ", 1));
	fruitTable.insert(make_pair("pingguo", 1));
	fruitTable.insert(make_pair("guoping", 1));
	fruitTable.insert(make_pair("apple", 1));
	fruitTable.insert(make_pair("elppa", 1));
	fruitTable.insert(make_pair("����", 1));
	fruitTable.insert(make_pair("����", 1));
	fruitTable.insert(make_pair("taozi", 1));
	fruitTable.insert(make_pair("zitao", 1));
	fruitTable.insert(make_pair("peach", 1));
	fruitTable.insert(make_pair("hcaep", 1));
	fruitTable.insert(make_pair("ˮ��", 1));

	table.insert("�����������");
	table.insert("�Ӻ���������");
	table.insert("wotaosinihouzi");
	table.insert("izuohinisoatow");
	table.insert("����ƽ��");
	table.insert("����ƽ��");
	table.insert("ƽ������");
	table.insert("ƽ������");
	table.insert("pingansuisui");
	table.insert("suisuipingan");
	table.insert("iusiusnagnip");
	table.insert("�����˸�ͷ��");
	table.insert("��ͷ��������");
	table.insert("nennianglegetoua");
}

int main() {
	myUnorderTest();

	return 0;
}