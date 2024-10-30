#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLabel *statusLabel = new QLabel(this);  // 使用指针定义QLabel对象
    statusLabel->setMaximumWidth(150);
    statusLabel->setText("Length: " + QString::number(0) + "    Lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(statusLabel);  // 添加到状态栏，去掉&符号

    QLabel *statusCursorLabel = new QLabel(this);  // 使用指针定义QLabel对象
    statusCursorLabel->setMaximumWidth(150);
    statusCursorLabel->setText("Ln: " + QString::number(0) + "    Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("宋佳伟"));
    ui->statusbar->addPermanentWidget(author);
}

MainWindow::~MainWindow()
{
    delete ui;
}
