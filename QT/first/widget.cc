#include "widget.h"

#include "ui_widget.h"
/*	qmake 根据Form file生成的头文件, 里边有一个Ui namespace
    此头文件中, 定义了一个Widget类, 继承自Ui_Widget
    Ui::Widget类中, 重写了一些Ui_Widget类的接口, 可以让Form file的内容设置在 Ui::Widget中
    并且, 调用接口 可以将Ui::Widget与自定义的继承自QWidget的类关联起来

    说简单点, 就是 qmake根据Formfile(UI文件), 定义了一个Ui_Widget的子类Ui::Widget
    Ui::Widget里重写了一些父类函数, 可以将Formfile里的UI设计、配置设置在Ui:Widget中
    并且, Ui:Widget的函数, 还能够将自定义的类和Ui::Widget关联起来, 进而让自定义的类与Formfile的UI关联起来
*/

Widget::Widget(QWidget* parent)
	: QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this); // Widget的构造函数, 实现将 Widget对象与qmake根据Form file生成的Ui:Widget 关联起来
}

Widget::~Widget() {
	delete ui;
}
