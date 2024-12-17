#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    QString title = this->windowTitle();
    qDebug() << title;

    this->setWindowTitle("这是一个窗口标题");
    title = this->windowTitle();
    qDebug() << title;

    QIcon winIcon(":/smile.png");
    this->setWindowIcon(winIcon);

    btn1 = new QPushButton(this);
    btn1->setGeometry(300, 210, 200, 40);
    btn1->setText("按钮1: +");
    connect(btn1, &QPushButton::clicked, this, &Widget::btn1ClickedHandler);

    btn2 = new QPushButton(this);
    btn2->setGeometry(300, 330, 200, 40);
    btn2->setText("按钮2: -");
    connect(btn2, &QPushButton::clicked, this, &Widget::btn2ClickedHandler);
}

Widget::~Widget() {
    delete ui;
}

void Widget::btn1ClickedHandler() {
    qreal winOpacity = this->windowOpacity();
    if (winOpacity > 1) {
        return;
    }

    this->setWindowOpacity(winOpacity + 0.05);

    qDebug() << this->windowOpacity();
}

void Widget::btn2ClickedHandler() {
    qreal winOpacity = this->windowOpacity();
    if (winOpacity < 0) {
        return;
    }

    this->setWindowOpacity(winOpacity - 0.05);

    qDebug() << this->windowOpacity();
}
