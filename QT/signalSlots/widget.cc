#include "widget.h"
#include "ui_widget.h"

#include <QLabel>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    QLabel* label = new QLabel(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::aSlot() {
    ui->pushButton->setText("在按钮2被按下之后, 按钮1再被按下");
    this->setWindowTitle("按钮1的clicked信号连接的槽, 已成功被修改为aSlot()");
}

void Widget::on_pushButton_clicked() {
    // 按钮1的clicked信号的槽
    ui->pushButton->setText("按钮1已被按下");
    this->setWindowTitle("按钮1已被按下, 标题被修改");
}

void Widget::on_pushButton_2_clicked() {
    // 按钮2的clicked信号的槽
    ui->pushButton_2->setText("按钮2已被按下, 修改按钮1 clicked信号连接的槽");
    // 断开按钮1与原槽的连接
    disconnect(ui->pushButton, &QPushButton::clicked, this, &Widget::on_pushButton_clicked);
    // 按钮1与另一个槽建立连接
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::aSlot);
}
