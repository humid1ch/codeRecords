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
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label_OK;
    QLabel *label_staples;
    QLabel *label_snacks;
    QLabel *label_sweety;
    QLabel *label_drink;
    QRadioButton *radioBtn_staples_spa;
    QRadioButton *radioBtn_staples_yes;
    QRadioButton *radioBtn_snacks_fries;
    QRadioButton *radioBtn_snacks_chickenRice;
    QRadioButton *radioBtn_sweety_redBeanPie;
    QRadioButton *radioBtn_sweety_mashedPotato;
    QRadioButton *radioBtn_sweety_sundae;
    QRadioButton *radioBtn_sweety_sundaeStrawberry;
    QRadioButton *radioBtn_drink_coke;
    QRadioButton *radioBtn_drink_juice;
    QRadioButton *radioBtn_drink_lemonTea;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        Widget->setTabletTracking(false);
        label_OK = new QLabel(Widget);
        label_OK->setObjectName(QString::fromUtf8("label_OK"));
        label_OK->setGeometry(QRect(305, 100, 191, 61));
        QFont font;
        font.setFamily(QString::fromUtf8("HarmonyOS Sans SC"));
        font.setPointSize(21);
        label_OK->setFont(font);
        label_staples = new QLabel(Widget);
        label_staples->setObjectName(QString::fromUtf8("label_staples"));
        label_staples->setGeometry(QRect(80, 220, 41, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("HarmonyOS Sans SC"));
        font1.setPointSize(14);
        label_staples->setFont(font1);
        label_snacks = new QLabel(Widget);
        label_snacks->setObjectName(QString::fromUtf8("label_snacks"));
        label_snacks->setGeometry(QRect(260, 220, 41, 21));
        label_snacks->setFont(font1);
        label_sweety = new QLabel(Widget);
        label_sweety->setObjectName(QString::fromUtf8("label_sweety"));
        label_sweety->setGeometry(QRect(440, 220, 91, 21));
        label_sweety->setFont(font1);
        label_drink = new QLabel(Widget);
        label_drink->setObjectName(QString::fromUtf8("label_drink"));
        label_drink->setGeometry(QRect(620, 220, 41, 21));
        label_drink->setFont(font1);
        radioBtn_staples_spa = new QRadioButton(Widget);
        radioBtn_staples_spa->setObjectName(QString::fromUtf8("radioBtn_staples_spa"));
        radioBtn_staples_spa->setGeometry(QRect(70, 260, 131, 21));
        QFont font2;
        font2.setFamily(QString::fromUtf8("HarmonyOS Sans SC"));
        font2.setPointSize(12);
        radioBtn_staples_spa->setFont(font2);
        radioBtn_staples_yes = new QRadioButton(Widget);
        radioBtn_staples_yes->setObjectName(QString::fromUtf8("radioBtn_staples_yes"));
        radioBtn_staples_yes->setGeometry(QRect(70, 300, 151, 21));
        radioBtn_staples_yes->setFont(font2);
        radioBtn_snacks_fries = new QRadioButton(Widget);
        radioBtn_snacks_fries->setObjectName(QString::fromUtf8("radioBtn_snacks_fries"));
        radioBtn_snacks_fries->setGeometry(QRect(250, 260, 131, 21));
        radioBtn_snacks_fries->setFont(font2);
        radioBtn_snacks_chickenRice = new QRadioButton(Widget);
        radioBtn_snacks_chickenRice->setObjectName(QString::fromUtf8("radioBtn_snacks_chickenRice"));
        radioBtn_snacks_chickenRice->setGeometry(QRect(250, 300, 131, 21));
        radioBtn_snacks_chickenRice->setFont(font2);
        radioBtn_sweety_redBeanPie = new QRadioButton(Widget);
        radioBtn_sweety_redBeanPie->setObjectName(QString::fromUtf8("radioBtn_sweety_redBeanPie"));
        radioBtn_sweety_redBeanPie->setGeometry(QRect(430, 260, 131, 21));
        radioBtn_sweety_redBeanPie->setFont(font2);
        radioBtn_sweety_mashedPotato = new QRadioButton(Widget);
        radioBtn_sweety_mashedPotato->setObjectName(QString::fromUtf8("radioBtn_sweety_mashedPotato"));
        radioBtn_sweety_mashedPotato->setGeometry(QRect(430, 300, 131, 21));
        radioBtn_sweety_mashedPotato->setFont(font2);
        radioBtn_sweety_sundae = new QRadioButton(Widget);
        radioBtn_sweety_sundae->setObjectName(QString::fromUtf8("radioBtn_sweety_sundae"));
        radioBtn_sweety_sundae->setGeometry(QRect(430, 340, 131, 21));
        radioBtn_sweety_sundae->setFont(font2);
        radioBtn_sweety_sundaeStrawberry = new QRadioButton(Widget);
        radioBtn_sweety_sundaeStrawberry->setObjectName(QString::fromUtf8("radioBtn_sweety_sundaeStrawberry"));
        radioBtn_sweety_sundaeStrawberry->setGeometry(QRect(430, 380, 131, 21));
        radioBtn_sweety_sundaeStrawberry->setFont(font2);
        radioBtn_drink_coke = new QRadioButton(Widget);
        radioBtn_drink_coke->setObjectName(QString::fromUtf8("radioBtn_drink_coke"));
        radioBtn_drink_coke->setGeometry(QRect(610, 260, 131, 21));
        radioBtn_drink_coke->setFont(font2);
        radioBtn_drink_juice = new QRadioButton(Widget);
        radioBtn_drink_juice->setObjectName(QString::fromUtf8("radioBtn_drink_juice"));
        radioBtn_drink_juice->setGeometry(QRect(610, 300, 131, 21));
        radioBtn_drink_juice->setFont(font2);
        radioBtn_drink_lemonTea = new QRadioButton(Widget);
        radioBtn_drink_lemonTea->setObjectName(QString::fromUtf8("radioBtn_drink_lemonTea"));
        radioBtn_drink_lemonTea->setGeometry(QRect(610, 340, 131, 21));
        radioBtn_drink_lemonTea->setFont(font2);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_OK->setText(QCoreApplication::translate("Widget", "OK\351\244\220(\344\273\273\351\200\2114\344\273\266)", nullptr));
        label_staples->setText(QCoreApplication::translate("Widget", "\344\270\273\351\243\237", nullptr));
        label_snacks->setText(QCoreApplication::translate("Widget", "\345\260\217\351\243\237", nullptr));
        label_sweety->setText(QCoreApplication::translate("Widget", "\347\224\234\347\202\271/\351\205\215\351\244\220", nullptr));
        label_drink->setText(QCoreApplication::translate("Widget", "\351\245\256\346\226\231", nullptr));
        radioBtn_staples_spa->setText(QCoreApplication::translate("Widget", "\350\227\244\346\244\222\351\243\216\345\221\263SPA\345\240\241", nullptr));
        radioBtn_staples_yes->setText(QCoreApplication::translate("Widget", "\346\273\213\346\273\213YES\347\203\244\351\270\241\350\205\277\345\240\241", nullptr));
        radioBtn_snacks_fries->setText(QCoreApplication::translate("Widget", "\350\226\257\346\235\241(\344\270\255)", nullptr));
        radioBtn_snacks_chickenRice->setText(QCoreApplication::translate("Widget", "\345\212\262\347\210\206\351\270\241\347\261\263\350\212\261(\345\260\217)", nullptr));
        radioBtn_sweety_redBeanPie->setText(QCoreApplication::translate("Widget", "\347\272\242\350\261\206\346\264\276(1\345\217\252)", nullptr));
        radioBtn_sweety_mashedPotato->setText(QCoreApplication::translate("Widget", "\351\206\207\351\246\231\345\234\237\350\261\206\346\263\245", nullptr));
        radioBtn_sweety_sundae->setText(QCoreApplication::translate("Widget", "\345\216\237\345\221\263\345\234\243\344\273\243", nullptr));
        radioBtn_sweety_sundaeStrawberry->setText(QCoreApplication::translate("Widget", "\347\273\217\345\205\270\350\215\211\350\216\223\345\234\243\344\273\243", nullptr));
        radioBtn_drink_coke->setText(QCoreApplication::translate("Widget", "\345\217\257\344\271\220(\345\206\267/\344\270\255)", nullptr));
        radioBtn_drink_juice->setText(QCoreApplication::translate("Widget", "\346\236\234\346\261\201(\345\206\267)", nullptr));
        radioBtn_drink_lemonTea->setText(QCoreApplication::translate("Widget", "\346\237\240\346\252\254\350\214\266(\345\206\267/\344\270\255)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
