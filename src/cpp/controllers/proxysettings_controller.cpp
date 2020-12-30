#include "../../headers/proxysettings_controller.h"

Proxysettings_controller::Proxysettings_controller()
{

}

Proxysettings_controller::~Proxysettings_controller()
{

}

QString Proxysettings_controller::getProxyHost()
{
  return Proxysettings_model::proxyHost;
}

int Proxysettings_controller::getProxyPort()
{
  return Proxysettings_model::proxyPort;
}

QNetworkProxy::ProxyType Proxysettings_controller::getProxyType()
{
  return Proxysettings_model::proxyType;
}

void Proxysettings_controller::setProxyHost(QString proxyHost)
{
  Proxysettings_model::proxyHost = proxyHost;
}

void Proxysettings_controller::setProxyPort(int proxyPort)
{
  Proxysettings_model::proxyPort = proxyPort;
}

void Proxysettings_controller::setProxyType(QNetworkProxy::ProxyType proxyType)
{
  Proxysettings_model::proxyType = proxyType;
}

