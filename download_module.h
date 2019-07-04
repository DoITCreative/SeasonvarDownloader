#ifndef DOWNLOAD_MODULE_H
#define DOWNLOAD_MODULE_H
#pragma once

#include <QWidget>
#include <QString>
#include <QtCore>
#include <string>
#include "download_manager.h"

namespace Ui {
class Download_module;
}

class Download_module : public QWidget
{
    Q_OBJECT

public:
    explicit Download_module(QWidget *parent = nullptr);
    void setProgress(uint m_progress);
    int getProgress();
    void setToken(std::string token);
    std::string getToken();
    void setLabel(const QString &label);
    ~Download_module();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Download_module *ui;
    uint progress = 0;
    std::string token;
};

#endif // DOWNLOAD_MODULE_H
