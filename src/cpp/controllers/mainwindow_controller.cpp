#include "../../headers/mainwindow_controller.h"

Mainwindow_controller::Mainwindow_controller(QObject* view)
{
  this->view = view;
}

Mainwindow_controller::~Mainwindow_controller()
{}

void Mainwindow_controller::parse_response(QString response)
{
  QVector<QString>tokens;
  QString token;
  QVector<QString>codes {"#2","//Z3JpZA==","//b2xvbG8="};
  QVector<QString>delimeters {R"("file":")","\""};
  tokens.clear();

  int index = 0;
  while((index = response.indexOf(delimeters.at(0))) != -1)
  {
      token = "";
      token = response.right(response.size() - index - 8);
      token = token.left(token.indexOf(delimeters.at(1)));
      tokens.push_back(token);
      response = response.right(response.size() - index - 8 - token.size() - 1);
  }

  Mainwindow_model::tokens.clear();
  for (auto& t: tokens)
  {
    //Remove Junk
    t = t.replace("\\/", "/");
    for (auto &code: codes)
    {
      t = t.replace(code, "");
    }

    //Decode Base64
    QByteArray b = t.toUtf8();
    t = QString(QByteArray::fromBase64(b));

    //Remove or's
    t = t.left(t.indexOf(" or "));

    Mainwindow_model::tokens.push_back(t);
  }
}

QVector<QString> Mainwindow_controller::getTokens()
{
  return Mainwindow_model::tokens;
}

void Mainwindow_controller::network_request(QString* url, QString film_id, QString seasonvar_full_url)
{
    Network_requester nr;
    Proxysettings_controller psc;

    //Get secret token from main page
    if (!seasonvar_full_url.startsWith("http://"))
    {
      seasonvar_full_url = "http://" + seasonvar_full_url;
    }
    if (!differentLink)
    {
      QString token = nr.getRequest(seasonvar_full_url, psc.getProxyHost(), psc.getProxyPort(), psc.getProxyType()).first;
      token = token.right(token.size() - token.indexOf("'secureMark': '") - 15);
      token = token.left(token.indexOf("'"));
      if (token != "")
      {
        QString newUrl = "http://seasonvar.ru/playls2/"+token+"/trans/";
        newUrl = newUrl + film_id + "/plist.txt";
        differentLink = true;
        network_request(&newUrl, film_id, seasonvar_full_url);
        return;
      }
      else
      {
        differentLink = false;
      }
    }
    //Send get request
    QPair response = nr.getRequest(*url, psc.getProxyHost(), psc.getProxyPort(), psc.getProxyType());
    QString answer = response.first;

    if (!response.second)
    {
        ((MainWindow*)view)->printForbiddenError();
        return;
    }

    if (answer == "[]")
    {
        ((MainWindow*)view)->printForbiddenError();
        return;
    }

    if (answer == "")
    {
        ((MainWindow*)view)->printForbiddenError();
        return;
    }

    parse_response(answer);

    QString textBrowserData = "";
    for (auto &token: Mainwindow_model::tokens)
    {
      textBrowserData.append("<a href=\""+token+"\">"+token+"</a><br><br>");
    }
    setTextBrowserData(textBrowserData);
}

bool Mainwindow_controller::checkUrl(QString url)
{
    QString search = "/serial-";

    if (url.indexOf(search) == -1)
      return false;

    //TODO add more complex check

    return true;
}

void Mainwindow_controller::getAllTheLinks() 
{
  setUrlText(((MainWindow*)view)->getUrlText());

  QString seasonvar_full_url = Mainwindow_model::urlText;
  QString film_id = "";
  QString url = "https://datalock.ru/playlist/145fb00f6ec315adbbe3db0ea331e94e/"; //Download template url

  if (!checkUrl(seasonvar_full_url))
  {
      ((MainWindow*)view)->printUrlError();
      return;
  }

  QString cut_url = seasonvar_full_url.right(seasonvar_full_url.size() - seasonvar_full_url.lastIndexOf("/serial-") - 8);
  cut_url = cut_url.left(cut_url.indexOf("-"));
  film_id = cut_url;

  if (film_id != "")
  {
      url= url + film_id + "/list.txt";
      network_request(&url,film_id,seasonvar_full_url);
  }
};

void Mainwindow_controller::copyLinksToClipboard() 
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(Mainwindow_model::plainTextBrowserData);
};

void Mainwindow_controller::downloadFromLinks() 
{

};

void Mainwindow_controller::generateScript() 
{

};

void Mainwindow_controller::linksDatabase() 
{

};

void Mainwindow_controller::configureProxy() 
{

};

void Mainwindow_controller::setUrlText(QString text) 
{
  Mainwindow_model::urlText = text;
  ((MainWindow*)view)->setUrlText(Mainwindow_model::urlText);
};

QString Mainwindow_controller::getUrlText() 
{
  return Mainwindow_model::urlText;
};

void Mainwindow_controller::setTextBrowserData(QString tbData)
{
  Mainwindow_model::textBrowserData = tbData;
  ((MainWindow*)view)->setTextBrowserData(Mainwindow_model::textBrowserData);
  Mainwindow_model::plainTextBrowserData = ((MainWindow*)view)->getPlainTextBrowserData();
};

QString Mainwindow_controller::getTextBrowserData()
{
  return Mainwindow_model::textBrowserData;
};
