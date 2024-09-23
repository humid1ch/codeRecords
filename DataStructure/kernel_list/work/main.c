#include "flyList.h"

#include <stdio.h>
#include <stdlib.h>

datatype fly[13] = {
	{"1001", "xxx", "yyy", "0818", 'A', 9, 12, 204.44},
	{"1002", "zzz", "ccc", "0818", 'C', 10, 12, 256.44},
	{"1003", "xxx", "hhh", "0818", 'D', 8, 10, 267.44},
	{"1004", "vvv", "yyy", "0818", 'C', 11, 15, 145.44},
	{"1005", "ddd", "eee", "0818", 'B', 9, 11, 244.44},
	{"1001", "xxx", "www", "0818", 'A', 9, 12, 204.44},
	{"1002", "zzz", "ccc", "0819", 'C', 10, 12, 256.44},
	{"1003", "xxx", "yyy", "0818", 'D', 8, 10, 267.44},
	{"1004", "vvv", "yyy", "0818", 'C', 11, 15, 145.44},
	{"1001", "xxx", "yyy", "0818", 'A', 9, 12, 204.44},
	{"1002", "zzz", "ccc", "0820", 'C', 10, 12, 256.44},
	{"1003", "xxx", "yyy", "0818", 'D', 8, 10, 267.44},
	{"1004", "vvv", "yyy", "0818", 'C', 11, 15, 145.44},
};

int main() {
	FlyList* aList = createList();

	for (int i = 0; i < 13; i++) {
		listAddTail(aList, &(fly[i]));
	}
	printf("原:\n");
	listShow(aList);

	FlyList* dateList = listFindByDate(aList, "0818");
	printf("班期:\n");
	listShow(dateList);
	FlyList* aaddrList = listFindByAaddr(aList, "yyy");
	printf("目的地:\n");
	listShow(aaddrList);
	FlyList* saddrList = listFindBySaddr(aList, "xxx");
	printf("始发地:\n");
	listShow(saddrList);
	FlyList* typeList = listFindByType(aList, 'A');
	printf("机型:\n");
	listShow(typeList);

	aList = listSortBySaddr(aList);
	printf("saddr排序:\n");
	listShow(aList);

	aList = listSortByStime(aList);
	printf("stime排序:\n");
	listShow(aList);

    aList = listSortByPrice(aList);
    printf("price排序:\n");
	listShow(aList);

	free(aList);
	free(aaddrList);
    free(dateList);
    free(saddrList);
    free(typeList);

	return 0;
}
