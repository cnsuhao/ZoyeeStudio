/********************************************************************************
** Form generated from reading UI file 'linewgt.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEWGT_H
#define UI_LINEWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LineWgt
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;

    void setupUi(QWidget *LineWgt)
    {
        if (LineWgt->objectName().isEmpty())
            LineWgt->setObjectName(QStringLiteral("LineWgt"));
        LineWgt->resize(414, 100);
        LineWgt->setMinimumSize(QSize(0, 0));
        LineWgt->setMaximumSize(QSize(16777215, 100));
        LineWgt->setStyleSheet(QLatin1String("QGroupBox{\n"
"	border: 1px solid rgb(180,180,255);\n"
"}\n"
"QLineEdit{\n"
"	border: 1px solid rgb(180,180,255);\n"
"	border-radius: 4px;\n"
"}\n"
"QLineEdit:hover{\n"
"	border: 1px solid rgb(124,124,255);\n"
"}\n"
"QPushButton{\n"
"	border: 1px solid rgb(180,180,255);\n"
"}\n"
"QPushButton:hover{\n"
"	border: 1px solid rgb(124,124,255);\n"
"}"));
        horizontalLayout = new QHBoxLayout(LineWgt);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(LineWgt);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        groupBox->setChecked(false);

        horizontalLayout->addWidget(groupBox);


        retranslateUi(LineWgt);

        QMetaObject::connectSlotsByName(LineWgt);
    } // setupUi

    void retranslateUi(QWidget *LineWgt)
    {
        LineWgt->setWindowTitle(QApplication::translate("LineWgt", "LineWgt", 0));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class LineWgt: public Ui_LineWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEWGT_H
