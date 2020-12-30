#ifndef PROXYSETTINGS_MODEL_H
#define PROXYSETTINGS_MODEL_H
#pragma once

#include <QString>
#include <QNetworkProxy>

class Proxysettings_model
{
public:
  inline static QString proxyHost = "";
  inline static int proxyPort = 0;
  inline static QNetworkProxy::ProxyType proxyType = QNetworkProxy::ProxyType::NoProxy;
};

#endif // PROXYSETTINGS_MODEL_H
