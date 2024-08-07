#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

struct Point {
    int _x;
    int _y;
};

class Date {
public:
    Date(int year, int month, int day)
        : _year(year), _month(month), _day(day) {
        cout << "Date(int year, int month, int day)" << endl;
    }

private:
    int _year;
    int _month;
    int _day;
};

int main() {
    int a = 1;
    int b = {2};
    int c{3};

    Point po1 = {1, 2};
    Point po2{1, 2};

    int array1[] = {1, 2, 3, 4, 5};
    int array2[5]{1, 2, 3, 4, 5};

    // Date d1(2022, 1, 1);

    // // C++11支持的列表初始化，这里会调用构造函数初始化
    // Date d2{2022, 1, 2};
    // Date d3 = {2022, 1, 3};

    vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    vector<int>* v3 = new vector<int>[4] { {1,2,3,4},{5,6,7,8},{9,10,11,12},{12,13,14,15} };

    auto li = {1, 2, 3, 4, 5};
    // cout << typeid(li).name() << endl;

    set<int> s1{1, 2, 3, 4, 5, 6, 7};
    set<int> s2 = {1, 2, 3, 4, 5, 6, 7};

    map<string, string> dict ={ {"apple", "苹果"}, {"banana", "香蕉"}, {"sun", "太阳"} };

    int e = 1;
    auto f = 3.3;
    // cout << typeid(f).name() << endl;

    decltype(2 * 2.2) m;
    // cout << typeid(m).name() << endl;

    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (auto e : v) {
        cout << e << " ";
    }

    return 0;
}
