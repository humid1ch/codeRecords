#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    connect(this, &Widget::diySignal, this, &Widget::diySignalHandler1);
    connect(this, &Widget::diySignal, this, &Widget::diySignalHandler2);
    disconnect();
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_pushButton_clicked() {
    ui->pushButton->setText("按钮已被点击, 槽发送diySignal");
    // 按钮点击信号的槽函数, 发送自定义信号
    // diySignal();
    emit diySignal(20, "diySignal携带一个标题参数");
}

void Widget::diySignalHandler1(int num, const QString& title) {
    //    this->setWindowTitle("diySignal被接收到, 并执行槽函数");
    this->setWindowTitle(title);
    qDebug("Handler1 receive type(int) data: %d\n", num);
}

void Widget::diySignalHandler2(int num) {
    qDebug("Handler2 receive type(int) data: %d\n", num);
}
