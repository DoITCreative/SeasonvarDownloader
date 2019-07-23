#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelCloud->setVisible(false); //Make parrot invisible
    ui->textBrowser->setEnabled(false);
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
        network_request(&url,&response);
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
int MainWindow::network_request(std::string* url, std::string* response)
{
    manager = new QNetworkAccessManager();
    connect(manager,&QNetworkAccessManager::finished,
        this, [&](QNetworkReply *reply)
        {
            if (reply->error()) 
            {
                printNetError();
                return 1;
            }
            std::string answer = reply->readAll().toStdString();
            response = &answer;
            parse_response(response,&tokens);
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
    request.setUrl(QUrl(QString::fromStdString(*url)));
    manager->get(request);
    return 0;
}

/*
 * Parses response to vector of urls
 */
void MainWindow::parse_response(std::string* response, std::vector<std::string>* tokens)
{
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
        replaceStringInPlace(m_token, "//Z3JpZA==", "");

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
    BIO *b64, *bmem;
    char *buffer = static_cast<char *>(malloc((*token).length()));
    memset(buffer,0,(*token).length());
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64,BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new_mem_buf((*token).c_str(),static_cast<int>((*token).length()));
    bmem = BIO_push(b64, bmem);
    BIO_read(bmem,buffer,static_cast<int>((*token).length()));
    BIO_free_all(bmem);
    std::string output(buffer);
    *token = output;
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
