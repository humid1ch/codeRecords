#include "widget.h"
#include "ui_widget.h"

#include <QIcon>
#include <QShortcut>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

    ui->pushBtn_up->setIcon(QIcon(":/up.png"));
    ui->pushBtn_up->setIconSize(QSize(32, 32));
    ui->pushBtn_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));

    ui->pushBtn_down->setIcon(QIcon(":/down.png"));
    ui->pushBtn_down->setIconSize(QSize(32, 32));
    ui->pushBtn_down->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    ui->pushBtn_left->setIcon(QIcon(":/left.png"));
    ui->pushBtn_left->setIconSize(QSize(32, 32));
    ui->pushBtn_left->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));

    ui->pushBtn_right->setIcon(QIcon(":/right.png"));
    ui->pushBtn_right->setIconSize(QSize(32, 32));
    ui->pushBtn_right->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    ui->pushBtn_right->setDefault(true);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_pushBtn_up_clicked() {
    QRect nowGeo = ui->pushBtn_ICON->geometry();
    if (nowGeo.y() < 5)
        return;

    ui->pushBtn_ICON->setGeometry(nowGeo.x(), nowGeo.y() - 5, nowGeo.width(), nowGeo.height());
}

void Widget::on_pushBtn_down_clicked() {
    QRect nowGeo = ui->pushBtn_ICON->geometry();
    if (nowGeo.y() > ui->pushBtn_up->geometry().y() - nowGeo.height() - 1)
        return;

    ui->pushBtn_ICON->setGeometry(nowGeo.x(), nowGeo.y() + 5, nowGeo.width(), nowGeo.height());
}

void Widget::on_pushBtn_left_clicked() {
    QRect nowGeo = ui->pushBtn_ICON->geometry();
    if (nowGeo.x() < 5)
        return;

    ui->pushBtn_ICON->setGeometry(nowGeo.x() - 5, nowGeo.y(), nowGeo.width(), nowGeo.height());
}

void Widget::on_pushBtn_right_clicked() {
    QRect nowGeo = ui->pushBtn_ICON->geometry();
    if (nowGeo.x() > this->geometry().width() - nowGeo.width() - 1)
        return;

    ui->pushBtn_ICON->setGeometry(nowGeo.x() + 5, nowGeo.y(), nowGeo.width(), nowGeo.height());
}
