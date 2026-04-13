#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
#include <QRandomGenerator>
#include "chatuserwid.h"
#include "loadingdlg.h"
#include <QMouseEvent>
ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode), _state(ChatUIMode::ChatMode), _b_loading(false)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->SetMaxLength(15);

    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    //初始时不显示清除图标
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);

    connect(ui->search_edit, &QLineEdit::textChanged, [clearAction](const QString &text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/res/close_search.png"));
        }else{
            clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        }
    });
    connect(clearAction, &QAction::triggered, [this, clearAction](){
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        ui->search_edit->clearFocus();
        ShowSearch(false);
    });

    ShowSearch(false);
    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    addchatUserList();

    QPixmap pixmap(":/res/head_1.jpg");
    ui->side_head_lb->setPixmap(pixmap);
    QPixmap scaledPixmap = pixmap.scaled(ui->side_head_lb->size(), Qt::KeepAspectRatio);
    ui->side_head_lb->setPixmap(scaledPixmap);
    ui->side_head_lb->setScaledContents(true);
    ui->side_chat_lb->setProperty("state", "normal");
    ui->search_list->SetSearchEdit(ui->search_edit);


    ui->side_chat_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");
    ui->side_contact_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");

    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);

    connect(ui->side_chat_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_contact);
    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_change);
    this->installEventFilter(this);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

// std::vector<QString>  strs ={"hello world !",
//                              "nice to meet u",
//                              "New year，new life",
//                              "You have to love yourself",
//                              "My love is written in the wind ever since the whole world is you"};

// std::vector<QString> heads = {
//     ":/res/head_1.jpg",
//     ":/res/head_2.jpg",
//     ":/res/head_3.jpg",
//     ":/res/head_4.jpg",
//     ":/res/head_5.jpg"
// };

// std::vector<QString> names = {
//     "llfc",
//     "zack",
//     "golang",
//     "cpp",
//     "java",
//     "nodejs",
//     "python",
//     "rust"
// };

void ChatDialog::addchatUserList()
{
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(99);
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

    auto *chat_user_wid = new ChatUserWid();
    chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug() << "chat_user_wid sizeHintis " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->addItem(item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    }
}

void ChatDialog::ShowSearch(bool bsearch){
    if(bsearch){
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }
    else if(_state == ChatUIMode::ChatMode){
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    }
    else if(_state == ChatUIMode::ContactMode){
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::AddLBGroup(StateWidget *lb)
{
    _lb_list.push_back(lb);
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading){
        return;
    }
    _b_loading = true;
    LoadingDlg* loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->move(this->rect().center() - loadingDialog->rect().center()); // 居中
    loadingDialog->show();
    qDebug() << "add new data to list...";
    addchatUserList();
    //关闭对话框
    loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::slot_side_chat()
{
    qDebug() << "receive side chat clicked";
    // 先清所有标签状态，同时把side_chat_lb设为选中
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact()
{
    qDebug() << "receive side contact clicked";
    // 同理，清所有、设当前contact为选中
    ClearLabelState(ui->side_contact_lb);
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);
}

void ChatDialog::slot_text_change(const QString &str)
{
    if(!str.isEmpty()){
        ShowSearch(true);
    }
}

void ChatDialog::ClearLabelState(StateWidget* lb)
{
    for(auto &ele : _lb_list){
        if(ele == lb){
            continue;
        }
        ele->ClearState();
    }
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }

    return QDialog::eventFilter(watched, event);
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    if(_mode != ChatUIMode::SearchMode){
        return;
    }
    QPoint posInSearchList = ui->search_list->mapFromGlobal(event->globalPos());
    if(!ui->search_list->rect().contains(posInSearchList)){
        ui->search_edit->clear();
        ShowSearch(false);
    }
}
