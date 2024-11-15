#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qlabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    bool userEditConfirmed();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionOSave_triggered();

    void on_TextEdit_textChanged();


    void on_actionRecall_triggered();

    void on_actionBack_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_TextEdit_undoAvailable(bool b);

    void on_TextEdit_copyAvailable(bool b);

    void on_TextEdit_redoAvailable(bool b);

    void on_actionfontcolor_triggered();

    void on_actionBackgroundcolor_triggered();

    void on_actionEditbackgroundcolor_triggered();

    void on_actionNewline_triggered();

    void on_actionFont_triggered();

    void on_actionState_triggered();

    void on_actionTool_triggered();

    void on_actionAllselect_triggered();

    void on_actionQuit_triggered();

    void on_TextEdit_cursorPositionChanged();

    void on_actionShowline_triggered(bool checked);

    void on_actionShowline_triggered();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChanged;

};

#endif // MAINWINDOW_H
