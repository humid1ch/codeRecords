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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QCheckBox *checkBox;
    QLabel *label;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QPushButton *pushButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        checkBox = new QCheckBox(Widget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(70, 200, 151, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("HarmonyOS Sans SC"));
        font.setPointSize(16);
        checkBox->setFont(font);
        checkBox->setTristate(false);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 110, 721, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("HarmonyOS Sans SC"));
        font1.setPointSize(19);
        label->setFont(font1);
        checkBox_2 = new QCheckBox(Widget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(250, 200, 151, 31));
        checkBox_2->setFont(font);
        checkBox_2->setTristate(false);
        checkBox_3 = new QCheckBox(Widget);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(430, 200, 151, 31));
        checkBox_3->setFont(font);
        checkBox_3->setTristate(false);
        checkBox_4 = new QCheckBox(Widget);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setGeometry(QRect(610, 200, 151, 31));
        checkBox_4->setFont(font);
        checkBox_4->setTristate(false);
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(350, 280, 101, 51));
        pushButton->setFont(font);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        checkBox->setText(QCoreApplication::translate("Widget", "\346\260\270\350\221\206\351\235\222\346\230\245", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\345\260\217\345\255\251\345\255\220\346\211\215\345\201\232\351\200\211\346\213\251, \350\200\214\346\210\221\351\200\211\346\213\251:", nullptr));
        checkBox_2->setText(QCoreApplication::translate("Widget", "\345\255\246\345\257\214\344\272\224\350\275\246", nullptr));
        checkBox_3->setText(QCoreApplication::translate("Widget", "\346\210\220\344\270\272\345\244\247\344\275\254", nullptr));
        checkBox_4->setText(QCoreApplication::translate("Widget", "\344\270\200\345\244\234\346\232\264\345\257\214", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
