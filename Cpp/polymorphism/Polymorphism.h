#pragma once
#include <iostream>
using namespace std;

//
//class Person
//{
//public:
//	virtual Person* buyTicket() {
//		cout << "һ������ˣ���Ʊ    ȫ�ۡ�������80��" << endl;
//		return this;
//	}
//};
//
//class Student : public Person
//{
//public:
//	virtual Student* buyTicket(){
//		cout << "��Уѧ��  ����Ʊ    ��ۡ�������40��" << endl;
//		return this;
//	}
//};
//
//class Elderly : public Person
//{
//	virtual Elderly* buyTicket(){
//		cout << "������    ����Ʊ    ��Ʊ��������0��" << endl;
//		return this;
//	}
//};

//class Person
//{
//public:
//	virtual Person& buyTicket() {
//		cout << "һ������ˣ���Ʊ    ȫ�ۡ�������80��" << endl;
//		return *this;
//	}
//};
//
//class Student : public Person
//{
//public:
//	virtual Student& buyTicket() {
//		cout << "��Уѧ��  ����Ʊ    ��ۡ�������40��" << endl;
//		return *this;
//	}
//};
//
//class Elderly : public Person
//{
//	virtual Elderly& buyTicket() {
//		cout << "������    ����Ʊ    ��Ʊ��������0��" << endl;
//		return *this;
//	}
//};
//
//class Soldier : public Person
//{
//public:
//	virtual Soldier& buyTicket() {
//		cout << "����      ����Ʊ    ����Ԥ����Ʊ������ۡ�������40��" << endl;
//		return *this;
//	}
//};

//void BuyTicket(Person& per)
//{
//	per.buyTicket();
//}


//class Person
//{
//public:
//	virtual ~Person() {
//		cout << "~Person():: ����Person����" << endl;
//	}
//};
//
//class Student : public Person
//{
//public:
//	virtual ~Student(){
//		cout << "~Student():: ����Student����" << endl;
//	}
//};
//
//class Elderly : public Person
//{
//public:
//	virtual ~Elderly() {
//		cout << "~Elderly():: ����Elderly����" << endl;
//	}
//};
//
//class Soldier : public Person
//{
//public:
//	virtual ~Soldier() {
//		cout << "~Soldier():: ����Soldier����" << endl;
//	}
//};
//
//void _Destructor(Person* per)
//{
//	delete per;
//	cout << endl;
//}

//class Base1 {
//public:
//	virtual void func1() {
//		cout << "Base1::func1" << endl;
//	}
//	virtual void func2() {
//		cout << "Base1::func2" << endl;
//	}
//private:
//	int b1;
//};
//
//class Base2 {
//public:
//	virtual void func1() {
//		cout << "Base2::func1" << endl;
//	}
//	virtual void func2() {
//		cout << "Base2::func2" << endl;
//	}
//private:
//	int b2;
//};
//
//class Derive : public Base1, public Base2 {
//public:
//	virtual void func1() {
//		cout << "Derive::func1" << endl;
//	}
//	virtual void func3() {
//		cout << "Derive::func3" << endl;
//	}
//private:
//	int d1;
//};
//
//typedef void(*VFPTR) ();
//void PrintVTable(VFPTR vTable[])
//{
//	cout << " ����ַ>" << vTable << endl;
//	for (int i = 0; vTable[i] != nullptr; ++i)
//	{
//		printf(" ��%d���麯����ַ :0X%x,->", i, vTable[i]);
//		VFPTR f = vTable[i];
//		f();
//	}
//	cout << endl;
//}
//
//class Base3
//{
//public:
//	virtual void test1()
//	{
//		cout << "test1" << endl;
//	}
//
//	void test2()
//	{
//		cout << "test2" << endl;
//	}
//};
//

//class A {
//public:
//	A(const char* s) {
//		cout << s << endl;
//	}
//
//	~A() {}
//};
//class B :virtual public A
//{
//public:
//	B(const char* s1, const char* s2)
//		:A(s1)
//	{
//		cout << s2 << endl;
//	}
//};
//
//class C :virtual public A
//{
//public:
//	C(const char* s1, const char* s2)
//		:A(s1)
//	{
//		cout << s2 << endl;
//	}
//};
//
//class D :public B, public C
//{
//public:
//	D(const char* s1, const char* s2, const char* s3, const char* s4)
//		: B(s1, s2)
//		, C(s1, s3)
//		, A(s1)
//	{
//		cout << s4 << endl;
//	}
//};

//class Base
//{
//public:
//	virtual void Func1() {
//		cout << "Base1:: Func1()" << endl;
//	}
//	void Func2() {
//		cout << "Base1:: Func2()" << endl;
//	}
//	virtual void Func3() {
//		cout << "Base1:: Func3()" << endl;
//	}
//
//private:
//	int _b1 = 1;
//};
//
//class Advanced :public Base
//{
//public:
//	virtual void Func1() {
//		cout << "Advanced:: Func1()" << endl;
//	}
//	void Func2() {
//		cout << "Advanced:: Func2()" << endl;
//	}
//
//private:
//	int _b2 = 2;
//};
//


// �о���̬����ԭ��
//class Clothes
//{
//public:
//	virtual void Wear() = 0;
//};
//
//class Jacket : public Clothes
//{
//public:
//	virtual void Wear()
//	{
//		cout << "����" << endl;
//	}
//};
//
//class Pants : public Clothes
//{
//public:
//	virtual void Wear()
//	{
//		cout << "����" << endl;
//	}
//};


// �о���̳е����
//class Base1 {
//public:
//    virtual void func1() {
//        cout << "Base1:: func1" << endl;
//    }
//    virtual void func2() {
//        cout << "Base1:: func2" << endl;
//    }
//private:
//    int b1;
//    int b4;
//    int b3;
//};
//
//class Base2 {
//public:
//    virtual void func1() {
//        cout << "Base2:: func1" << endl;
//    }
//    virtual void func2() {
//        cout << "Base2:: func2" << endl;
//    }
//private:
//    int b2;
//};
//
//class Advanced : public Base1, public Base2 {
//public:
//    virtual void func1() {
//        cout << "Advanced:: func1" << endl;
//    }
//    virtual void func3() {
//        cout << "Advanced:: func3" << endl;
//    }
//private:
//    int d1;
//};

//class Elementary
//{
//public:
//	virtual void func1(){
//		cout << "Elementary:: func1" << endl;
//	}
//	virtual void func2() {
//		cout << "Elementary:: func2" << endl;
//	}
//	virtual void func3() {
//		cout << "Elementary:: func3" << endl;
//	}
//
//private:
//	int _elem;
//};
//
//class Intermediate1 : virtual public Elementary
//{
//public:
//	virtual void func1() {
//		cout << "Intermediate1:: func1" << endl;
//	}
//
//private:
//	int _inte1;
//};
//
//class Intermediate2 : virtual public Elementary
//{
//public:
//	virtual void func2() {
//		cout << "Intermediate2:: func2" << endl;
//	}
//
//private:
//	int _inte2;
//};
//
//class Advanced : public Intermediate1, public Intermediate2
//{
//public:
//	virtual void func1() {
//		cout << "Advanced:: func1" << endl;
//	}
//	void func3() {
//		cout << "Advanced:: func3" << endl;
//	}
//	virtual void func4() {
//		cout << "Advanced:: func4" << endl;
//	}
//private:
//	int _adv;
//};

class Elementary
{
public:
	Elementary()
		: _elem(1)
	{}

	virtual inline void func1() {
		cout << "Elementary:: func1" << endl;
	}
	virtual void func2();

	static void func3()
	{
		cout << "Elementary:: static func3" << endl;
	}

private:
	int _elem;
};

void Elementary::func2() {
	cout << "Elementary:: func2" << endl;
}

class Intermediate : public Elementary
{
	virtual inline void func1() {
		cout << "Intermediate:: func1" << endl;
	}
	virtual void func2();
};

void Intermediate::func2() {
	cout << "Intermediate:: func2" << endl;
}