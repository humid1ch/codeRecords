/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushBtn_left;
    QPushButton *pushBtn_right;
    QPushButton *pushBtn_down;
    QPushButton *pushBtn_up;
    QPushButton *pushBtn_ICON;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        pushBtn_left = new QPushButton(Widget);
        pushBtn_left->setObjectName(QString::fromUtf8("pushBtn_left"));
        pushBtn_left->setGeometry(QRect(268, 444, 88, 88));
        pushBtn_right = new QPushButton(Widget);
        pushBtn_right->setObjectName(QString::fromUtf8("pushBtn_right"));
        pushBtn_right->setGeometry(QRect(444, 444, 88, 88));
        pushBtn_down = new QPushButton(Widget);
        pushBtn_down->setObjectName(QString::fromUtf8("pushBtn_down"));
        pushBtn_down->setGeometry(QRect(356, 488, 88, 88));
        pushBtn_up = new QPushButton(Widget);
        pushBtn_up->setObjectName(QString::fromUtf8("pushBtn_up"));
        pushBtn_up->setGeometry(QRect(356, 400, 88, 88));
        pushBtn_up->setFlat(false);
        pushBtn_ICON = new QPushButton(Widget);
        pushBtn_ICON->setObjectName(QString::fromUtf8("pushBtn_ICON"));
        pushBtn_ICON->setGeometry(QRect(336, 100, 128, 128));

        retranslateUi(Widget);

        pushBtn_up->setDefault(false);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pushBtn_left->setText(QString());
        pushBtn_right->setText(QString());
        pushBtn_down->setText(QString());
        pushBtn_up->setText(QString());
        pushBtn_ICON->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
