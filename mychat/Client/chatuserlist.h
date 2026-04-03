#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include <QObject>
#include <QListWidget>
#include <QEvent>
#include <QScroller>
#include <QDebug>
class ChatUserList: public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget* parent);
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
