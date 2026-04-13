#include "findsuccessdlg.h"
#include "ui_findsuccessdlg.h"
#include <QDir>
#include "applyfriend.h"
FindSuccessDlg::FindSuccessDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindSuccessDlg), _parent(parent)
{
    ui->setupUi(this);
    setWindowTitle("添加");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path + QDir::separator() + "static" + QDir::separator() + "head_1.jpg");

    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal", "hover", "press");
    this->setModal(true);
}

FindSuccessDlg::~FindSuccessDlg()
{
    delete ui;
}

void FindSuccessDlg::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "=== FindSuccessDlg::SetSearchInfo ===";
    qDebug() << "si address:" << si.get();
    qDebug() << "si use_count:" << si.use_count();

    if (si) {
        qDebug() << "si->_name:" << si->_name;
        ui->name_lb->setText(si->_name);
    } else {
        qDebug() << "ERROR: Received null si!";
    }

    _si = si;
    qDebug() << "_si stored, use_count:" << _si.use_count();
}

void FindSuccessDlg::on_add_friend_btn_clicked()
{
    this->hide();
    //弹出加好友界面
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->SetSearchInfo(_si);
    applyFriend->setModal(true);
    applyFriend->show();
}
