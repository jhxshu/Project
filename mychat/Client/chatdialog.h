#ifndef CHATDIALOG_H
#define CHATDIALOG_H
#include <global.h>
#include <QDialog>
#include <statewidget.h>
#include <QList>
namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void addchatUserList();
    void ClearLabelState(StateWidget* lb);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void handleGlobalMousePress(QMouseEvent* event);
private:
    void ShowSearch(bool bsearch = false);
    void AddLBGroup(StateWidget* lb);
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*> _lb_list;
private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_change(const QString& str);
};

#endif // CHATDIALOG_H
