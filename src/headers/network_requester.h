#ifndef NETWORK_REQUESTER_H
#define NETWORK_REQUESTER_H
#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkProxy>

class Network_requester: public QObject
{
  Q_OBJECT

public:
  QPair<QString,bool> getRequest(QString url, QString proxyHost = "", int proxyPort = 0, QNetworkProxy::ProxyType proxyType = QNetworkProxy::ProxyType::NoProxy);
};

#endif // NETWORK_REQUESTER_H
