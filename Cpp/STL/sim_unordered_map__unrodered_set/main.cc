
#include "unordered_map.hpp"
#include "unordered_set.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>
using std::unordered_map;
using std::unordered_set;

void unorderTest() {
    July::unordered_map<std::string, int> fruitTable;
    July::unordered_set<std::string> table;

	fruitTable.insert(make_pair("苹果", 1));
	fruitTable.insert(make_pair("果苹", 1));
	fruitTable.insert(make_pair("pingguo", 1));
	fruitTable.insert(make_pair("guoping", 1));
	fruitTable.insert(make_pair("apple", 1));
	fruitTable.insert(make_pair("elppa", 1));
	fruitTable.insert(make_pair("桃子", 1));
	fruitTable.insert(make_pair("子桃", 1));
	fruitTable.insert(make_pair("taozi", 1));
	fruitTable.insert(make_pair("zitao", 1));
	fruitTable.insert(make_pair("peach", 1));
	fruitTable.insert(make_pair("hcaep", 1));
	fruitTable.insert(make_pair("水果", 1));

	table.insert("我套死你猴子");
	table.insert("子猴你死套我");
	table.insert("wotaosinihouzi");
	table.insert("izuohinisoatow");
	table.insert("碎碎平安");
	table.insert("岁岁平安");
	table.insert("平安碎碎");
	table.insert("平安岁岁");
	table.insert("pingansuisui");
	table.insert("suisuipingan");
	table.insert("iusiusnagnip");
	table.insert("嫩娘了个头啊");
	table.insert("啊头个了娘嫩");
	table.insert("nennianglegetoua");
}

int main() {
	unorderTest();

	return 0;
}
