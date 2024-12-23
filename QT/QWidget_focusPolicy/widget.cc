#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->lineEdit_noFocus->setFocusPolicy(Qt::NoFocus);
    qDebug() << ui->lineEdit_noFocus->focusPolicy();

    ui->lineEdit_tabFocus_1->setFocusPolicy(Qt::TabFocus);
    qDebug() << ui->lineEdit_tabFocus_1->focusPolicy();

    ui->lineEdit_tabFocus_2->setFocusPolicy(Qt::TabFocus);
    qDebug() << ui->lineEdit_tabFocus_2->focusPolicy();

    ui->lineEdit_clickFocus->setFocusPolicy(Qt::ClickFocus);
    qDebug() << ui->lineEdit_clickFocus->focusPolicy();

    ui->lineEdit_strongFocus->setFocusPolicy(Qt::StrongFocus);
    qDebug() << ui->lineEdit_strongFocus->focusPolicy();
}

Widget::~Widget() {
    delete ui;
}
