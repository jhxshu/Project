#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tcpmgr.h>
#include "resetdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建登录对话框
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 设置为中心控件
    setCentralWidget(_login_dlg);

    // 连接信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);

    // 【关键】连接聊天登录成功信号到切换聊天界面的槽
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_switch_chatdlg,
            this, &MainWindow::SlotSwitchChat);

    // 显示登录对话框（作为中央控件已经显示，不需要额外调用show）
    qDebug() << "MainWindow initialized, waiting for login...";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    qDebug() << "SlotSwitchReg called";

    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(_reg_dlg, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(_reg_dlg);

    // 删除旧的登录对话框
    if(_login_dlg) {
        _login_dlg->deleteLater();
        _login_dlg = nullptr;
    }

    _reg_dlg->show();
}

void MainWindow::SlotSwitchLogin()
{
    qDebug() << "SlotSwitchLogin called";

    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 重新连接信号
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);

    setCentralWidget(_login_dlg);

    if(_reg_dlg) {
        _reg_dlg->deleteLater();
        _reg_dlg = nullptr;
    }

    _login_dlg->show();
}

void MainWindow::SlotSwitchReset()
{
    qDebug() << "SlotSwitchReset called";

    _reset_dlg = new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(_reset_dlg);

    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);

    _reset_dlg->show();
}

void MainWindow::SlotSwitchLogin2()
{
    qDebug() << "SlotSwitchLogin2 called";

    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(_login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);

    setCentralWidget(_login_dlg);

    if(_reset_dlg) {
        _reset_dlg->deleteLater();
        _reset_dlg = nullptr;
    }

    _login_dlg->show();
}

void MainWindow::SlotSwitchChat()
{
    qDebug() << "=== SlotSwitchChat CALLED ===";
    qDebug() << "Switching to chat dialog...";

    // 创建聊天对话框
    _chat_dlg = new ChatDialog(this);
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 设置为中心控件
    setCentralWidget(_chat_dlg);

    // 调整窗口大小
    this->setMinimumSize(QSize(1050, 900));
    this->resize(1050, 900);

    // 删除登录对话框（可选）
    if(_login_dlg) {
        _login_dlg->deleteLater();
        _login_dlg = nullptr;
    }

    qDebug() << "Chat dialog shown successfully";
}
