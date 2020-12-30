#include "../headers/network_requester.h"

QPair<QString,bool> Network_requester::getRequest(QString url, QString proxyHost, int proxyPort, QNetworkProxy::ProxyType proxyType)
{
  QString output = "";
  QNetworkAccessManager manager;

  //Set proxy
  QNetworkProxy proxy;
  proxy.setHostName(proxyHost);
  proxy.setPort(proxyPort);
  proxy.setType(proxyType);
  manager.setProxy(proxy);

  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
  QEventLoop loop;
  connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  QTimer timer;

  //Timeout
  connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
  timer.start(5000);

  loop.exec();
  bool successfully_read = false;
  QByteArray result = reply->readAll();
  if ( reply->error() == QNetworkReply::NoError ) {
      output = result;
      successfully_read = true;
  }
  QPair<QString, bool> outpair;
  outpair.first = output;
  outpair.second = successfully_read;
  return outpair;
}
