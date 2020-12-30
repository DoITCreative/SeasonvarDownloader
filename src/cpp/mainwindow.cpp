#include "../headers/mainwindow.h"
#include "../headers/mainwindow_model.h"
#include "../headers/mainwindow_controller.h"
#include "../headers/mainwindow_interface.h"
#include "ui_mainwindow.h"

Mainwindow_interface* mainwindow_controller;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelCloud->setVisible(false); //Make parrot invisible
    ui->textBrowser->setEnabled(false);
    ui->pushButtonLinksDatabase->setEnabled(false); //Hide links button
    ui->pushButtonCopyLinksToClipboard->setEnabled(false); //Copy links to clipboard
    ui->pushButtonDownloadFromLinks->setEnabled(false); //Download screen
    ui->pushButtonGenerateScript->setEnabled(false); //Generate download script
    ui->loadingAnimation->setVisible(false); //Hide loading animation
    loadingAnimation = new QMovie(":/loading-animation.gif"); //Initialize loading animation
    ui->loadingAnimation->setMovie(loadingAnimation);
    this->setGeometry( //Align window to the center of the screen
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),    
                    qApp->screens().first()->geometry()));
    mainwindow_controller = new Mainwindow_controller(this);
}

MainWindow::~MainWindow()
{
    delete mainwindow_controller;
    delete loadingAnimation;
    delete ui;
}

void MainWindow::printUrlError()
{
    QMessageBox messageBox;
    QMessageBox::critical(nullptr,"Error","URL is wrong!\nPlease specify correct one.");
    messageBox.setFixedSize(500,200);
}

void MainWindow::printForbiddenError()
{
    QMessageBox messageBox;
    QMessageBox::critical(nullptr,"Error","Link access is forbidden by provider. Use proxy settings to bypass.");
    messageBox.setFixedSize(500,200);
}

/*
 * Shows loading animation
 */
void MainWindow::startLoadingAnimation()
{
    ui->loadingAnimation->setVisible(true);
    loadingAnimation->start();
}

/*
 * Hides loading animation
 */
void MainWindow::stopLoadingAnimation()
{
    ui->loadingAnimation->setVisible(false);
    loadingAnimation->stop();
}

/*
 * Connect button pressed
 */
void MainWindow::on_pushButtonGetAllTheLinks_clicked()
{
    startLoadingAnimation();
    mainwindow_controller->getAllTheLinks();
    stopLoadingAnimation();
}

QString MainWindow::getPlainTextBrowserData()
{
  return ui->textBrowser->toPlainText();
}

void MainWindow::setTextBrowserData(QString tbData)
{
  ui->textBrowser->clear();
  ui->textBrowser->append(tbData);

  ui->labelCloud->setVisible(true);
  ui->textBrowser->setEnabled(true);
  //ui->pushButtonDownloadFromLinks->setEnabled(true);
  ui->pushButtonCopyLinksToClipboard->setEnabled(true);
  ui->pushButtonGenerateScript->setEnabled(true);

  QTextCursor cursor = ui->textBrowser->textCursor();
  cursor.setPosition(0);
  ui->textBrowser->setTextCursor(cursor);
}

void MainWindow::setUrlText(QString text)
{
  ui->lineEditSeasonvarURL->clear();
  ui->lineEditSeasonvarURL->setText(text);
}

QString MainWindow::getUrlText()
{
  return ui->lineEditSeasonvarURL->text();
}

/*
 * Copy to clipboard
 */
void MainWindow::on_pushButtonCopyLinksToClipboard_clicked()
{
    mainwindow_controller->copyLinksToClipboard();
}

void MainWindow::on_pushButtonDownloadFromLinks_clicked()
{
    Download_screen *downloadScreen = new Download_screen();
    downloadScreen->setCalledFrom(this);
    //downloadScreen->setTokens(tokens);
    downloadScreen->show();
    hide();
}

void MainWindow::on_pushButtonGenerateScript_clicked()
{
    Script_screen *scriptScreen = new Script_screen();
    scriptScreen->setTokens(mainwindow_controller->getTokens());
    scriptScreen->setCalledFrom(this);
    scriptScreen->show();
    hide();
}

void MainWindow::on_pushButtonConfigureProxy_clicked()
{
    ProxySettings *proxySettings = new ProxySettings();
    proxySettings->show();
}
