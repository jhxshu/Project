#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include "QStyle"
#include <Mutex>
#include <QRegularExpression>
#include <QNetworkReply>
#include <QDir>
#include <QSettings>

extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorString;

enum ReqId{
    ID_GET_VERIFY_CODE = 1001, //获取验证码
    ID_REG_USER = 1002,  //注册用户
    ID_RESET_PWD = 1003,
    ID_LOGIN_USER = 1004,
    ID_CHAT_LOGIN = 1005,
    ID_CHAT_LOGIN_RSP = 1006,
};
enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1, //json解析失败
    ERROR_NETWORK = 2,  //网络错误
};

extern QString gate_url_prefix;


enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6,
};

enum ClickLbState{
    Normal = 0,
    Selected = 1,
};

struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

#endif // GLOBAL_H
