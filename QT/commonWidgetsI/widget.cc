#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>
#include <QDebug>
#include <QRect>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    btn1 = new QPushButton(this);
    btn1->setText("按钮1");
    btn1->move(200, 200);
    btn1->setEnabled(false);

    btn2 = new QPushButton(this);
    btn2->setText("按钮2");
    btn2->move(300, 300);
    btn2->setEnabled(true);

    QRect btn1Geom = btn1->geometry();
    QRect btn2Geom = btn2->geometry();
    qDebug("按钮1 坐标: (%d, %d), 尺寸: (%d, %d)", btn1Geom.x(), btn1Geom.y(), btn1Geom.width(), btn1Geom.height());
    qDebug("按钮2 坐标: (%d, %d), 尺寸: (%d, %d)", btn2Geom.x(), btn2Geom.y(), btn2Geom.width(), btn2Geom.height());

    connect(btn1, &QPushButton::clicked, this, &Widget::btn1ClickedHandler);
    connect(btn2, &QPushButton::clicked, this, &Widget::btn2ClickedHandler);
}

Widget::~Widget() {
    delete ui;
}

void Widget::btn1ClickedHandler() {
    qDebug() << "按钮1被点击, 槽函数执行";
}

void Widget::btn2ClickedHandler() {
    QRect btn1Geom = btn1->geometry();
    qDebug("按钮1 坐标: (%d, %d), 尺寸: (%d, %d)", btn1Geom.x(), btn1Geom.y(), btn1Geom.width(), btn1Geom.height());

    bool btn1Enabled = btn1->isEnabled();
    if (btn1Enabled) {
        // 可用
        btn1->setEnabled(false);
        btn1Geom.setX(200);
        btn1Geom.setY(200);
        btn1Geom.setWidth(100);
        btn1Geom.setHeight(30);

        btn1->setGeometry(btn1Geom);
        qDebug("重新设置按钮1 坐标: (%d, %d), 尺寸: (%d, %d)", btn1Geom.x(), btn1Geom.y(), btn1Geom.width(), btn1Geom.height());
    }
    else {
        // 不可用
        btn1->setEnabled(true);
        btn1Geom.setX(500);
        btn1Geom.setY(500);
        btn1Geom.setWidth(200);
        btn1Geom.setHeight(30);

        btn1->setGeometry(btn1Geom);
        qDebug("重新设置按钮1 坐标: (%d, %d), 尺寸: (%d, %d)", btn1Geom.x(), btn1Geom.y(), btn1Geom.width(), btn1Geom.height());
    }
}

//void Widget::btn1ClickedHandler() {
//    qDebug() << "按钮1被点击, 槽函数执行";
//}

//void Widget::btn2ClickedHandler() {
//    bool btn1Enabled = btn1->isEnabled();
//    if (btn1Enabled)
//        btn1->setEnabled(false);
//    else
//        btn1->setEnabled(true);
//}
