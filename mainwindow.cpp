#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QIcon>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QScrollArea>
#include <QStyle>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(960, 640);
    setWindowTitle(tr("桌面影像處理工具 - Week 10 基礎版"));

    // 實作關鍵 1：在 ui->setupUi(this) 之後，依序初始化工作區、動作、選單與工具列
    setupCentralWidget();
    setupActions();
    setupMenus();
    setupToolBar();

    ui->statusbar->showMessage(tr("就緒"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCentralWidget()
{
    // 實作關鍵 2：設定 centralWidget 作為未來影像工作區
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);

    m_imageLabel = new QLabel(scrollArea);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setText(tr("影像工作區（請由 File -> Open 開啟圖片）"));

    scrollArea->setWidget(m_imageLabel);
    setCentralWidget(scrollArea);
}

void MainWindow::setupActions()
{
    // 實作關鍵 3：建立 Open / Save / Exit 等 QAction，並設定 icon、shortcut 與槽函式
    m_openAction = new QAction(tr("&Open..."), this);
    m_openAction->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    m_openAction->setShortcut(QKeySequence::Open); // Ctrl+O
    m_openAction->setStatusTip(tr("開啟影像檔案"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpenImage);

    m_saveAction = new QAction(tr("&Save..."), this);
    m_saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_saveAction->setShortcut(QKeySequence::Save); // Ctrl+S
    m_saveAction->setStatusTip(tr("儲存目前影像"));
    m_saveAction->setEnabled(false); // 尚未載入圖片前先停用
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveImage);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    m_exitAction->setShortcut(QKeySequence::Quit); // Ctrl+Q
    m_exitAction->setStatusTip(tr("離開程式"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    // 實作關鍵 4：預留 Image / Edit 類別的擴充位置
    m_placeholderEditAction = new QAction(tr("Edit Placeholder"), this);
    m_placeholderEditAction->setStatusTip(tr("後續影像處理功能擴充點"));
    m_placeholderEditAction->setEnabled(false);
}

void MainWindow::setupMenus()
{
    // 實作關鍵 5：直接透過 ui->menubar 建立 File 與 Image / Edit 選單
    m_fileMenu = ui->menubar->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_imageMenu = ui->menubar->addMenu(tr("&Image"));
    m_imageMenu->addAction(m_placeholderEditAction);

    m_editMenu = ui->menubar->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_placeholderEditAction);
}

void MainWindow::setupToolBar()
{
    // 實作關鍵 6：建立 ToolBar 並將常用 QAction 同時加入，與 Menu 共用同一組 QAction
    m_mainToolBar = addToolBar(tr("Main ToolBar"));
    m_mainToolBar->setObjectName("MainToolBar");
    m_mainToolBar->addAction(m_openAction);
    m_mainToolBar->addAction(m_saveAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_exitAction);
}

void MainWindow::onOpenImage()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("開啟影像"),
        QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)")
        );

    if (filePath.isEmpty()) {
        return;
    }

    QImage loaded(filePath);
    if (loaded.isNull()) {
        QMessageBox::warning(this, tr("開啟失敗"), tr("無法讀取所選的影像檔案。"));
        return;
    }

    m_currentImage = loaded;
    m_saveAction->setEnabled(true);
    updateImageView();
    ui->statusbar->showMessage(tr("已載入：%1").arg(filePath));
}

void MainWindow::onSaveImage()
{
    if (m_currentImage.isNull()) {
        return;
    }

    const QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("儲存影像"),
        QString("output.png"),
        tr("PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)")
        );

    if (filePath.isEmpty()) {
        return;
    }

    if (!m_currentImage.save(filePath)) {
        QMessageBox::warning(this, tr("儲存失敗"), tr("無法將影像儲存至指定路徑。"));
        return;
    }

    ui->statusbar->showMessage(tr("已儲存：%1").arg(filePath));
}

void MainWindow::updateImageView()
{
    if (m_currentImage.isNull() || !m_imageLabel) {
        return;
    }
    m_imageLabel->setPixmap(QPixmap::fromImage(m_currentImage));
}