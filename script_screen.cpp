#include "script_screen.h"
#include "ui_script_screen.h"

Script_screen::Script_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Script_screen)
{
    ui->setupUi(this);
    ui->lineEditOther->setEnabled(false);
    ui->plainTextEditScriptField->setPlainText(defaultScriptHeader+"\n");
    ui->pushButtonSaveScript->setEnabled(false);
    this->setGeometry( //Align window to the center of the screen
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()));
}

void Script_screen::setCalledFrom(QWidget *calledFrom)
{
    this->calledFrom = calledFrom;
}

void Script_screen::fillScript()
{
    ui->plainTextEditScriptField->setPlainText(defaultScriptHeader+"\n");
    for (auto &token:tokens)
    {
        ui->plainTextEditScriptField->setPlainText(ui->plainTextEditScriptField->toPlainText()+selectedDownloadProgramName+" "+token.c_str()+"\n");
    }
}

void Script_screen::setTokens(const std::vector<std::string> &tokens)
{
    this->tokens=tokens;
    for (auto &token:tokens)
    {
        ui->plainTextEditScriptField->setPlainText(ui->plainTextEditScriptField->toPlainText()+selectedDownloadProgramName.toStdString().c_str()+" "+token.c_str()+"\n");
    }
}

Script_screen::~Script_screen()
{
    delete ui;
}

void Script_screen::on_pushButtonBack_clicked()
{
    auto* mw = dynamic_cast<MainWindow*>(calledFrom);
    mw->show();
    hide();
}

void Script_screen::on_pushButtonSaveLocation_clicked()
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
    ui->lineEditSaveLocation->setText(directory+"/"+defaultScriptName);
}

void Script_screen::on_lineEditSaveLocation_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->pushButtonSaveScript->setEnabled(false);
    } else {
        ui->pushButtonSaveScript->setEnabled(true);
    }
}

void Script_screen::on_lineEditOther_textChanged(const QString &arg1)
{
    selectedDownloadProgramName = arg1;
    fillScript();
}

void Script_screen::on_radioButtonWget_clicked()
{
    ui->lineEditOther->setEnabled(false);
    selectedDownloadProgramName = "wget";
    fillScript();
}

void Script_screen::on_radioButtonAria2c_clicked()
{
    ui->lineEditOther->setEnabled(false);
    selectedDownloadProgramName = "aria2c";
    fillScript();
}

void Script_screen::on_radioButtonOther_clicked()
{
    ui->lineEditOther->setEnabled(true);
    selectedDownloadProgramName = ui->lineEditOther->text();
    fillScript();
}

void Script_screen::on_pushButtonSaveScript_clicked()
{
    QString filePath = ui->lineEditSaveLocation->text();
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream<<ui->plainTextEditScriptField->toPlainText();
        file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup | QFile::ReadOther);
        file.close();
    }
    on_pushButtonBack_clicked();
}
