#ifndef _SHOP_H
#define _SHOP_H 1

typedef struct Shop {
	char _name[20]; // 商品名
	int _rest;		// 库存
	int _id;		// 商品编号
	char _addr[20]; // 出产地
	float _price;	// 价格
} Shop;

// 用于存储购买了什么商品
typedef struct Buy {
	int _id;
	int _num;
} Buy;

typedef struct Gift {
    char _name[20];
    int _id;
    int _useInteg;
} Gift;

void shopInit();

float howMuchNoVIP(Buy* buys);

void printShopInfo(const Shop* shop);

void showShops();
// 购物结算
void shoppingCheckout();
// 福利
void welfare();

#endif
