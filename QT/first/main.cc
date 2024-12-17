#include "widget.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Widget w;
    w.show();
    //    w.hide(); // 隐藏控件
    //    w.show(); // 显示控件

    return a.exec();

    // Linux中 存在的6个进程替换系统调用, 与这里的 QApplication::exec() 没有任何关系
}
