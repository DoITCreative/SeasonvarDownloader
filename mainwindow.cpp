#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>

bool MainWindow::use_proxy = false;
QString MainWindow::proxy_host = "";
quint16 MainWindow::proxy_port = 0;
QString MainWindow::proxy_type = "";

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelCloud->setVisible(false); //Make parrot invisible
    ui->textBrowser->setEnabled(false);
    ui->pushButtonLinksDatabase->setEnabled(false); //Hide links button
    ui->pushButtonCopyLinksToClipboard->setEnabled(false); //Copy links to clipboard
    ui->pushButtonDownloadFromLinks->setEnabled(false); //Download screen
    ui->pushButtonGenerateScript->setEnabled(false); //Generate download script
    ui->loadingAnimation->setVisible(false); //Hide loading animation
    loadingAnimation = new QMovie(":/loading-animation.gif"); //Initialize loading animation
    ui->loadingAnimation->setMovie(loadingAnimation);
    this->setGeometry( //Align window to the center of the screen
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()));
}

MainWindow::~MainWindow()
{
    delete manager;
    delete loadingAnimation;
    delete ui;
}

/*
 * Checks if url is valid
 */
MainWindow::UrlType MainWindow::checkUrl(std::string url)
{
    std::string search = "/serial-";
    if (url.find(search)==std::string::npos)
    {
        bool input_correct=true;
        for(char i:url)
        {
            if(static_cast<int>(i-48) >= 10)
            {
                input_correct=false;
            }
        }
        int url_int = 0;
        if (!url.empty()&&input_correct)
        {
            url_int=std::stoi(url);
        } else
        {
            input_correct=false;
        }
        if ((!input_correct)||(url_int<=0))
        {
            printIdError();
            return MainWindow::UrlType::wrongAddress;
        }
        return MainWindow::UrlType::justId;
    }
    return MainWindow::UrlType::fullUrl;
}


void MainWindow::printIdError()
{
    QMessageBox messageBox;
    QMessageBox::critical(nullptr,"Error","Id is wrong!\nPlease specify correct one.");
    messageBox.setFixedSize(500,200);
}

void MainWindow::printNetError()
{
    QMessageBox messageBox;
    QMessageBox::critical(nullptr,"Error","Network error ruins it!");
    messageBox.setFixedSize(500,200);
}

void MainWindow::printForbiddenError()
{
    QMessageBox messageBox;
    QMessageBox::critical(nullptr,"Error","Link access is forbidden by provider. Use proxy settings to bypass.");
    messageBox.setFixedSize(500,200);
}

/*
 * Shows loading animation
 */
void MainWindow::startLoadingAnimation()
{
    ui->loadingAnimation->setVisible(true);
    loadingAnimation->start();
}

/*
 * Hides loading animation
 */
void MainWindow::stopLoadingAnimation()
{
    ui->loadingAnimation->setVisible(false);
    loadingAnimation->stop();
}

/*
 * Connect button pressed
 */
void MainWindow::on_pushButtonGetAllTheLinks_clicked()
{
    startLoadingAnimation();
    std::string seasonvar_full_url=ui->lineEditSeasonvarURL->text().toStdString();
    std::string film_id;
    std::string response;
    std::string url = "https://datalock.ru/playlist/145fb00f6ec315adbbe3db0ea331e94e/"; //Download template url

    switch (checkUrl(seasonvar_full_url))
    {
        case MainWindow::UrlType::justId:
        {
            film_id=seasonvar_full_url;
            break;
        }
        case MainWindow::UrlType::fullUrl:
        {
            seasonvar_full_url=seasonvar_full_url.substr(seasonvar_full_url.find("/serial-")+8);
            seasonvar_full_url=seasonvar_full_url.substr(0,seasonvar_full_url.find("-"));
            if (checkUrl(seasonvar_full_url)==MainWindow::UrlType::justId)
            {
                film_id=seasonvar_full_url;
            }
            else
            {
                film_id="";
            }
            break;
        }
        case MainWindow::UrlType::wrongAddress:
        {
            film_id="";
            break;
        }
    }

    if (!film_id.empty())
    {
        url=url+film_id+"/list.txt";
        network_request(&url,&response,film_id);
    }
    stopLoadingAnimation();
}

/*
 * Search and replace function
 */
void MainWindow::replaceStringInPlace(std::string& subject,
                                      const std::string& search,
                                      const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

/*
 * Replacement for standart output for libcurl - disables response output to stdout
 */
size_t MainWindow::writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

/*
 * Sends GET request to seasonvar servers to get list of links
 */
int MainWindow::network_request(std::string* url, std::string* response, std::string film_id)
{
    int linkType = 0;
    manager = new QNetworkAccessManager();
    connect(manager,&QNetworkAccessManager::finished,
        this, [&, film_id](QNetworkReply *reply)
        {
            if (reply->error()) 
            {
                printNetError();
                return 1;
            }
            std::string answer = reply->readAll().toStdString();
            if (answer == "[]")
            {
                linkType++;
                //Different link here, use another list
                std::string newUrl = "http://seasonvar.ru/playls2/a9f6bc3e39887f12e261c5105a6ae0c8/trans/";
                newUrl = newUrl+film_id+"/plist.txt";
                network_request(&newUrl, response, film_id);
            }
            if (answer == "")
            {
                printForbiddenError();
                return 1;
            }
            response = &answer;
            parse_response(response, linkType, &tokens);
            print_tokens();
            QTextCursor cursor = ui->textBrowser->textCursor();
            cursor.setPosition(0);
            ui->textBrowser->setTextCursor(cursor);
            ui->pushButtonDownloadFromLinks->setEnabled(true);
            ui->pushButtonCopyLinksToClipboard->setEnabled(true);
            ui->pushButtonGenerateScript->setEnabled(true);
            return 0;
        }
    );
    if (MainWindow::use_proxy)
    {
        QNetworkProxy proxy;
        if (MainWindow::proxy_type == "socks5") proxy.setType(QNetworkProxy::Socks5Proxy);
        else if (MainWindow::proxy_type == "http") proxy.setType(QNetworkProxy::HttpProxy);
        else proxy.setType(QNetworkProxy::NoProxy);
        proxy.setHostName(MainWindow::proxy_host);
        proxy.setPort(MainWindow::proxy_port);
        manager->setProxy(proxy);
    }
    request.setUrl(QUrl(QString::fromStdString(*url)));
    manager->get(request);
    return 0;
}

/*
 * Parses response to vector of urls
 */
void MainWindow::parse_response(std::string* response, int code_id, std::vector<std::string>* tokens)
{
    std::string code;
    switch (code_id) {
    case 0:
        code = "//Z3JpZA==";
        break;
    case 1:
        code = "//b2xvbG8=";
        break;
    default:
        code = "//Z3JpZA==";
        break;
    }
    tokens->clear();
    std::string delimiter = R"("file":")";
    std::string delimiter2 = "\"";
    std::string token;
    std::string token_parsed;
    while (response->find(delimiter) != std::string::npos)
    {
        token = response->substr(response->find(delimiter)+
            delimiter.length(),response->length());
        token_parsed = token.substr(0,token.find(delimiter2));
        tokens->push_back(token_parsed);
        *response=token.substr(token_parsed.length(),token.length());
    }
    for (auto &m_token:*tokens)
    {
        replaceStringInPlace(m_token, "\\/", "/");

        //Removes unnecessary lines from base64 encoded string
        replaceStringInPlace(m_token, "#2", "");
        replaceStringInPlace(m_token, code, "");

        //Decodes from base64
        decode(&m_token);

        //Remove or's
        removeOrs(&m_token);
    }
}

/*
 * Decodes from base64
 */
void MainWindow::decode(std::string* token)
{
    std::string decodedToken = "";
    QByteArray b = "";
    b.append(QString::fromStdString(*token));
    decodedToken = QByteArray::fromBase64(b).toStdString();
    *token = decodedToken;
}

/*
 * Removes links with .m3u8, that are specified with or keyword
 */
void MainWindow::removeOrs(std::string* token)
{
    std::string delimiter = " or ";
    std::string teststr;
    if (token->find(delimiter) != std::string::npos)
    {
        teststr = token->substr(token->find(delimiter)+delimiter.length(),token->length());
        *token = teststr;
    }
}

/*
 * Prints all tokens
 */
void MainWindow::print_tokens()
{
    ui->textBrowser->clear();
    QString str;
    for (const auto &m_token:tokens)
    {
        str = str.fromStdString("<a href=\""+m_token+"\">"+m_token+"</a>"+"<br><br>");
        ui->textBrowser->append(str);
    }
    ui->labelCloud->setVisible(true);
    ui->textBrowser->setEnabled(true);
}

/*
 * Copy to clipboard
 */
void MainWindow::on_pushButtonCopyLinksToClipboard_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    QTextBrowser* tb = ui->textBrowser;
    clipboard->setText(tb->toPlainText());
}

void MainWindow::on_pushButtonDownloadFromLinks_clicked()
{
    Download_screen *downloadScreen = new Download_screen();
    downloadScreen->setCalledFrom(this);
    downloadScreen->setTokens(tokens);
    downloadScreen->show();
    hide();
}

void MainWindow::on_pushButtonGenerateScript_clicked()
{
    Script_screen *scriptScreen = new Script_screen();
    scriptScreen->setCalledFrom(this);
    scriptScreen->setTokens(tokens);
    scriptScreen->show();
    hide();
}

void MainWindow::on_pushButtonConfigureProxy_clicked()
{
    ProxySettings *proxySettings = new ProxySettings();
    proxySettings->show();
}
