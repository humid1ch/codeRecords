#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanInputBuf() {
    char ch;
    while ((ch = getchar()) != '\n')
        ;
}

typedef struct Hero {
    char name[20]; // 英雄名
    int hp;        // 血量
    int attack;    // 攻击力
    int defense;   // 防御力
    int wins;      // 胜场
    int losses;    // 败厂
    float winRate; // 胜率
} Hero;

typedef Hero dataType;

typedef struct ListNode {
    dataType data;
    struct ListNode* next;
    struct ListNode* prev;
} List, Node;

Node* buyNode(const dataType* hero) {
    Node* node = (Node*)malloc(sizeof(Node));

    memcpy(&node->data, hero, sizeof(dataType));

    node->next = node;
    node->prev = node;

    return node;
}

List* createList() {
    dataType none = {};
    Node* head = buyNode(&none);

    return head;
}

void listPushBack(List* head, Node* node) {
    assert(head);
    assert(node);

    Node* backNode = head->prev;

    // 新节点连接
    node->prev = backNode;
    node->next = head;

    backNode->next = node;
    head->prev = node;
}

void listErase(List* head, Node* curNode) {
    assert(head);
    assert(curNode);

    if (curNode == head) {
        printf("哨兵头节点不能删除\n");
        return;
    }

    Node* prev = curNode->prev;
    Node* next = curNode->next;

    prev->next = next;
    next->prev = prev;

    free(curNode);
    curNode = NULL;
}

Node* destroyList(List* head) {
    assert(head);

    Node* cur = head->next;
    Node* next = cur->next;
    while (cur != head) {
        free(cur);
        cur = next;
        next = cur->next;
    }

    free(cur);
    cur = NULL;

    return cur;
}

// 参数: Hero变量地址, 名字, 血量, 攻击, 防御
void initHero(Hero* hero, const char* name, int hp, int attack, int defense) {
    strcpy(hero->name, name);
    hero->hp = hp;
    hero->attack = attack;
    hero->defense = defense;
    hero->wins = 0;
    hero->losses = 0;
    hero->winRate = 0.0;
}

void listAddHero(List* head, Hero* hero) {
    Node* newNode = buyNode(hero);
    listPushBack(head, newNode);
}

Node* findHeroByName(List* head, const char* name) {
    assert(head);
    assert(name);

    Node* cur = head->next;
    while (cur != head) {
        // 找到了
        if (strcmp(cur->data.name, name) == 0) {
            return cur;
        }

        cur = cur->next;
    }

    // 没找到
    return NULL;
}

void listDelHero(List* head, const char* name) {
    assert(head);
    assert(name);

    Node* cur = findHeroByName(head, name);
    if (cur == NULL) {
        printf("查找失败, 没有该英雄\n");
        return;
    }

    listErase(head, cur);
}

void addHero(List* head) {
    assert(head);

    Hero hero;
    char name[20] = {};
    int hp = 0;
    int attack = 0;
    int defense = 0;
    // 请输入英雄名称：赵云
    // 请输入生命值：100
    // 请输入攻击力：30
    // 请输入防御力：10
    // 英雄 赵云 添加成功！

    printf("请输入英雄名称: ");
    scanf("%s", name);
    cleanInputBuf();

    printf("请输入生命值: ");
    scanf("%d", &hp);
    cleanInputBuf();

    printf("请输入攻击力: ");
    scanf("%d", &attack);
    cleanInputBuf();

    printf("请输入防御力: ");
    scanf("%d", &defense);
    cleanInputBuf();

    if (findHeroByName(head, name) != NULL) {
        printf("该英雄已存在\n");
        return;
    }
    else {
        initHero(&hero, name, hp, attack, defense);

        listAddHero(head, &hero);

        printf("英雄 %s 添加成功!\n", name);
    }
}

void delHero(List* head) {
    assert(head);

    char name[20] = {};
    printf("请输入要删除的英雄名称: ");
    scanf("%s", name);
    cleanInputBuf();

    listDelHero(head, name);
}

void displayHeros(const List* head) {
    assert(head);

    Node* cur = head->next;
    printf("英雄列表:\n");
    printf(" 名称            生命值\t攻击力\t防御力\t胜场数\t败场数\t胜率\n");
    while (cur != head) {
        printf(" %-20s%d\t%d\t%d\t%d\t%d\t%.2f%%\n", cur->data.name, cur->data.hp, cur->data.attack, cur->data.defense, cur->data.wins, cur->data.losses, cur->data.winRate * 100);
        cur = cur->next;
    }
}

void calculateWinRate(Hero* hero) {
    assert(hero);

    int sum = hero->wins + hero->losses;
    hero->winRate = (float)hero->wins / (float)sum;
}

int menu(List* head) {
    printf("守望先锋 英雄管理系统\n");
    printf("1. 添加英雄\n");
    printf("2. 删除英雄\n");
    printf("3. 显示英雄列表\n");
    printf("0. 退出\n");

    int num = -1;
    printf("请输入选项:> ");
    scanf("%d", &num);
    cleanInputBuf();

    switch (num) {
    case 1:
        addHero(head);
        break;
    case 2:
        delHero(head);
        break;
    case 3:
        displayHeros(head);
        break;
    case 0:
        return 0;
    default:
        printf("输入选项非法, 请重新输入\n");
        break;
    }

    return 1;
}

int main() {
    List* list = createList();
    while (menu(list))
        ;

    destroyList(list);

    return 0;
}
