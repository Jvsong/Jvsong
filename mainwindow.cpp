#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <aboutdialog.h>
#include <searchdialog.h>
#include <replacedialog.h>
#include <qfiledialog.h>
#include <QMessageBox>
#include <QAction>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;
    on_actionNew_triggered();

    // QLabel *statusLabel = new QLabel(this);  // 使用指针定义QLabel对象
    statusLabel.setMaximumWidth(150);
    statusLabel.setText("Length: " + QString::number(0) + "    Lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);  // 添加到状态栏，去掉&符号

    // QLabel *statusCursorLabel = new QLabel(this);  // 使用指针定义QLabel对象
    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "    Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("宋佳伟"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionRecall->setEnabled(false);
    ui->actionBack->setEnabled(false);
    ui->actionCut->setEnabled(false);

    ui->actionTool->setChecked(true);
    ui->actionState->setChecked(true);
    on_actionShowline_triggered(false);
    // statusLabel = ui->statusbar->findChild<QLabel*>("statusLabel");

    connect(ui->TextEdit, &QPlainTextEdit::undoAvailable, this, &MainWindow::on_TextEdit_undoAvailable);
    connect(ui->TextEdit, &QPlainTextEdit::redoAvailable, this, &MainWindow::on_TextEdit_redoAvailable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dig;
    dig.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dig(this,ui->TextEdit);
    dig.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig(this,ui->TextEdit);
    dig.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed()){
        return;
    }

    filePath = "";

    ui->TextEdit->clear();
    this->setWindowTitle("新建文本文件-编辑器");

    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{
    if(filePath == ""){
        QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.*)"));

        // 检查用户是否取消了保存文件
        if (filename.isEmpty()) {
            return;
        }

        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this,"..","打开文件失败");
            return;
        }
        file.close();
        filePath = filename;
    }

    QFile file(filePath);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开文件失败");
        return;
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text; // 将文本写入文件
    file.flush(); // 确保文件数据被写入
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionOSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt)"));

    if (filename.isEmpty()) {
        return;  // 用户取消了保存操作，直接返回
    }

    QFile file(filename);
    // 使用 WriteOnly 模式来保存文件
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","无法打开文件进行保存");
        return;
    }

    // 在此处保存文件内容
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;  // 将文本写入到文件

    file.flush();  // 确保数据写入文件
    file.close();  // 关闭文件

    // 更新文件路径，文件已保存
    filePath = filename;
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_actionOpen_triggered()
{
    // 如果有未保存的修改，提示用户
    if (!userEditConfirmed()) {
        return;
    }

    // 让用户选择文件并打开
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt);;All files (*.*)"));
    if (filename.isEmpty()) {
        return;  // 用户取消了打开操作
    }

    QFile file(filename);

    // 以只读文本模式打开文件
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    // 更新文件路径
    filePath = filename;

    // 读取文件内容并将其插入到文本框
    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->setPlainText(text);  // 使用 setPlainText 替代 insertPlainText，避免追加文本

    file.close();

    // 更新窗口标题
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;  // 文件打开后标记为未修改
}

void MainWindow::on_TextEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*"+this->windowTitle());
        textChanged=true;
    }

    statusLabel.setText("length: "+ QString::number(ui->TextEdit->toPlainText().length()) +
                        "  lines: "+
                        QString::number(ui->TextEdit->document()->lineCount()));
}


bool MainWindow::userEditConfirmed()
{
    QString path = (filePath !="") ? filePath : "无标题！";

    if(textChanged){
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n")+"\"" + path + "\" ?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int r = msg.exec();
        switch( r ){
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }

    return true;
}

void MainWindow::on_actionRecall_triggered()
{
    ui->TextEdit->undo();
}


void MainWindow::on_actionBack_triggered()
{
    ui->TextEdit->redo();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->TextEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->TextEdit->paste();
}

void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}

void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionRecall->setEnabled(b);  // 启用或禁用撤销按钮
}

void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionBack->setEnabled(b);  // 启用或禁用前进按钮
}

void MainWindow::on_actionfontcolor_triggered() //字体颜色
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit{color:%1}").arg(color.name()));
    }
}


void MainWindow::on_actionBackgroundcolor_triggered() //字体背景颜色
{

}


void MainWindow::on_actionEditbackgroundcolor_triggered() // 背景颜色
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if (color.isValid()) {
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit { background-color: %1; }").arg(color.name()));
    }
}



void MainWindow::on_actionNewline_triggered() //自动换行
{
    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if( mode == QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionNewline->setChecked(true);
    }else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionNewline->setChecked(false);
    }
}

void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok){
        ui->TextEdit->setFont(font);
    }
}

void MainWindow::on_actionState_triggered()
{
    bool visable = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visable);
    ui->actionState->setChecked(!visable);
}

void MainWindow::on_actionTool_triggered()
{
    bool visable = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visable);
    ui->actionTool->setChecked(!visable);

}

void MainWindow::on_actionAllselect_triggered()
{
    ui->TextEdit->selectAll();
}

void MainWindow::on_actionQuit_triggered()
{
    if(userEditConfirmed()){
        exit(0);
    }
}

void MainWindow::on_TextEdit_cursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg = 0;  // 从0开始以正确计算第一行
    int pos = ui->TextEdit->textCursor().position();
    QString text = ui->TextEdit->toPlainText();

    // 计算行号和行首位置
    for (int i = 0; i < pos; i++) {
        if (text[i] == '\n') {
            ln++;
            flg = i + 1;  // 更新为下一行行首位置
        }
    }

    // 计算列号
    col = pos - flg;
    statusCursorLabel.setText("Ln: " + QString::number(ln + 1) + "   Col: " + QString::number(col + 1));
}



void MainWindow::on_actionShowline_triggered(bool checked)
{
    ui->TextEdit->hideLineNumberArea(checked);
}


