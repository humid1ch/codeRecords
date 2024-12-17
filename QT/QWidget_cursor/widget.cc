#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>
#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    //    QCursor csr(Qt::CrossCursor);
    QPixmap csrPic(":/heartWings.png");
    QCursor csr(csrPic, 128, 128);
    this->setCursor(csr);
    ui->pushButton->setCursor(csr);

    qDebug() << csr.hotSpot();
}

Widget::~Widget() {
    delete ui;
}
