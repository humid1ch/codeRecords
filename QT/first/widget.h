/* 2024年的今天, 不建议定义宏来防止头文件重复包含 */
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
	class Widget;
}
QT_END_NAMESPACE
/*
QT_BEGIN_NAMESPACE -> namespace QT_NAMESAPCE {

QT_BEGIN_NAMESAPCE -> }
*/


// Widget 继承自 QWidget
// Widget这个类名, 是在创建项目时指定的, 可以自定义
class Widget : public QWidget {
	Q_OBJECT
	// 此宏 与 QT中 信号 和 槽有关, 使用时会展开

public:
	Widget(QWidget* parent = nullptr);
	// QWidget* parent;
	/*	QT中引入对象树的机制
		创建的QT对象, 可以挂在对象树上
		对象树不是一个二叉树, 而是多叉树
		也就是说, QWidget对象可以被指定为parent, 树的父节点
	*/

	~Widget();

private:
	Ui::Widget* ui;
	/*	不是本Widget类对象
		而是 Ui 命名空间内的Widget对象
		Ui::Widget 继承自 Ui_Widget
		而本Widget类 继承自 QWidget
	*/
};
