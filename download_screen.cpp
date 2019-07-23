#include "download_screen.h"
#include "ui_download_screen.h"
#include <QScrollArea>
#include <QFileDialog>

Download_screen::Download_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Download_screen)
{
    ui->setupUi(this);
    this->setGeometry( //Align window to the center of the screen
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()));
    vl = new QVBoxLayout();
    QScrollArea *sa = ui->scrollArea;
    directoryInfo = ui->labelSetSaveDirectory;

    //Container for download modules
    auto* container = new QWidget;
    container->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    container->setLayout(vl);
    sa->setWidget(container);
    vl->setSizeConstraint(QLayout::SetMinAndMaxSize);

    //Spacer to move first item on top of the page
    qsi = new QSpacerItem(0,400,QSizePolicy::Minimum,QSizePolicy::Maximum);

    if(directory!="")
    {
        //Sets directory for file download
        directoryInfo->setText(directory);
    }
}

Download_screen::~Download_screen()
{
    delete ui;
}

void Download_screen::setTokens(const std::vector<std::string> &tokens)
{
    this->tokens=tokens;

    //Fills vector of modules
    if (!tokens.empty())
    {
        for (uint i=0; i<tokens.size(); i++)
        {
            auto *dm = new Download_module;
            dm->setProgress(0);
            dm->setLabel("Episode "+QString::number(i));
            dm->setToken(tokens.at(i));
            modules_list.push_back(dm);
        }
    }

    //Adds modules from vector to layout
    if (!modules_list.empty())
    {
        for (auto &i:modules_list)
        {
            vl->addWidget(i);
        }
        vl->addItem(qsi);
    }
}

void Download_screen::setCalledFrom(QWidget *calledFrom)
{
    this->calledFrom = calledFrom;
}

void Download_screen::on_pushButtonChooseDirectory_clicked()
{
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    int result = fd->exec();
    if (result)
    {
        directory=fd->selectedFiles()[0];
        directoryInfo->setText(directory);
    }
}

void Download_screen::on_pushButtonGoBack_clicked()
{
    auto* mw = dynamic_cast<MainWindow*>(calledFrom);
    mw->show();
    hide();
}
