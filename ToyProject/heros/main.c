#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 技能结构体
typedef struct Skill {
    char name[20];     // 技能名字
    double damageRate; // 伤害倍率
    int recovery;      // 恢复血量
} Skill;

// 英雄结构体
typedef struct Hero {
    char name[20];   // 英雄名
    int hp;          // 血量
    int attack;      // 攻击力
    int defense;     // 防御力
    int critical;    // 暴击, 发生暴击伤害增加 50%
    Skill skills[3]; // 技能表: [0]: 大招, [1]: 平A, [2]: 治疗
} Hero;

// 参数: Hero变量地址, 名字, 血量, 攻击, 防御, 暴击, 技能列表
void initHero(Hero* hero, const char* name, int hp, int attack, int defense, int critical, const Skill* skills) {
    strcpy(hero->name, name);
    hero->hp = hp;
    hero->attack = attack;
    hero->defense = defense;
    hero->critical = critical;

    // 技能赋予
    memcpy(hero->skills, skills, sizeof(Skill) * 3);
}

float critical(const Hero* hero) {
    int num = rand() % 100 + 1; // 生成 1~100 的随机数

    if (num <= hero->critical)
        return 1.5; // 暴击

    return 1.0; // 不暴击
}

void skillChoose(Skill* skill, const Hero* hero) {
    int num = rand() % 5 + 1; // 生成 1 ~ 5随机数

    // 大招 和 治疗的概率都是 20%
    if (num == 4)
        memcpy(skill, &hero->skills[0], sizeof(Skill));
    else if (num == 5)
        memcpy(skill, &hero->skills[2], sizeof(Skill));
    else
        memcpy(skill, &hero->skills[1], sizeof(Skill));
}

// 不允许改变英雄默认属性
void herosPK(const Hero* hero1, const Hero* hero2) {
    assert(hero1);
    assert(hero2);

    srand(time(NULL));

    printf("%s: 血: %d, 攻击: %d, 防御: %d, 暴击率: %d%%\n", hero1->name, hero1->hp, hero1->attack, hero1->defense, hero1->critical);
    printf("%s: 血: %d, 攻击: %d, 防御: %d, 暴击率: %d%%\n", hero2->name, hero2->hp, hero2->attack, hero2->defense, hero2->critical);

    printf("\n战斗开始! %s VS %s\n\n", hero1->name, hero2->name);

    int hp1 = hero1->hp; // 英雄1, 剩余血量
    int hp2 = hero2->hp; // 英雄2, 剩余血量

    float cri1 = 1.0; // 英雄1, 计算暴击伤害, 若暴击, 为1.5
    float cri2 = 1.0; // 英雄2, 计算暴击伤害

    int damage1 = 0; // 英雄1, 本次造成伤害
    int damage2 = 0; // 英雄2, 本次造成伤害

    int recovery1 = 0; // 英雄1, 本次恢复血量
    int recovery2 = 0; // 英雄2, 本次恢复血量

    Skill skill1; // 英雄1, 技能释放
    Skill skill2; // 英雄2, 技能释放

    int cnt = 0; // 回合计数
    while (hp1 && hp2) {
        printf("第 %d 回合\n", ++cnt);
        // 暴击判断
        cri1 = critical(hero1);
        cri2 = critical(hero2);

        // 本次释放技能
        skillChoose(&skill1, hero1);
        skillChoose(&skill2, hero2);

        // 本次造成伤害
        damage1 = hero1->attack * skill1.damageRate * cri1 - hero2->defense;
        damage1 < 0 ? (damage1 = 0) : damage1;

        damage2 = hero2->attack * skill2.damageRate * cri2 - hero1->defense;
        damage2 < 0 ? (damage2 = 0) : damage2;

        // 本次恢复血量, 不吃暴击
        recovery1 = skill1.recovery;
        recovery2 = skill2.recovery;

        // 剩余血量计算
        // hero1 先手, 如果吃血包, 就先回血, 如果攻击, 对面就先吃伤害
        hp1 += recovery1; // 回血
        hp2 -= damage1;   // 对面吃伤害
        // 吃血包也不能超出血量上限
        hp1 > hero1->hp ? (hp1 = hero1->hp) : hp1;
        // 剩余血量 不能是负数啊
        hp2 < 0 ? (hp2 = 0) : hp2;

        // hero1 回合结算
        printf("%s 使用 '%s' ", hero1->name, skill1.name);
        if (cri1 - 1.0 > 0.0000001) {
            printf(" 暴击了!!!\n");
        }
        printf("对 %s 造成 %d 伤害, 自身恢复 %d 血量\n", hero2->name, damage1, recovery1);
        printf("%s 剩余血量: %d, %s 剩余血量: %d\n\n", hero1->name, hp1, hero2->name, hp2);

        // hero2 后手, 可能已经去世
        // 若没去世, 才能放技能
        if (hp2) {
            hp1 -= damage2;
            hp2 += recovery2;
            hp2 > hero2->hp ? (hp2 = hero2->hp) : hp2;
            hp1 < 0 ? (hp1 = 0) : hp1;

            // hero2 回合结算
            printf("%s 使用 '%s' ", hero2->name, skill2.name);
            if (cri2 - 1.0 > 0.0000001) {
                printf(" 暴击了!!!\n");
            }
            printf("对 %s 造成 %d 伤害, 自身恢复 %d 血量\n", hero1->name, damage2, recovery2);
            printf("%s 剩余血量: %d, %s 剩余血量: %d\n\n", hero1->name, hp1, hero2->name, hp2);
        }
    }

    if (hp1) {
        printf("%s 受到致命伤害, 倒下了\n", hero2->name);
        printf("%s 获胜!\n", hero1->name);
    }

    if (hp2) {
        printf("%s 受到致命伤害, 倒下了\n", hero1->name);
        printf("%s 获胜!\n", hero2->name);
    }
}

int main() {
    Hero hanzo; // 半藏
    Hero genji; // 源氏

    // 技能列表: 大招  平A  治疗
    Skill hanzoSkills[3] = {{"竜", 1.5, 0}, {"风", 1, 0}, {"血包", 0, 50}};
    Skill genjiSkills[3] = {{"斩", 1.5, 0}, {"镖", 1, 0}, {"血包", 0, 50}};

    initHero(&hanzo, "岛田半藏", 200, 40, 20, 5, hanzoSkills);
    initHero(&genji, "岛田源氏", 200, 40, 20, 5, genjiSkills);

    herosPK(&hanzo, &genji);

    return 0;
}
