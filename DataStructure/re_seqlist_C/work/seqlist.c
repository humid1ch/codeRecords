#include "seqlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bookSwap(Book* bk1, Book* bk2) {
	Book tmpBook;
	memcpy(&tmpBook, bk1, sizeof(Book));

	memcpy(bk1, bk2, sizeof(Book));

	memcpy(bk2, &tmpBook, sizeof(Book));
}

// 建议每次输入完 非字符串类型, 都清空一下输入缓冲区
void clearInputBuffer() {
	char c;
	while ((c = getchar()) != EOF && c != '\n')
		;
}

void setBook(Book* book) {
	printf("请输入书名:> ");
	scanf("%s", book->_name);
	printf("请输入数量:> ");
	scanf("%d", &book->_num);
	clearInputBuffer();
	printf("请输入价格:> ");
	scanf("%f", &book->_price);
	clearInputBuffer();
}

int booksInit(BookSL* books, size_t num) {
	assert(books);

	books->_books = NULL;
	Book* tmpBooks = (Book*)malloc(sizeof(Book) * num);
	if (tmpBooks == NULL) {
		printf("books init faild!\n");
		return -3;
	}

	books->_books = tmpBooks;
	books->_size = 0;
	books->_capacity = num;

	return 0;
}

int booksFree(BookSL* books) {
	assert(books);

	if (books->_books == NULL) {
		printf("Please init.\n");
		return -1;
	}

	free(books->_books);
	books->_books = NULL;
	books->_size = 0;
	books->_capacity = 0;

	return 0;
}

int booksPush(BookSL* books, const Book* book) {
	assert(books);

	if (books->_books == NULL) {
		printf("Please init.\n");
		return -1;
	}

	if (books->_size == books->_capacity) {
		size_t newCapacity = books->_size * 1.5;
		Book* tmpBooks = (Book*)realloc(books->_books, sizeof(Book) * newCapacity);
		if (tmpBooks == NULL) {
			printf("realloc faild\n");
			return -3;
		}

		books->_books = tmpBooks;
		books->_capacity = newCapacity;
	}

	// 赋值, 即添加Book数据
	strcpy(books->_books[books->_size]._name, book->_name);
	books->_books[books->_size]._num = book->_num;
	books->_books[books->_size]._price = book->_price;

	books->_size++;

	return 0;
}

//  尾删图书, 当前场景用不太到
int booksPop(BookSL* books) {
	assert(books);

	if (books->_books == NULL) {
		printf("Please init.\n");
		return -1;
	}
	if (books->_size <= 0) {
		printf("Nothing.\n");
		return -2;
	}

	books->_size--;

	return 0;
}

// 删除指定位置的图书, 一般来说, 还是查找名字再删除
// 所以可以考虑再封装
int booksErase(BookSL* books, size_t pos) {
	assert(books);
	assert(pos >= 0 && pos < books->_size);
	if (books->_books == NULL) {
		printf("Please init.\n");
		return -1;
	}
	if (books->_size == 0) {
		printf("Nothing.\n");
		return -2;
	}

	// 无移动数据版
	if (pos != books->_size - 1) {
		bookSwap(books->_books + pos, books->_books + books->_size - 1);
	}
	books->_size--;

	return 0;
}

// 按照书名查找图书
size_t booksFind(const BookSL* books, const char* name) {
	assert(books);
	assert(name);

	for (int i = 0; i < books->_size; i++) {
		if (!strcmp(books->_books[i]._name, name)) {
			return i;
		}
	}

	return -2;
}

// 修改书信息? 应该也是由位置修改, 但是应该先根据书名找到位置
// 考虑再封装
void booksModify(BookSL* books, size_t pos) {
	assert(books);
	assert(pos >= 0 || pos < books->_size);

	while (1) {
		printf("修改图书信息\n");
		printf("1. 修改书名\n");
		printf("2. 修改数量\n");
		printf("3. 修改价格\n");
		printf("0. 退出修改\n");

		printf("请选择:> ");
		int index = 0;
		scanf("%d", &index);
		clearInputBuffer();

		char name[20];
		int num = 0;
		float price = 0.0;
		char sure = 0;
		switch (index) {
		case 1:
			printf("请输入新书名:> ");
			scanf("%s", name);
			printf("是否确认修改(Y/N):> ");
			scanf("%c", &sure);
			clearInputBuffer();
			if (sure == 'Y') {
				strcpy(books->_books[pos]._name, name);
				printf("修改成功\n");
			}
			break;
		case 2:
			printf("请输入新数量:> ");
			scanf("%d", &num);
			clearInputBuffer();
			printf("是否确认修改(Y/N):> ");
			scanf("%c", &sure);
			clearInputBuffer();
			if (sure == 'Y') {
				books->_books[pos]._num = num;
				printf("修改成功\n");
			}
			break;
		case 3:
			printf("请输入新价格:> ");
			scanf("%f", &price);
			clearInputBuffer();
			printf("是否确认修改(Y/N):> ");
			scanf("%c", &sure);
			clearInputBuffer();
			if (sure == 'Y') {
				books->_books[pos]._price = price;
				printf("修改成功\n");
			}
			break;
		case 0:
			return;
		default:
			printf("选项非法, 请重新输入\n");
			break;
		}
	}
}

// 传入顺序表和未赋值的书变量
void booksAddBook(BookSL* books, Book* book) {
	assert(books);

	setBook(book);
	printf("\n");

	booksPush(books, book);
}

void booksModifyByName(BookSL* books) {
	assert(books);

	char name[20] = {0};
	printf("请输入要修改书的书名:> ");
	scanf("%s", name);

	int pos = booksFind(books, name);
	if (pos < 0) {
		printf("未找到 《%s》\n", name);
		return;
	}

	booksModify(books, pos);
	printf("\n");
}

void booksEraseByName(BookSL* books) {
	assert(books);

	char name[20] = {0};
	printf("请输入要删除书的书名:> ");
	scanf("%s", name);

	int pos = booksFind(books, name);
	if (pos < 0) {
		printf("未找到 《%s》\n", name);
		return;
	}

	booksErase(books, pos);
	printf("\n");
}

void ascendSortByPrice(BookSL* books) {
	int gap = books->_size;

	while (gap > 1) {
		gap = gap / 3 + 1;
		for (int i = 0; i < books->_size - gap; i++) {
			int end = i;
			Book cur = books->_books[end + gap];
			float curPrice = cur._price;
			while (end >= 0) {
				if (curPrice < books->_books[end]._price) {
					strcpy(books->_books[end + gap]._name, books->_books[end]._name);
					books->_books[end + gap]._num = books->_books[end]._num;
					books->_books[end + gap]._price = books->_books[end]._price;

					end -= gap;
				}
				else {
					break;
				}
			}
			strcpy(books->_books[end + gap]._name, cur._name);
			books->_books[end + gap]._num = cur._num;
			books->_books[end + gap]._price = cur._price;
		}
	}
}

void descendSortByPrice(BookSL* books) {
	int gap = books->_size;

	while (gap > 1) {
		gap = gap / 3 + 1;
		for (int i = 0; i < books->_size - gap; i++) {
			int end = i;
			Book cur = books->_books[end + gap];
			float curPrice = cur._price;
			while (end >= 0) {
				if (curPrice > books->_books[end]._price) {
					strcpy(books->_books[end + gap]._name, books->_books[end]._name);
					books->_books[end + gap]._num = books->_books[end]._num;
					books->_books[end + gap]._price = books->_books[end]._price;

					end -= gap;
				}
				else {
					break;
				}
			}
			strcpy(books->_books[end + gap]._name, cur._name);
			books->_books[end + gap]._num = cur._num;
			books->_books[end + gap]._price = cur._price;
		}
	}
}

void booksShow(const BookSL* books) {
	assert(books);

	printf("书名\t\t数量\t价格\n");
	for (int i = 0; i < books->_size; i++) {
		if (strlen(books->_books[i]._name) > 4) {
			printf("%s\t%d\t%.2f\n", books->_books[i]._name, books->_books[i]._num,
				   books->_books[i]._price);
		}
		else {
			printf("%s\t\t%d\t%.2f\n", books->_books[i]._name, books->_books[i]._num,
				   books->_books[i]._price);
		}
	}
}
