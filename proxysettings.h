#ifndef PROXYSETTINGS_H
#define PROXYSETTINGS_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class ProxySettings;
}

class ProxySettings : public QWidget
{
    Q_OBJECT

public:
    explicit ProxySettings(QWidget *parent = nullptr);
    ~ProxySettings();

private slots:
    void on_radioButton_socks5_clicked();
    void on_radioButton_http_clicked();
    void on_radioButton_none_clicked();
    void on_pushButton_apply_clicked();

private:
    Ui::ProxySettings *ui;
};

#endif // PROXYSETTINGS_H
