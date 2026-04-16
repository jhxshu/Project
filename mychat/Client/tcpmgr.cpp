#include "tcpmgr.h"
#include <QAbstractSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "usermgr.h"

TcpMgr::TcpMgr(): _host(""), _port(0), _b_recv_pending(false), _message_id(0), _message_len(0)
{
    QObject::connect(&_socket, &QTcpSocket::connected, [&](){
        qDebug() << "Connected to server!";  //连续建立后发送信息
        emit sig_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]() {
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);

    forever{
        if(!_b_recv_pending){
            if(_buffer.size() < static_cast<int>(sizeof(quint16)*2)){
                return;
            }
            stream >> _message_id >> _message_len;
            _buffer = _buffer.mid(sizeof(quint16)*2);
            qDebug() << "Message ID: " << _message_id << ", length: " <<_message_len;
        }

        if(_buffer.size() < _message_len){
            _b_recv_pending = true;
            return;
        }

        _b_recv_pending = false;
        QByteArray messageBody = _buffer.mid(0, _message_len);
        qDebug() << "receive body msg is " << messageBody;

        _buffer = _buffer.mid(_message_len);
        handleMsg(ReqId(_message_id), _message_len, messageBody);
    }
    });

    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
           qDebug() << "Error:" << _socket.errorString();
       });

    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]{
        qDebug() << "Disconnected from server.";
    });

    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    initHandlers();
}

TcpMgr::~TcpMgr(){

}

void TcpMgr::CloseConnection(){
    _socket.close();
}

void TcpMgr::initHandlers(){
    _handlers.insert((ID_CHAT_LOGIN_RSP), [this](ReqId id, int len, QByteArray data){
        qDebug() << "=== ID_CHAT_LOGIN_RSP HANDLER START ===";
        qDebug() << "id:" << id << "len:" << len;
        qDebug() << "data:" << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if(jsonDoc.isNull()){
            qDebug() << "ERROR: jsonDoc is NULL!";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        qDebug() << "jsonObj:" << jsonObj;

        if(!jsonObj.contains("error")){
            qDebug() << "ERROR: No error field in response";
            emit sig_login_failed(ErrorCodes::ERR_JSON);
            return;
        }

        int err = jsonObj["error"].toInt();
        qDebug() << "error code:" << err;

        if(err != ErrorCodes::SUCCESS){
            qDebug() << "ERROR: Login failed with error:" << err;
            emit sig_login_failed(err);
            return;
        }

        // 保存用户信息
        if(jsonObj.contains("uid")){
            UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
            qDebug() << "Set uid:" << jsonObj["uid"].toInt();
        }
        if(jsonObj.contains("name")){
            UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
            qDebug() << "Set name:" << jsonObj["name"].toString();
        }
        if(jsonObj.contains("token")){
            UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
            qDebug() << "Set token:" << jsonObj["token"].toString();
        }

        qDebug() << "Emitting sig_switch_chatdlg...";
        emit sig_switch_chatdlg();
        qDebug() << "=== ID_CHAT_LOGIN_RSP HANDLER END ===";
    });

    _handlers.insert(ID_SEARCH_USER_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug()<< "handle id is "<< id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if(jsonDoc.isNull()){
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err ;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug() << "Login Failed, err is " << err ;
            emit sig_login_failed(err);
            return;
        }

        auto search_info = std::make_shared<SearchInfo>(jsonObj["uid"].toInt(),
                                                        jsonObj["name"].toString(), jsonObj["nick"].toString(),
                                                        jsonObj["desc"].toString(), jsonObj["sex"].toInt(), jsonObj["icon"].toString());

        emit sig_user_search(search_info);
    });

    _handlers.insert(ID_ADD_FRIEND_RSP, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Add Friend Failed, err is Json Parse Err" << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Add Friend Failed, err is " << err;
            return;
        }

        qDebug() << "Add Friend Success " ;
    });

    _handlers.insert(ID_NOTIFY_ADD_FRIEND_REQ, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err;

            emit sig_user_search(nullptr);
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Login Failed, err is " << err;
            emit sig_user_search(nullptr);
            return;
        }

        int from_uid = jsonObj["applyuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString desc = jsonObj["desc"].toString();
        QString icon = jsonObj["icon"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();

        auto apply_info = std::make_shared<AddFriendApply>(
            from_uid, name, desc,
            icon, nick, sex);

        emit sig_friend_apply(apply_info);
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data){
    auto find_iter = _handlers.find(id);
    if(find_iter == _handlers.end()){
        qDebug() << "not found id [" << id << "] to handle";
        return;
    }
    find_iter.value()(id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si){
    qDebug() << "receive tcp connect signal.";
    qDebug() << "Connecting to server...";

    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(si.Host, _port);
}

void TcpMgr::slot_send_data(ReqId reqId, QString data){
    uint16_t id = reqId;
    QByteArray dataBytes = data.toUtf8();
    quint16 len = static_cast<quint16>(data.size());

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);

    out << id << len;
    block.append(data.toUtf8());
    _socket.write(block);
}

