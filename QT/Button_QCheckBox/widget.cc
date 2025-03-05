#include "widget.h"
#include "ui_widget.h"

#include <QString>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_pushButton_clicked() {
    QString result = "小孩子才做选择, 而我选择:";

    if (ui->checkBox->isChecked()) {
        result += " " + ui->checkBox->text();
    }
    if (ui->checkBox_2->isChecked()) {
        result += " " + ui->checkBox_2->text();
    }
    if (ui->checkBox_3->isChecked()) {
        result += " " + ui->checkBox_3->text();
    }
    if (ui->checkBox_4->isChecked()) {
        result += " " + ui->checkBox_4->text();
    }

    ui->label->setText(result);
}
