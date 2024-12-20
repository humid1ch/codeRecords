#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private slots:
    void btn1ClickedHandler();
    void btn2ClickedHandler();

private:
    Ui::Widget* ui;
    QPushButton* btn1;
    QPushButton* btn2;
};
#endif // WIDGET_H
