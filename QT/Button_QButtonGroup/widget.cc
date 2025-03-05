#include "widget.h"
#include "ui_widget.h"

#include <QButtonGroup>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    QButtonGroup* staplesGroup = new QButtonGroup(this);
    QButtonGroup* snacksGroup = new QButtonGroup(this);
    QButtonGroup* sweetyGroup = new QButtonGroup(this);
    QButtonGroup* drinkGroup = new QButtonGroup(this);

    staplesGroup->addButton(ui->radioBtn_staples_spa, 1);
    staplesGroup->addButton(ui->radioBtn_staples_yes, 2);
    ui->radioBtn_staples_spa->setChecked(true);

    snacksGroup->addButton(ui->radioBtn_snacks_fries, 1);
    snacksGroup->addButton(ui->radioBtn_snacks_chickenRice, 2);
    ui->radioBtn_snacks_fries->setChecked(true);

    sweetyGroup->addButton(ui->radioBtn_sweety_redBeanPie, 1);
    sweetyGroup->addButton(ui->radioBtn_sweety_mashedPotato, 2);
    sweetyGroup->addButton(ui->radioBtn_sweety_sundae, 3);
    sweetyGroup->addButton(ui->radioBtn_sweety_sundaeStrawberry, 4);
    ui->radioBtn_sweety_redBeanPie->setChecked(true);

    drinkGroup->addButton(ui->radioBtn_drink_coke, 1);
    drinkGroup->addButton(ui->radioBtn_drink_juice, 1);
    drinkGroup->addButton(ui->radioBtn_drink_lemonTea, 1);
    ui->radioBtn_drink_coke->setChecked(true);
}

Widget::~Widget() {
    delete ui;
}
