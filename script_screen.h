#ifndef SCRIPT_SCREEN_H
#define SCRIPT_SCREEN_H
#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

namespace Ui {
    class Script_screen;
}

class Script_screen : public QWidget
{
    Q_OBJECT

public:
    explicit Script_screen(QWidget *parent = nullptr);
    void setCalledFrom(QWidget *calledFrom);
    void setTokens(const std::vector<std::string> &tokens);
    ~Script_screen();

private slots:
    void on_backButton_clicked();
    void on_saveLocationPushButton_clicked();
    void on_saveLocationLineEdit_textChanged(const QString &arg1);
    void on_otherLineEdit_textChanged(const QString &arg1);
    void on_wgetRadioButton_clicked();
    void on_aria2cRadioButton_clicked();
    void on_otherRadioButton_clicked();
    void on_saveScriptPushButton_clicked();

private:
    void fillScript();
    Ui::Script_screen *ui;
    QWidget *calledFrom = nullptr;
    std::vector<std::string> tokens;
    QString directory;
    const QString defaultScriptName = "downloadScript.sh";
    const QString defaultScriptHeader = "#!/usr/bin/env bash";
    QString selectedDownloadProgramName = "aria2c";
};

#endif // SCRIPT_SCREEN_H
