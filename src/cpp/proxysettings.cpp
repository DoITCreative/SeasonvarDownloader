#include "../headers/proxysettings.h"
#include "ui_proxysettings.h"

Proxysettings_controller* proxysettings_controller;
ProxySettings::ProxySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProxySettings)
{
  proxysettings_controller = new Proxysettings_controller;
    ui->setupUi(this);
    ui->label_host->setEnabled(false);
    ui->label_port->setEnabled(false);
    ui->lineEdit_host->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);

    if (proxysettings_controller->getProxyHost() != "")
      ui->lineEdit_host->setText(proxysettings_controller->getProxyHost());

    if (proxysettings_controller->getProxyPort() != 0)
      ui->lineEdit_port->setText(QString::number(proxysettings_controller->getProxyPort()));

    ui->radioButton_none->setChecked(false);
    ui->radioButton_http->setChecked(false);
    ui->radioButton_socks5->setChecked(false);
    switch (proxysettings_controller->getProxyType())
    {
        case QNetworkProxy::ProxyType::DefaultProxy:
              ui->radioButton_none->setChecked(true);
              break;
        case QNetworkProxy::ProxyType::FtpCachingProxy:
              ui->radioButton_none->setChecked(true);
              break;
        case QNetworkProxy::ProxyType::HttpCachingProxy:
              ui->radioButton_none->setChecked(true);
              break;
        case QNetworkProxy::ProxyType::HttpProxy:
              ui->radioButton_http->setChecked(true);
              ui->label_host->setEnabled(true);
              ui->label_port->setEnabled(true);
              ui->lineEdit_host->setEnabled(true);
              ui->lineEdit_port->setEnabled(true);
              break;
        case QNetworkProxy::ProxyType::NoProxy:
              ui->radioButton_none->setChecked(true);
              break;
        case QNetworkProxy::ProxyType::Socks5Proxy:
              ui->radioButton_socks5->setChecked(true);
              ui->label_host->setEnabled(true);
              ui->label_port->setEnabled(true);
              ui->lineEdit_host->setEnabled(true);
              ui->lineEdit_port->setEnabled(true);
              break;
    }
}

ProxySettings::~ProxySettings()
{
  delete proxysettings_controller;
    delete ui;
}

void ProxySettings::on_radioButton_socks5_clicked()
{
    ui->label_host->setEnabled(true);
    ui->label_port->setEnabled(true);
    ui->lineEdit_host->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
    if (ui->lineEdit_host->text().isEmpty() && ui->lineEdit_port->text().isEmpty()) {
        ui->lineEdit_host->setText("127.0.0.1");
        ui->lineEdit_port->setText("9050");
    }
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
    proxysettings_controller->setProxyHost(ui->lineEdit_host->text());
    proxysettings_controller->setProxyPort(ui->lineEdit_port->text().toInt());

    if (ui->radioButton_http->isChecked()) proxysettings_controller->setProxyType(QNetworkProxy::ProxyType::HttpProxy);
    if (ui->radioButton_socks5->isChecked()) proxysettings_controller->setProxyType(QNetworkProxy::ProxyType::Socks5Proxy);
    if (ui->radioButton_none->isChecked()) proxysettings_controller->setProxyType(QNetworkProxy::ProxyType::NoProxy);
    close();
}
