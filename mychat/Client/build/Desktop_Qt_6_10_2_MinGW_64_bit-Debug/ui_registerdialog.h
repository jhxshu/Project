/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QVBoxLayout *verticalLayout_6;
    QLabel *err_tip;
    QHBoxLayout *horizontalLayout_2;
    QLabel *user_label;
    QLineEdit *user_edit;
    QHBoxLayout *horizontalLayout_16;
    QLabel *emaillabel;
    QLineEdit *email_edit;
    QHBoxLayout *horizontalLayout_17;
    QLabel *pass_label;
    QLineEdit *pass_edit;
    QHBoxLayout *horizontalLayout_18;
    QLabel *confirm_label;
    QLineEdit *confirm_edit;
    QHBoxLayout *horizontalLayout_19;
    QLabel *verify_label;
    QLineEdit *verify_edit;
    QPushButton *get_code;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_20;
    QPushButton *confirm_btn;
    QPushButton *cancel_btn;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_4;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(300, 500);
        RegisterDialog->setMinimumSize(QSize(300, 500));
        verticalLayout_2 = new QVBoxLayout(RegisterDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(RegisterDialog);
        stackedWidget->setObjectName("stackedWidget");
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        verticalLayout_4 = new QVBoxLayout(page_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        widget = new QWidget(page_3);
        widget->setObjectName("widget");
        verticalLayout_6 = new QVBoxLayout(widget);
        verticalLayout_6->setObjectName("verticalLayout_6");
        err_tip = new QLabel(widget);
        err_tip->setObjectName("err_tip");
        err_tip->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_6->addWidget(err_tip);


        verticalLayout_4->addWidget(widget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        user_label = new QLabel(page_3);
        user_label->setObjectName("user_label");
        user_label->setMinimumSize(QSize(0, 25));
        user_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(user_label);

        user_edit = new QLineEdit(page_3);
        user_edit->setObjectName("user_edit");
        user_edit->setMinimumSize(QSize(0, 25));
        user_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(user_edit);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        emaillabel = new QLabel(page_3);
        emaillabel->setObjectName("emaillabel");
        emaillabel->setMinimumSize(QSize(0, 25));
        emaillabel->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_16->addWidget(emaillabel);

        email_edit = new QLineEdit(page_3);
        email_edit->setObjectName("email_edit");
        email_edit->setMinimumSize(QSize(0, 25));
        email_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_16->addWidget(email_edit);


        verticalLayout_4->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        pass_label = new QLabel(page_3);
        pass_label->setObjectName("pass_label");
        pass_label->setMinimumSize(QSize(0, 25));
        pass_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_17->addWidget(pass_label);

        pass_edit = new QLineEdit(page_3);
        pass_edit->setObjectName("pass_edit");
        pass_edit->setMinimumSize(QSize(0, 25));
        pass_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_17->addWidget(pass_edit);


        verticalLayout_4->addLayout(horizontalLayout_17);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        confirm_label = new QLabel(page_3);
        confirm_label->setObjectName("confirm_label");
        confirm_label->setMinimumSize(QSize(0, 25));
        confirm_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_18->addWidget(confirm_label);

        confirm_edit = new QLineEdit(page_3);
        confirm_edit->setObjectName("confirm_edit");
        confirm_edit->setMinimumSize(QSize(0, 25));
        confirm_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_18->addWidget(confirm_edit);


        verticalLayout_4->addLayout(horizontalLayout_18);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName("horizontalLayout_19");
        verify_label = new QLabel(page_3);
        verify_label->setObjectName("verify_label");
        verify_label->setMinimumSize(QSize(0, 25));
        verify_label->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_19->addWidget(verify_label);

        verify_edit = new QLineEdit(page_3);
        verify_edit->setObjectName("verify_edit");
        verify_edit->setMinimumSize(QSize(0, 25));
        verify_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_19->addWidget(verify_edit);

        get_code = new QPushButton(page_3);
        get_code->setObjectName("get_code");
        get_code->setMinimumSize(QSize(0, 25));
        get_code->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_19->addWidget(get_code);


        verticalLayout_4->addLayout(horizontalLayout_19);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName("horizontalLayout_20");
        confirm_btn = new QPushButton(page_3);
        confirm_btn->setObjectName("confirm_btn");
        confirm_btn->setMinimumSize(QSize(0, 25));
        confirm_btn->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_20->addWidget(confirm_btn);

        cancel_btn = new QPushButton(page_3);
        cancel_btn->setObjectName("cancel_btn");
        cancel_btn->setMinimumSize(QSize(0, 25));
        cancel_btn->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_20->addWidget(cancel_btn);


        verticalLayout_4->addLayout(horizontalLayout_20);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        stackedWidget->addWidget(page_4);

        verticalLayout->addWidget(stackedWidget);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        err_tip->setText(QCoreApplication::translate("RegisterDialog", "\351\224\231\350\257\257\346\217\220\347\244\272", nullptr));
        user_label->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        emaillabel->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        pass_label->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        confirm_label->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        verify_label->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        get_code->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226\351\252\214\350\257\201\347\240\201", nullptr));
        confirm_btn->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244", nullptr));
        cancel_btn->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
