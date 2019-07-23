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
    void on_pushButtonBack_clicked();
    void on_pushButtonSaveLocation_clicked();
    void on_lineEditSaveLocation_textChanged(const QString &arg1);
    void on_lineEditOther_textChanged(const QString &arg1);
    void on_radioButtonWget_clicked();
    void on_radioButtonAria2c_clicked();
    void on_radioButtonOther_clicked();
    void on_pushButtonSaveScript_clicked();

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
