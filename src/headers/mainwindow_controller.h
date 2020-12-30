#ifndef MAINWINDOW_CONTROLLER_H
#define MAINWINDOW_CONTROLLER_H
#pragma once

#include "mainwindow_interface.h"
#include "mainwindow_model.h"
#include "mainwindow.h"
#include "network_requester.h"
#include <QApplication>
#include <QNetworkAccessManager>
#include <QClipboard>
#include <QString>
#include <QVector>

class Mainwindow_controller: public Mainwindow_interface
{
private:
  QObject* view;
  bool checkUrl(QString url);
  void network_request(QString* url, QString film_id, QString seasonvar_full_url);
  void parse_response(QString response);
  bool differentLink = false;
public:
    Mainwindow_controller(QObject* view);
    ~Mainwindow_controller();

    void getAllTheLinks() override;
    void copyLinksToClipboard() override;
    void downloadFromLinks() override;
    void generateScript() override;
    void linksDatabase() override;
    void configureProxy() override;

    void setUrlText(QString text) override;
    QString getUrlText() override;

    void setTextBrowserData(QString tbData) override;
    QString getTextBrowserData() override;

    QVector<QString> getTokens() override;
};

#endif
