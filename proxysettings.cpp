#include "proxysettings.h"
#include "ui_proxysettings.h"

ProxySettings::ProxySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProxySettings)
{
    ui->setupUi(this);
    ui->label_host->setEnabled(false);
    ui->label_port->setEnabled(false);
    ui->lineEdit_host->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
}

ProxySettings::~ProxySettings()
{
    delete ui;
}

void ProxySettings::on_radioButton_socks5_clicked()
{
    ui->label_host->setEnabled(true);
    ui->label_port->setEnabled(true);
    ui->lineEdit_host->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
}

void ProxySettings::on_radioButton_http_clicked()
{
    ui->label_host->setEnabled(true);
    ui->label_port->setEnabled(true);
    ui->lineEdit_host->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
}

void ProxySettings::on_radioButton_none_clicked()
{
    ui->label_host->setEnabled(false);
    ui->label_port->setEnabled(false);
    ui->lineEdit_host->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
}

void ProxySettings::on_pushButton_apply_clicked()
{
    MainWindow::use_proxy = !(ui->radioButton_none->isChecked());
    MainWindow::proxy_host = ui->lineEdit_host->text();
    MainWindow::proxy_port = ui->lineEdit_port->text().toUInt();
    if (ui->radioButton_http->isChecked()) MainWindow::proxy_type = "http";
    if (ui->radioButton_socks5->isChecked()) MainWindow::proxy_type = "socks5";
    if (ui->radioButton_none->isChecked()) MainWindow::proxy_type = "";
    close();
}
