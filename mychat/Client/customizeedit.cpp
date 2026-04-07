#include "customizeedit.h"

CustomizeEdit::CustomizeEdit(QWidget *parent)
    : QLineEdit(parent)    // 调用父类构造
    , _max_len(0)          // 初始化变量，防止崩溃
{
    connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::limitTextLength);
}

void CustomizeEdit::SetMaxLength(int maxLen)
{
    _max_len = maxLen;
}
