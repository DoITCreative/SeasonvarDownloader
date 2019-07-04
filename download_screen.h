#ifndef DOWNLOAD_SCREEN_H
#define DOWNLOAD_SCREEN_H
#pragma once

#include <QWidget>
#include <QString>
#include <QSpacerItem>
#include <string>
#include <vector>
#include "download_module.h"
#include <QVBoxLayout>
#include "mainwindow.h"

namespace Ui
{
    class Download_screen;
}

class Download_screen : public QWidget
{
    Q_OBJECT

public:
    explicit Download_screen(QWidget *parent = nullptr);
    void setCalledFrom(QWidget *calledFrom);
    void setTokens(const std::vector<std::string> &tokens);
    ~Download_screen();

private slots:
    void on_pushButton_clicked();
    void on_chooseDirectoryButton_clicked();

private:
    std::vector<Download_module*> modules_list;
    QSpacerItem *qsi;
    std::vector<std::string> tokens;
    Ui::Download_screen *ui;
    QVBoxLayout *vl;
    QWidget *calledFrom = nullptr;
    QLabel *directoryInfo;
    QString directory;
};

#endif // DOWNLOAD_SCREEN_H
