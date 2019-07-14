#include "script_screen.h"
#include "ui_script_screen.h"

Script_screen::Script_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Script_screen)
{
    ui->setupUi(this);
    ui->otherLineEdit->setEnabled(false);
}

void Script_screen::setCalledFrom(QWidget *calledFrom)
{
    this->calledFrom = calledFrom;
}

void Script_screen::setTokens(const std::vector<std::string> &tokens)
{
    this->tokens=tokens;
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

}

void Script_screen::on_saveLocationLineEdit_textChanged(const QString &arg1)
{

}

void Script_screen::on_otherLineEdit_textChanged(const QString &arg1)
{

}

void Script_screen::on_wgetRadioButton_clicked()
{
    ui->otherLineEdit->setEnabled(false);
}

void Script_screen::on_aria2cRadioButton_clicked()
{
    ui->otherLineEdit->setEnabled(false);
}

void Script_screen::on_otherRadioButton_clicked()
{
    ui->otherLineEdit->setEnabled(true);
}

void Script_screen::on_saveScriptPushButton_clicked()
{

}
