#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct Hero {
    char name[20];
    int hp;
    int attack;
    int defense;
} Hero;

void initHero(Hero* hero, const char* name, int hp, int attack, int defense) {
    strcpy(hero->name, name);
    hero->hp = hp;
    hero->attack = attack;
    hero->defense = defense;
}

void herosPK(const Hero* hero1, const Hero* hero2) {
    printf("战斗开始! %s VS %s\n", hero1->name, hero2->name);

    printf("%s: 血: %d, 攻击: %d, 防御: %d\n", hero1->name, hero1->hp, hero1->attack, hero1->defense);
    printf("%s: 血: %d, 攻击: %d, 防御: %d\n", hero2->name, hero2->hp, hero2->attack, hero2->defense);

    int damage1 =
        hero1->attack > hero2->defense ? hero1->attack - hero2->defense : 0;
    int damage2 =
        hero2->attack > hero1->defense ? hero2->attack - hero1->defense : 0;
    int hp1 = hero1->hp;
    int hp2 = hero2->hp;

    if (damage1 == 0 && damage1 == damage2) {
        printf("%s 打不动 %s, %s 也打不动 %s, 平局\n", hero1->name, hero2->name,
               hero2->name, hero1->name);
        return;
    }

    while (hp1 && hp2) {
        printf("%s 对 %s 造成了 %d 点伤害，%s 的生命值剩余 %d\n", hero1->name,
               hero2->name, damage1, hero2->name,
               hp2 - damage1 > 0 ? hp2 -= damage1 : (hp2 = 0));
        printf("%s 对 %s 造成了 %d 点伤害，%s 的生命值剩余 %d\n", hero2->name,
               hero1->name, damage2, hero1->name,
               hp1 - damage2 > 0 ? hp1 -= damage2 : (hp1 = 0));
        sleep(1);
        printf("\n");
    }

    if (hp1) {
        printf("%s 获胜!\n", hero1->name);
    }

    if (hp2) {
        printf("%s 获胜!\n", hero2->name);
    }
}

int main() {
    Hero dc;
    Hero zy;
    initHero(&dc, "貂蝉", 80, 30, 20);
    initHero(&zy, "赵云", 100, 30, 30);

    herosPK(&dc, &zy);

    return 0;
}
