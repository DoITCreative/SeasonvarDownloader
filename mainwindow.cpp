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
    ui->label_3->setVisible(false); //Make parrot invisible
    ui->textBrowser->setEnabled(false);
    ui->pushButton_2->setEnabled(false); //Copy links to clipboard
    ui->pushButton_3->setEnabled(false); //Download screen
}

MainWindow::~MainWindow()
{
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
 *	Connect button pressed
 */
void MainWindow::on_pushButton_clicked()
{
    std::string seasonvar_full_url=ui->lineEdit->text().toStdString();
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
        if(curl_request(&url,&response)==1)
        {
            printNetError();
        }
        if (response.empty())
        {
            printNetError();
        }
        parse_response(&response,&tokens);
        print_tokens();
        QTextCursor cursor = ui->textBrowser->textCursor();
        cursor.setPosition(0);
        ui->textBrowser->setTextCursor(cursor);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }
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
 * Makes GET request with curllib on given url
 */
int MainWindow::curl_request(std::string* url, std::string* response)
{
    CURL *curl = curl_easy_init();
    if(curl)
    {
        CURLcode res;
        curl_easy_setopt(curl,CURLOPT_URL,url->c_str()); //specify url
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,1L); //disable progressbar
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writeFunction); //disable console output
        curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L); //0 disable messages
        curl_easy_setopt(curl,CURLOPT_MAXREDIRS,3L); //Max redirects allowed
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,response); //Save response to variable
        res = curl_easy_perform(curl); //Do request
        if (res==CURLE_OK)
        {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            if (response_code!=200)
            {
                return 1;
            }
        }
        curl_easy_cleanup(curl); //Cleanup
        curl=nullptr;
        return 0;
    }
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
    ui->label_3->setVisible(true);
    ui->textBrowser->setEnabled(true);
}

/*
 * Copy to clipboard
 */
void MainWindow::on_pushButton_2_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    QTextBrowser* tb = ui->textBrowser;
    clipboard->setText(tb->toPlainText());
}

void MainWindow::on_pushButton_3_clicked()
{
    auto *ds = new Download_screen();
    ds->setCalledFrom(this);
    ds->setTokens(tokens);
    ds->show();
    hide();
}
