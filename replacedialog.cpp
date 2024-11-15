#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit* textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pTextEdit = textEdit;

    ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btFindNext_clicked()
{
    QString target = ui->SearchNext->text();

    if(target==""||pTextEdit==nullptr){
        return;
    }

    QString text = pTextEdit->toPlainText();
    QTextCursor c=pTextEdit->textCursor();
    int index = -1;

    if(ui->rbDown->isChecked()){
        index = text.indexOf(target,c.position(),ui->cbCaseInsensitive ? Qt::CaseInsensitive : Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index);
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->rbUp){
        index = text.lastIndexOf(target,c.position()-text.length()-1,
                                 ui->cbCaseInsensitive ? Qt::CaseInsensitive : Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }

    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("提示");
        msg.setText(QString("找不到")+target);
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void ReplaceDialog::on_btReplace_clicked()
{
    // 获取要查找的文本和替换的文本
    QString target = ui->SearchNext->text();
    QString to = ui->ReplaceEdit->text();

    // 检查条件是否满足
    if (pTextEdit != nullptr && !target.isEmpty() && !to.isEmpty()) {
        QTextCursor cursor = pTextEdit->textCursor();

        // 检查当前选中的文本是否与目标文本匹配
        if (cursor.selectedText() == target) {
            cursor.insertText(to); // 用替换文本覆盖当前选中的文本
            pTextEdit->setTextCursor(cursor); // 更新光标位置
        }

        // 调用查找函数，查找下一个匹配项
        on_btFindNext_clicked();
    }
}


void ReplaceDialog::on_btAllReplace_clicked()
{
    QString target = ui->SearchNext->text();
    QString to = ui->ReplaceEdit->text();

    if (pTextEdit != nullptr && !target.isEmpty() && !to.isEmpty()) {
        QTextCursor cursor = pTextEdit->textCursor();
        cursor.beginEditBlock();

        // 将光标移动到文档的起始位置
        cursor.movePosition(QTextCursor::Start);

        int replaceCount = 0;

        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = pTextEdit->document()->find(target, cursor, ui->cbCaseInsensitive->isChecked() ? QTextDocument::FindCaseSensitively : QTextDocument::FindFlags());

            if (!cursor.isNull()) {
                // 找到匹配项后替换
                cursor.insertText(to);
                replaceCount++; // 增加替换次数
            }
        }

        cursor.endEditBlock(); // 结束批处理

        QMessageBox::information(this, "替换完成", QString("共替换了 %1 处匹配项").arg(replaceCount));
    }
}



void ReplaceDialog::on_btCancel_clicked()
{
    accept();
}

