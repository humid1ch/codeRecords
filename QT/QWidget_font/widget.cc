#include "widget.h"
#include "ui_widget.h"

#include <QFont>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    QFont fonts;
    fonts.setFamily("FiraCode Nerd Font"); // 设置 字体族
    fonts.setPointSize(14);				   // 设置 字体大小14
    fonts.setBold(true);				   // 开启 粗体
    fonts.setItalic(false);				   // 关闭 斜体
    fonts.setUnderline(false);			   // 关闭 下划线
    fonts.setStrikeOut(true);			   // 开启 删除线
    fonts.setKerning(true);				   // 关闭 字距调整

    ui->label->setFont(fonts);
    ui->label->setText("This is a QLabel.");
}

Widget::~Widget() {
    delete ui;
}
