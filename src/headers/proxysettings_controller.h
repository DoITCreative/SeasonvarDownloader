#ifndef PROXYSETTINGS_CONTROLLER_H
#define PROXYSETTINGS_CONTROLLER_H
#pragma once

#include <QString>
#include <QNetworkProxy>
#include "proxysettings_model.h"

class Proxysettings_controller
{
public:
  Proxysettings_controller();
  ~Proxysettings_controller();
  QString getProxyHost();
  int getProxyPort();
  QNetworkProxy::ProxyType getProxyType();

  void setProxyHost(QString proxyHost);
  void setProxyPort(int proxyPort);
  void setProxyType(QNetworkProxy::ProxyType proxyType);
};

#endif // PROXYSETTINGS_CONTROLLER_H
