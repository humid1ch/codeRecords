#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , isNight(false) {
    ui->setupUi(this);

    ui->textEdit->setStyleSheet("font-family: 'FiraCode Nerd Font'; font-size: 16pt;");
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_pushButton_clicked() {
    if (isNight) {
        // 当前为夜间模式, 需要设置为日间模式
        this->setStyleSheet("background-color: rgb(240, 240, 240); color: black;");
        ui->textEdit->setStyleSheet("background-color: white; color: black; font-family: 'FiraCode Nerd "
                                    "Font'; font-size: 16pt;");
        ui->pushButton->setText("夜间模式");

        isNight = false;
    }
    else {
        // 当前为日间模式, 需要设置为夜间模式
        this->setStyleSheet("background-color: rgb(36, 39, 58); color: rgb(202, 211, 245);");
        ui->textEdit->setStyleSheet("background-color: rgb(36, 39, 58); color: rgb(202, 211, 245); font-family: 'FiraCode Nerd "
                                    "Font'; font-size: 16pt;");
        ui->pushButton->setText("日间模式");

        isNight = true;
    }
}
