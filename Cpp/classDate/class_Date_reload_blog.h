#pragma once

#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

class Date
{
public:
	// ȫȱʡ���캯��
	Date(int year = 1900, int month = 1, int day = 1);
	bool isLeapYear(int year)
	{// �����ж�
		return (year % 4 == 0 && year % 100 != 0)
			|| (year % 400 == 0);
	}
	int GetMonthDay(int year, int month); // ÿ��������ȡ

	// ���� >
	bool operator>(const Date& d)
	{
		return (_year > d._year
			|| (_year == d._year && _month > d._month)
			|| (_year == d._year && _month == d._month && _day > d._day));
	}
	// ���� ==
	bool operator==(const Date& d)
	{
		return (_year == d._year && _month == d._month && _day == d._day);
	}
	// ���� >=
	bool operator>=(const Date& d)
	{
		return  (*this > d || *this == d);
	}
	// ���� !=
	bool operator!=(const Date& d)
	{
		return  !(*this == d);
	}
	// ���� <
	bool operator<(const Date& d)
	{
		return  !(*this >= d);
	}
	// ���� <=
	bool operator<=(const Date& d)
	{
		return  (*this < d || *this == d);
	}

	Date& operator=(const Date& d)
	{
		if (this != &d)
		{
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}

		return *this;
	}

	Date operator+(int day)
	{
		Date ret = *this;
		ret += day;

		return ret;
	}
	Date& operator+=(int day)
	{
		if (day < 0)
			*this -= -day;

		_day += day;
		while (_day > GetMonthDay(_year, _month))
		{
			_day -= GetMonthDay(_year, _month);
			_month++;
			if (_month == 13)
			{
				_year++;
				_month = 1;
			}
		}

		return *this;
	}

	Date operator-(int day)
	{
		Date ret = *this;
		ret -= day;

		return ret;
	}

	Date& operator-=(int day)
	{
		if (day < 0)
			*this += -day;

		while (day >= _day)
		{
			day -= _day;
			--_month;
			if (_month == 0)
			{
				--_year;
				_month = 12;
			}
			_day = GetMonthDay(_year, _month);
		}
		_day -= day;

		return *this;
	}

	Date& operator--()		// ǰ��--
	{
		*this -= 1;

		return *this;
	}
	Date operator--(int i)	// ����--
	{
		Date ret = *this;
		*this -= 1;

		return ret;
	}
	Date& operator++()		// ǰ��--
	{
		*this += 1;

		return *this;
	}
	Date operator++(int i)	// ����--
	{
		Date ret = *this;
		*this += 1;

		return ret;
	}

	int operator-(const Date& d)
	{
		Date max = *this;
		Date min = d;
		int flag = 1;

		if (*this < d)
		{
			max = d;
			min = *this;
			flag = -1;
		}
		int count = 0;
		while (min < max)
		{
			min++;
			count++;
		}

		return count * flag;
	}

	void print()
	{
		cout << _year << "-" << _month << "-" << _day << endl;
	}

private:
	int _year;
	int _month;
	int _day;
};
