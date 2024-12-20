#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->pushButton->setToolTip("这只是一个什么用都没有的按钮");
    ui->pushButton->setToolTipDuration(5000);
}

Widget::~Widget() {
    delete ui;
}
