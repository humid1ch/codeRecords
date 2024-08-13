#include "../inc/shop.h"
#include "../inc/man.h"
#include "../inc/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_SHOP 50
// 商品
Shop* shops;
Buy* buys;

int buyNum = 0;
int shopNum = 5;

Gift gifts[20] = {{"救赎", 1, 10}, {"日炎", 2, 10}, {"泰坦", 3, 10}, {"水银", 4, 10}, {"偷偷", 5, 10}, {"反曲弓", 6, 10}, {"青龙刀", 7, 10}, {"复活甲", 8, 10}, {"大天使", 9, 10}, {"破甲弓", 10, 10}, {"心之钢", 11, 10}, {"羊刀", 12, 10}, {"科技枪", 13, 10}, {"公理圆瓜", 14, 10}, {"金铲铲", 15, 10}, {"巨杀", 16, 10}, {"红霸符", 17, 10}, {"鬼书", 18, 10}, {"大法帽", 19, 10}, {"蓝霸符", 20, 10}};

Shop defaultShops[5] = {{"灯笼", 23, 1, "峡谷上路", 70.00},
						{"肉蛋鸡", 45, 2, "峡谷上路", 50.00},
						{"红皮鸭", 67, 3, "芜湖厂牌", 43.33},
						{"手里剑", 89, 4, "峡谷下路", 75.00},
						{"金身", 99, 5, "峡谷下路", 3250.00}};

// 初始化商品 和 购买列表
void shopInit() {
	shops = (Shop*)malloc(sizeof(Shop) * MAX_SHOP);
	memset(shops, 0, sizeof(Shop) * MAX_SHOP);

	buys = (Buy*)malloc(sizeof(Buy) * MAX_SHOP);
	memset(buys, 0, sizeof(Shop) * MAX_SHOP);

	// i + 1 即为实际id
	for (int i = 0; i < shopNum; i++) {
		shops[i]._id = defaultShops[i]._id;
		shops[i]._rest = defaultShops[i]._rest;
		shops[i]._price = defaultShops[i]._price;
		strcpy(shops[i]._name, defaultShops[i]._name);
		strcpy(shops[i]._addr, defaultShops[i]._addr);
	}
}

float howMuchNoVIP(Buy* buys) {
	float pay = 0.0;

	// i + 1 即为实际id
	for (int i = 0; i < shopNum; i++) {
		pay += shops[buys[i]._id - 1]._price * buys[i]._num;
	}

	return pay;
}

// 很短 不需要可以直接不用函数
float addInteg(Man* man, float pay) {
	// 积分按消费的百分之一算
	man->_integ += pay / 100.0;

	return pay / 100.0;
}

// 打印商品信息
void printShopInfo(const Shop* shop) {
	printf("%d\t%s\t\t%d\t%s\t\t%f\n", shop->_id, shop->_name, shop->_rest,
		   shop->_addr, shop->_price);
}

void showShops() {
	printf("编号\t商品名\t\t库存\t生产地\t\t价格\n");
	for (int i = 0; i < shopNum; i++) {
		printShopInfo(shops + i);
	}
	printf("\n");
}

void shoppingList(Buy* buyss) {
	printf("=======================购物清单=======================\n");
	printf("编号\t商品名\t\t价格\t购入数量\t商品总价\n");
	for (int i = 0; i < buyNum; i++) {
		Shop good = shops[buyss[i]._id - 1]; // 商品id-1 是商品在shops中存储的位置
		printf("%d\t%s\t\t%.2f\t%d\t\t%.2f\n", good._id, good._name, good._price,
			   buyss[i]._num, good._price * buyss[i]._num);
	}
	printf("\n");
}

// 购物结算
void shoppingCheckout() {
	showShops();
	printf("\n=========购物管理系统 > 主菜单 > 购物结算=========\n");

	int isVIP = 0;
	int inputCnt = 0;
	int isContinue = 1;
	int manID = -1;
	float discount = 1; // 折扣
	Man* nowMan = NULL;
	printf("客户是否是会员?(1/0):> ");
	scanf("%d", &isVIP);
	clearInputBuffer();

	if (isVIP) {
		printf("请输入客户号:> ");
		scanf("%d", &manID);
		clearInputBuffer();
		nowMan = queryManByID(manID); // 是否找到会员信息
		if (nowMan == NULL) {
			printf("抱歉, 没有查询到该会员\n");
			printf("可能无法享受折扣\n");
			isVIP = 0; // 不是会员
		}
	}

	// buys 已经初始化了
	while (isContinue) {
		int goodID = 0;
		int goodNum = 0;
        int existGood = 0;
        int goodIndex = inputCnt;
		printf("请输入商品编号:> ");
		scanf("%d", &goodID);
		clearInputBuffer();
		printf("请输入购买数量:> ");
		scanf("%d", &goodNum);
		clearInputBuffer();

        for (int i = 0; i < inputCnt; i++) {
            if (goodID == buys[i]._id) {
                existGood = 1;              // 已经输入过相同的商品
                goodIndex = i;
            }
        }

        // 之前没有输入过此商品
        if (!existGood) {
	    	buys[inputCnt]._id = goodID;
    		buys[inputCnt]._num = goodNum;
		    inputCnt++;
        }
        else {
    		buys[goodIndex]._num += goodNum;
        }

		// 减库存
		shops[goodID - 1]._rest -= goodNum;
		// 这里不用在意库存是否足够, 因为shopping肯定是 拿着东西去前台的
		// 既然拿着 肯定是够的,

		printf("是否继续输入?(1/0):> ");
		scanf("%d", &isContinue);
		clearInputBuffer();
	}

	buyNum = inputCnt; // 买东西的种类

	if (inputCnt >= 3) {
		isVIP = 1; // 买超过三种商品, 就当VIP对待
	}

	if (isVIP) {
		discount = 0.8; // VIP打折
	}

	float oriPay = howMuchNoVIP(buys);
	float vipPay = oriPay * discount;

	float payment = 0.0;
	shoppingList(buys);

	printf("购物总金额: %.2f\n", oriPay);
	printf("打折后所需金额: %.2f\n", vipPay);
	while (1) {
		printf("实际交款金额为:> ");
		scanf("%f", &payment);
		clearInputBuffer();

        if (payment - vipPay >= 0) {
            break;
        }
        else {
            printf("牢弟, 这对吗? 钱够吗?\n");
        }
	}

	printf("应找回: %.2f\n", payment - vipPay);
	if (nowMan != NULL) { // 如果是会员, 则积分
		printf("客户号 %d 客户本次积分: %.1f\n", manID, addInteg(nowMan, vipPay));
		printf("客户剩余积分: %.1f\n", nowMan->_integ);
	}
	printf("======================================================\n");
}

void showGifts() {
	printf("积分礼品列表(每个礼品仅限兑换一份)\n");
	for (int i = 0; i < 19; i += 2) {
		printf("礼品编号: %d\t礼品名: %s\t积分: %d ", gifts[i]._id, gifts[i]._name,
			   gifts[i]._useInteg);
		printf("礼品编号: %d\t礼品名: %s\t积分: %d\n", gifts[i + 1]._id,
			   gifts[i + 1]._name, gifts[i + 1]._useInteg);
	}
}

void integShop() {
	while (1) {
		printf("\n==购物管理系统 > 主菜单 > 真情回馈 > 积分兑好礼==\n");
		showGifts();
		Man* tmpMan = NULL;
		int tmpId = 0;
		printf("请输入您的客户号:> ");
		scanf("%d", &tmpId);
		clearInputBuffer();
		tmpMan = queryManByID(tmpId);
		if (tmpMan == NULL) {
			printf("未查询到客户号为 %d 的客户\n", tmpId);
			continue;
		}

		long long tmpPhone = 0;
		printf("请输入您登记的手机号:> ");
		scanf("%lld", &tmpPhone);
		clearInputBuffer();

		if (tmpMan->_phone != tmpPhone) {
			printf("手机号错误, 请重新输入验证\n");
			continue;
		}

		printf("您的积分余额为: %.1f\n", tmpMan->_integ);

		int isContinue = 1;
		int getGifts[20] = {0};
		while (isContinue) {
			int giftId = -1;
			printf("请输入你要兑换的礼品编号:> ");
			scanf("%d", &giftId);
			clearInputBuffer();

			if (getGifts[giftId - 1] == 1) {
				printf("每件礼品仅限选择一份, 请重新选择\n");
				continue;
			}

			getGifts[giftId - 1] = 1;

			printf("是否继续选择(1/0):> ");
			scanf("%d", &isContinue);
			clearInputBuffer();
		}

		printf("您选择的礼品有:\n");
		int payInteg = 0;
		for (int i = 0; i < 20; i++) {
			if (getGifts[i]) {
				printf("%s(%d积分) * 1\n", gifts[i]._name, gifts[i]._useInteg);
				payInteg += gifts[i]._useInteg;
			}
		}

		if (payInteg > tmpMan->_integ) {
			printf("共需消耗 %d 积分, 您的积分不足\n", payInteg);
		}
		else {
			printf("共消耗 %d 积分, 兑换成功\n", payInteg);
			printf("剩余积分 %.1f\n", tmpMan->_integ -= payInteg);
		}

		int isExchange = 0;
		printf("是否继续兑换?(1/0):> ");
		scanf("%d", &isExchange);
		clearInputBuffer();

		if (!isExchange) {
			printf("退出兑换\n");
			break;
		}
	}
}

int lotteryOnce() {
	srand(time(0));

	// 先让客户号码固定为1
	// 再随机生成3个0~2的号码, 表示中奖号码
	// 如果中奖了, 再随机生成一个0~9的号码, 用来选择奖励
	int manNum = 1;
	int winNums[3] = {rand() % 3, rand() % 3, rand() % 3};
	int isWin = 0;

	for (int i = 0; i < 3; i++) {
		if (manNum == winNums[i]) {
			isWin++;
		}
	}

	return isWin; // 返回一次抽奖中奖的次数
}

void lotteryReward(int* rewards, int winCnt) {
	srand(time(0));
	/* 1234  随机一件基础装备
   * 56    随即两件基础装备
   * 78    随机一件成装
   * 9    随机一件光明装备
   * */

	for (int i = 0; i < winCnt; i++) {
		rewards[i] = rand() % 10;
	}
}

void integLottery() {
	while (1) {
		printf("\n==购物管理系统 > 主菜单 > 真情回馈 > 积分大抽奖==\n");
		Man* tmpMan = NULL;
		int lottery = 0;
		int tmpId = 0;
		printf("请输入您的客户号:> ");
		scanf("%d", &tmpId);
		clearInputBuffer();

		tmpMan = queryManByID(tmpId);
		if (tmpMan == NULL) {
			printf("未查询到客户号为 %d 的客户\n", tmpId);
			continue;
		}

		long long tmpPhone = 0;
		printf("请输入您登记的手机号:> ");
		scanf("%lld", &tmpPhone);
		clearInputBuffer();
		if (tmpMan->_phone != tmpPhone) {
			printf("手机号错误, 请重新输入验证\n");
			continue;
		}

		printf("您的积分余额为: %.1f\n", tmpMan->_integ);

		printf("抽奖50积分一次, 每次最多获得3个奖品(无保底), 您可抽 %d 次\n",
			   (int)tmpMan->_integ / 50);

		printf("请输入抽奖次数:> ");
		scanf("%d", &lottery);
		if ((int)tmpMan->_integ / 50 < lottery) {
			printf("您的积分不足, 无法进行抽奖\n");
			continue;
		}
		else {
			// 减积分
			tmpMan->_integ -= lottery * 50;
			int winCnt = 0;
			for (int i = 0; i < lottery; i++) {
				winCnt += lotteryOnce();
			}
			int rewards[winCnt];
			lotteryReward(rewards, winCnt);

			printf("抽奖中...\n");
			sleep(3);

			int ward1 = 0;
			int ward2 = 0;
			int ward3 = 0;
			int ward4 = 0;
			for (int i = 0; i < winCnt; i++) {
				if (rewards[i] < 5) {
					ward1++;
				}
				else if (rewards[i] < 7) {
					ward2++;
				}
				else if (rewards[i] < 9) {
					ward3++;
				}
				else {
					ward4++;
				}
			}

			printf("恭喜, 共中将 %d 次, 获得: \n", winCnt);
			printf("随机基础装备 * %d\n随机成装 * %d\n随机光明装备 * %d\n",
				   ward1 + ward2 * 2, ward3, ward4);

			int isLottery = 0;
			printf("是否继续抽奖(1/0):> ");
			scanf("%d", &isLottery);
			clearInputBuffer();
			if (!isLottery) {
				printf("结束抽奖\n");
				break;
			}
		}
	}
}

// 福利
void welfare() {
	while (1) {
		printf("\n=========购物管理系统 > 主菜单 > 真情回馈=========\n");
		printf("|                  1. 积分兑好礼                   |\n");
		printf("|                  2. 积分大抽奖                   |\n");
		printf("|                   ~ 敬请期待 ~                   |\n");
		printf("|                  0. 返回上一级                   |\n");
		printf("====================================================\n");

		int select = -1;
		printf("请选择选项:> ");
		scanf("%d", &select);
		clearInputBuffer();

		switch (select) {
		case 1:
			integShop(); // 积分商城
			break;
		case 2:
			integLottery(); // 积分抽奖
			break;
		case 0:
			return;
		default:
			printf("选项非法, 请重新选择\n");
			break;
		}
	}
}
