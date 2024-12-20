#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    qDebug() << "包含 WindowFrame 的窗口信息: " << this->frameGeometry();
    qDebug() << this->geometry();
}

Widget::~Widget() {
    delete ui;
}
