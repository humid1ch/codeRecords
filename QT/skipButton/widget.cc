#include "widget.h"
#include "ui_widget.h"

#include <ctime>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    srand(time(nullptr));
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_pushButton_pressed() {
    // 获取窗口的尺寸, 防止按钮跳跃出窗口
    int winWidth = this->geometry().width();
    int winHeight = this->geometry().height();

    int x = rand() % winWidth;
    int y = rand() % winHeight;

    x > (winWidth - ui->pushButton->geometry().width()) ? (x = winWidth - ui->pushButton->geometry().width()) : x;
    y > (winHeight - ui->pushButton->geometry().height()) ? (y = winHeight - ui->pushButton->geometry().height()) : y;

    ui->pushButton->setGeometry(x, y, ui->pushButton->geometry().width(), ui->pushButton->geometry().height());
}

void Widget::on_pushButton_clicke() {
    // 获取窗口的尺寸, 防止按钮跳跃出窗口
    int winWidth = this->geometry().width();
    int winHeight = this->geometry().height();

    int x = rand() % winWidth;
    int y = rand() % winHeight;

    x > (winWidth - ui->pushButton->geometry().width()) ? (x = winWidth - ui->pushButton->geometry().width()) : x;
    y > (winHeight - ui->pushButton->geometry().height()) ? (y = winHeight - ui->pushButton->geometry().height()) : y;

    ui->pushButton->setGeometry(x, y, ui->pushButton->geometry().width(), ui->pushButton->geometry().height());
}
