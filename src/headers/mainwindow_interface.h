#ifndef MAINWINDOW_INTERFACE_H
#define MAINWINDOW_INTERFACE_H
#pragma once

#include <QString>
#include <QVector>

class Mainwindow_interface
{
public:
    virtual ~Mainwindow_interface() {};
    virtual void getAllTheLinks() = 0;
    virtual QVector<QString> getTokens() = 0;
    virtual void copyLinksToClipboard() = 0;
    virtual void downloadFromLinks() = 0;
    virtual void generateScript() = 0;
    virtual void linksDatabase() = 0;
    virtual void configureProxy() = 0;

    virtual void setUrlText(QString text) = 0;
    virtual QString getUrlText() = 0;

    virtual void setTextBrowserData(QString tbData) = 0;
    virtual QString getTextBrowserData() = 0;
};
#endif // MAINWINDOW_INTERFACE_H
