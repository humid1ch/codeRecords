#pragma once

#include <stdio.h>

typedef struct Book {
    char _name[20];
    int _num;
    float _price;
} Book;

/* 有关BookSeqList的接口
 * 返回值为-1, 表示BookSeqList 未初始化
 * 返回值为-2, 表示BookSeqList 缺少数据, 一般在删除 或 查找出现
 * 返回值为-3, 表示内存开辟错误 malloc 和 realloc
 * */
typedef struct BookSeqList {
    Book* _books;
    size_t _size;
    size_t _capacity;
} BookSL;

// 初始化BookSL中Book* 数组
int booksInit(BookSL* books, size_t num);

// 释放BookSL中Book* 的空间
int booksFree(BookSL* books);

// 向存储Book的顺序表中, 插入Book变量
int booksPush(BookSL* books, const Book* book);

//  尾删图书, 当前场景用不太到
int booksPop(BookSL* books);

// 删除指定位置的图书, 一般来说, 还是查找名字再删除
// 所以可以考虑再封装
int booksErase(BookSL* books, size_t pos);

// 按照书名查找图书
size_t booksFind(const BookSL* books, const char* name);

// 修改书信息? 应该也是由位置修改
// 考虑在封装
void booksModify(BookSL* books, size_t pos);

// 传入顺序表和未赋值的书变量
void booksAddBook(BookSL* books, Book* book);

// 通过书名修改书本
void booksModifyByName(BookSL* books);

// 通过书名删除书本
void booksEraseByName(BookSL* books);

// 价格升序排序
void ascendSortByPrice(BookSL* books);

// 价格降序排序
void descendSortByPrice(BookSL* books);

void booksShow(const BookSL* books);
