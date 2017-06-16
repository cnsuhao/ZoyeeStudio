/********************************************************************************
** Form generated from reading UI file 'regvisualeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGVISUALEDITOR_H
#define UI_REGVISUALEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegVisualEditorClass
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *m_combo;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_btnReset;
    QPushButton *m_btnAdd;
    QScrollArea *scrollArea;
    QWidget *ContentsLayer;

    void setupUi(QWidget *RegVisualEditorClass)
    {
        if (RegVisualEditorClass->objectName().isEmpty())
            RegVisualEditorClass->setObjectName(QStringLiteral("RegVisualEditorClass"));
        RegVisualEditorClass->resize(600, 400);
        gridLayout = new QGridLayout(RegVisualEditorClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(RegVisualEditorClass);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(RegVisualEditorClass);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        widget = new QWidget(RegVisualEditorClass);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 50));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_combo = new QComboBox(widget);
        m_combo->setObjectName(QStringLiteral("m_combo"));
        m_combo->setMinimumSize(QSize(180, 0));

        horizontalLayout_2->addWidget(m_combo);

        horizontalSpacer = new QSpacerItem(30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        m_btnReset = new QPushButton(widget);
        m_btnReset->setObjectName(QStringLiteral("m_btnReset"));

        horizontalLayout_2->addWidget(m_btnReset);

        m_btnAdd = new QPushButton(widget);
        m_btnAdd->setObjectName(QStringLiteral("m_btnAdd"));

        horizontalLayout_2->addWidget(m_btnAdd);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(widget);

        scrollArea = new QScrollArea(RegVisualEditorClass);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setMidLineWidth(1);
        scrollArea->setWidgetResizable(true);
        ContentsLayer = new QWidget();
        ContentsLayer->setObjectName(QStringLiteral("ContentsLayer"));
        ContentsLayer->setGeometry(QRect(0, 0, 578, 294));
        ContentsLayer->setMinimumSize(QSize(0, 0));
        scrollArea->setWidget(ContentsLayer);

        verticalLayout->addWidget(scrollArea);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(RegVisualEditorClass);

        QMetaObject::connectSlotsByName(RegVisualEditorClass);
    } // setupUi

    void retranslateUi(QWidget *RegVisualEditorClass)
    {
        RegVisualEditorClass->setWindowTitle(QApplication::translate("RegVisualEditorClass", "RegVisualEditor", 0));
        label->setText(QApplication::translate("RegVisualEditorClass", "Regex", 0));
        m_btnReset->setText(QApplication::translate("RegVisualEditorClass", "\346\270\205\347\251\272", 0));
        m_btnAdd->setText(QApplication::translate("RegVisualEditorClass", "\346\267\273\345\212\240", 0));
    } // retranslateUi

};

namespace Ui {
    class RegVisualEditorClass: public Ui_RegVisualEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGVISUALEDITOR_H
