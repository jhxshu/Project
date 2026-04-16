#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include "httpmgr.h"
#include "tcpmgr.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    _uid(0)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);
    initHead();
    initHttpHandlers();

    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish,
            this, &LoginDialog::slot_login_mod_finish);
    connect(this, &LoginDialog::sig_connect_tcp,
            TcpMgr::GetInstance().get(), &TcpMgr::slot_tcp_connect);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_con_success,
            this, &LoginDialog::slot_tcp_con_finish);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_login_failed,
            this, &LoginDialog::slot_login_failed);


    qDebug() << "LoginDialog created, sig_switch_chatdlg connected";
}

LoginDialog::~LoginDialog(){
    qDebug() << "destruct LoginDialog.";
    delete ui;
}

void LoginDialog::initHttpHandlers()
{
    //注册获取登录回包逻辑
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
        qDebug() << "=== Login Response JSON ===";
        qDebug() << "error:" << jsonObj["error"].toInt();
        qDebug() << "email:" << jsonObj["email"].toString();
        qDebug() << "uid:" << jsonObj["uid"].toInt();
        qDebug() << "host:" << jsonObj["host"].toString();
        qDebug() << "port:" << jsonObj["port"].toInt();
        qDebug() << "token:" << jsonObj["token"].toString();

        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            enableBtn(true);
            return;
        }
        auto email = jsonObj["email"].toString();

        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = QString::number(jsonObj["port"].toInt());
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;

        qDebug() << "email is " << email << " uid is " << si.Uid
                 << " host is " << si.Host << " Port is " << si.Port
                 << " Token is " << si.Token;

        emit sig_connect_tcp(si);
    });
}

bool LoginDialog::checkPwdValid(){
    auto pwd = ui->pass_edit->text();
    if(pwd.length() < 6 || pwd.length() > 15){
        qDebug() << "Pass length invalid";
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pwd).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符且长度为(6~15)"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->login_btn->setEnabled(enabled);
    ui->reg_btn->setEnabled(enabled);
    return true;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug() << "slot forget pwd.";
    emit switchReset();
}

void LoginDialog::initHead()
{
    QPixmap originalPixmap(":/res/ico1.png");
    qDebug()<< originalPixmap.size() << ui->head_label->size();
    originalPixmap = originalPixmap.scaled(ui->head_label->size(),
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10);
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, originalPixmap);
    ui->head_label->setPixmap(roundedPixmap);
}

bool LoginDialog::checkUserValid()
{
    auto email = ui->email_edit->text();
    if(email.isEmpty()){
        qDebug() << "email empty.";
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱不为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te){
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
        ui->email_edit->setProperty("state", "normal");
    }
    else{
        ui->email_edit->setProperty("state", "err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug() << "login btn clicked.";
    if(checkUserValid() == false){
        return;
    }
    if(checkPwdValid() == false){
        return;
    }
    enableBtn(false);
    auto email = ui->email_edit->text();
    auto pwd = ui->pass_edit->text();

    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = xorString(pwd);
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        json_obj, ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    qDebug() << "Raw response:" << res;

    if(err != ErrorCodes::SUCCESS){
        showTip(tr("网络请求错误"),false);
        enableBtn(true);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTip(tr("json解析错误"),false);
        enableBtn(true);
        return;
    }

    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        enableBtn(true);
        return;
    }

    _handlers[id](jsonDoc.object());
}

void LoginDialog::slot_tcp_con_finish(bool bsuccess)
{
    qDebug() << "=== slot_tcp_con_finish called, success:" << bsuccess;

    if(bsuccess){
        showTip(tr("聊天服务连接成功，正在登录..."), true);
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;

        QJsonDocument doc(jsonObj);
        QByteArray jsonString = doc.toJson(QJsonDocument::Indented);
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonString);

        qDebug() << "Emitting sig_switch_chat from LoginDialog";
        emit TcpMgr::GetInstance()->sig_switch_chatdlg();  // 确保这一行存在
    }
    else{
        showTip(tr("网络异常"), false);
        enableBtn(true);
    }
}

void LoginDialog::slot_login_failed(int err){
    QString result = QString("登录失败, err is %1 ").arg(err);
    showTip(result, false);
    enableBtn(true);
}


