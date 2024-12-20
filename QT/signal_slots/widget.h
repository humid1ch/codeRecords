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

signals:
    //    void diySignal();
    void diySignal(int, const QString&);

private slots:
    void on_pushButton_clicked();
    void diySignalHandler1(int num, const QString&);
    void diySignalHandler2(int num);

private:
    Ui::Widget* ui;
};

#endif // WIDGET_H
