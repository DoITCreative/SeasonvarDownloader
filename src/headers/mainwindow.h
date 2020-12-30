#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QScreen>
#include <QUrl>
#include <QWidget>
#include <QMovie>
#include <QStyle>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QClipboard>
#include "download_screen.h"
#include "script_screen.h"
#include "proxysettings.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setTextBrowserData(QString tbData);
    QString getPlainTextBrowserData();
    void setUrlText(QString text);
    QString getUrlText();
    void printForbiddenError();
    void printUrlError();
    ~MainWindow();

private slots:
    void on_pushButtonGetAllTheLinks_clicked();
    void on_pushButtonCopyLinksToClipboard_clicked();
    void on_pushButtonDownloadFromLinks_clicked();
    void on_pushButtonGenerateScript_clicked();
    void on_pushButtonConfigureProxy_clicked();

private:
    Ui::MainWindow *ui;
    QMovie *loadingAnimation;
    void startLoadingAnimation();
    void stopLoadingAnimation();
};

#endif // MAINWINDOW_H
