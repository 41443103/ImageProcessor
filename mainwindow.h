#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QAction;
class QLabel;
class QMenu;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenImage();
    void onSaveImage();

private:
    void setupCentralWidget();
    void setupActions();
    void setupMenus();
    void setupToolBar();
    void updateImageView();

    Ui::MainWindow *ui;

    // 未來影像工作區的基礎
    QLabel *m_imageLabel = nullptr;
    QImage m_currentImage;

    // 核心 QAction（Open / Save / Exit 與預留編輯動作）
    QAction *m_openAction = nullptr;
    QAction *m_saveAction = nullptr;
    QAction *m_exitAction = nullptr;
    QAction *m_placeholderEditAction = nullptr;

    // 選單與工具列
    QMenu *m_fileMenu = nullptr;
    QMenu *m_imageMenu = nullptr;
    QMenu *m_editMenu = nullptr;
    QToolBar *m_mainToolBar = nullptr;
};

#endif // MAINWINDOW_H