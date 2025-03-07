#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    //    void on_radioButton_male_clicked(bool checked);
    //    void on_radioButton_unknow_clicked(bool checked);
    //    void on_radioButton_male_clicked();
    //    void on_radioButton_male_pressed();
    //    void on_radioButton_male_released();

    void on_radioButton_unknow_toggled(bool checked);

    void on_radioButton_female_toggled(bool checked);

private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
