#include "script_screen.h"
#include "ui_script_screen.h"

Script_screen::Script_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Script_screen)
{
    ui->setupUi(this);
    ui->otherLineEdit->setEnabled(false);
    ui->plainTextEdit->setPlainText(defaultScriptHeader+"\n");
    ui->saveScriptPushButton->setEnabled(false);
}

void Script_screen::setCalledFrom(QWidget *calledFrom)
{
    this->calledFrom = calledFrom;
}

void Script_screen::fillScript()
{
    ui->plainTextEdit->setPlainText(defaultScriptHeader+"\n");
    for (auto &token:tokens)
    {
        ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText()+selectedDownloadProgramName+" "+token.c_str()+"\n");
    }
}

void Script_screen::setTokens(const std::vector<std::string> &tokens)
{
    this->tokens=tokens;
    for (auto &token:tokens)
    {
        ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText()+selectedDownloadProgramName.toStdString().c_str()+" "+token.c_str()+"\n");
    }
}

Script_screen::~Script_screen()
{
    delete ui;
}

void Script_screen::on_backButton_clicked()
{
    auto* mw = dynamic_cast<MainWindow*>(calledFrom);
    mw->show();
    hide();
}

void Script_screen::on_saveLocationPushButton_clicked()
{
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    int result = fd->exec();
    if (result)
    {
        directory=fd->selectedFiles()[0];
    }
    ui->saveLocationLineEdit->setText(directory+"/"+defaultScriptName);
}

void Script_screen::on_saveLocationLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->saveScriptPushButton->setEnabled(false);
    } else {
        ui->saveScriptPushButton->setEnabled(true);
    }
}

void Script_screen::on_otherLineEdit_textChanged(const QString &arg1)
{
    selectedDownloadProgramName = arg1;
    fillScript();
}

void Script_screen::on_wgetRadioButton_clicked()
{
    ui->otherLineEdit->setEnabled(false);
    selectedDownloadProgramName = "wget";
    fillScript();
}

void Script_screen::on_aria2cRadioButton_clicked()
{
    ui->otherLineEdit->setEnabled(false);
    selectedDownloadProgramName = "aria2c";
    fillScript();
}

void Script_screen::on_otherRadioButton_clicked()
{
    ui->otherLineEdit->setEnabled(true);
    selectedDownloadProgramName = ui->otherLineEdit->text();
    fillScript();
}

void Script_screen::on_saveScriptPushButton_clicked()
{
    QString filePath = ui->saveLocationLineEdit->text();
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream<<ui->plainTextEdit->toPlainText();
        file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::ReadOther);
        file.close();
    }
    on_backButton_clicked();
}
