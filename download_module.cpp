#include "download_module.h"
#include "ui_download_module.h"
#include <iostream>
#include <QProgressBar>
#include <QLabel>


Download_module::Download_module(QWidget *parent) :
    QWidget (parent), ui(new Ui::Download_module)
{
    ui->setupUi(this);
}

void Download_module::setProgress(uint m_progress)
{
    progress=m_progress;
    QProgressBar *progressBar = ui->progressBar;
    progressBar->setValue(static_cast<int>(progress));
}

void Download_module::setLabel(const QString &label)
{
    QLabel *l = ui->labelName;
    l->setText(label);
}

int Download_module::getProgress()
{
    return static_cast<int>(progress);
}

void Download_module::on_pushButton_clicked()
{
    QString token_qstr = QString::fromUtf8(token.data(), static_cast<int>(token.size()));
    Download_manager *dm = new Download_manager(token_qstr);

    QThread* thread = new QThread;
    dm->moveToThread(thread);
    connect(thread, SIGNAL(started()), dm, SLOT(execute()));
    connect(dm, SIGNAL(finished()), thread, SLOT(quit()));
    connect(dm, SIGNAL(finished()), dm, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Download_module::setToken(std::string token)
{
    this->token=token;
}

std::string Download_module::getToken()
{
    return this->token;
}

Download_module::~Download_module()
{
    delete ui;
}
