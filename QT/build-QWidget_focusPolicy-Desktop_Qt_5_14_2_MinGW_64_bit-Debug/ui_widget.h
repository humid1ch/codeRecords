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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget {
public:
    QLineEdit* lineEdit_noFocus;
    QLineEdit* lineEdit_tabFocus_1;
    QLineEdit* lineEdit_tabFocus_2;
    QLineEdit* lineEdit_clickFocus;
    QLineEdit* lineEdit_strongFocus;

    void setupUi(QWidget* Widget) {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        lineEdit_noFocus = new QLineEdit(Widget);
        lineEdit_noFocus->setObjectName(QString::fromUtf8("lineEdit_noFocus"));
        lineEdit_noFocus->setGeometry(QRect(230, 120, 340, 40));

        lineEdit_tabFocus_1 = new QLineEdit(Widget);
        lineEdit_tabFocus_1->setObjectName(QString::fromUtf8("lineEdit_tabFocus_1"));
        lineEdit_tabFocus_1->setGeometry(QRect(230, 190, 340, 40));

        lineEdit_tabFocus_2 = new QLineEdit(Widget);
        lineEdit_tabFocus_2->setObjectName(QString::fromUtf8("lineEdit_tabFocus_2"));
        lineEdit_tabFocus_2->setGeometry(QRect(230, 260, 340, 40));

        lineEdit_clickFocus = new QLineEdit(Widget);
        lineEdit_clickFocus->setObjectName(QString::fromUtf8("lineEdit_clickFocus"));
        lineEdit_clickFocus->setGeometry(QRect(230, 330, 340, 40));

        lineEdit_strongFocus = new QLineEdit(Widget);
        lineEdit_strongFocus->setObjectName(QString::fromUtf8("lineEdit_strongFocus"));
        lineEdit_strongFocus->setGeometry(QRect(230, 400, 340, 40));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget* Widget) {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        lineEdit_noFocus->setInputMask(QString());
        lineEdit_noFocus->setPlaceholderText(
            QCoreApplication::translate("Widget", "\346\227\240\346\263\225\350\201\232\347\204\246", nullptr));
        lineEdit_tabFocus_1->setPlaceholderText(QCoreApplication::translate("Widget", "Tab\350\201\232\347\204\246", nullptr));
        lineEdit_tabFocus_2->setPlaceholderText(QCoreApplication::translate("Widget", "Tab\350\201\232\347\204\246", nullptr));
        lineEdit_clickFocus->setPlaceholderText(
            QCoreApplication::translate("Widget", "\347\202\271\345\207\273\350\201\232\347\204\246", nullptr));
        lineEdit_strongFocus->setPlaceholderText(
            QCoreApplication::translate("Widget", "Tab | \347\202\271\345\207\273\350\201\232\347\204\246", nullptr));
    } // retranslateUi
};

namespace Ui {
    class Widget : public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
