#ifndef DOWNLOAD_SCREEN_H
#define DOWNLOAD_SCREEN_H
#pragma once

#include <QWidget>
#include <QScreen>
#include <QString>
#include <QSpacerItem>
#include <QScrollArea>
#include <QFileDialog>
#include <QVBoxLayout>
#include <string>
#include <vector>
#include "mainwindow.h"
#include "download_module.h"
#include "utils.h"

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
    void on_pushButtonGoBack_clicked();
    void on_pushButtonChooseDirectory_clicked();

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
