#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QWidget>
#include <QMessageBox>
#include <QClipboard>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <download_screen.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    std::vector<std::string> tokens; //For parsed urls
    Ui::MainWindow *ui;
	enum UrlType {fullUrl, justId, wrongAddress};
	void printNetError();
	void printIdError();
	MainWindow::UrlType checkUrl(std::string url);
    void replaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);
    static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
    int curl_request(std::string* url, std::string* response);
    void parse_response(std::string* response, std::vector<std::string>* tokens);
    void print_tokens();
	void decode(std::string* token);
	void removeOrs(std::string* token);
};

#endif // MAINWINDOW_H
