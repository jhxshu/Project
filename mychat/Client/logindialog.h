#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <global.h>
#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    void initHttpHandlers();
    Ui::LoginDialog *ui;
    void initHead();
    bool checkUserValid();
    bool checkPwdValid();
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    QMap<TipErr, QString> _tip_errs;
    void showTip(QString str, bool b_ok);
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    bool enableBtn(bool enabled);
    int _uid;
    QString _token;
signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo);
public slots:
    void slot_forget_pwd();
private slots:
    void on_login_btn_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    void slot_tcp_con_finish(bool bsuccess);
};

#endif // LOGINDIALOG_H
