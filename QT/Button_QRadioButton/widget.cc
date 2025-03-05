#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->radioButton_male->setText("男");
    ui->radioButton_female->setText("女");
    ui->radioButton_unknow->setText("未知");
    ui->label->setText("请选择你的性别:");

    ui->radioButton_unknow->setAutoExclusive(false);
    ui->radioButton_male->setChecked(true);
}

Widget::~Widget() {
    delete ui;
}

//void Widget::on_radioButton_male_clicked(bool checked) {
//    qDebug() << "radioButton_male checked: " << checked;
//}

//void Widget::on_radioButton_male_clicked() {
//    qDebug() << "radioButton_male clicked";
//}

//void Widget::on_radioButton_unknow_clicked(bool checked) {
//    qDebug() << "radioButton_unknow checked: " << checked;
//}

//void Widget::on_radioButton_male_pressed() {
//    qDebug() << "radioButton_male pressed";
//}

//void Widget::on_radioButton_male_released() {
//    qDebug() << "radioButton_male released";
//}

void Widget::on_radioButton_unknow_toggled(bool checked) {
    qDebug() << "radioButton_unknow checked changed, new checked: " << checked;
}

void Widget::on_radioButton_female_toggled(bool checked) {
    qDebug() << "radioButton_female checked changed, new checked: " << checked;
}
