#ifndef MAINWINDOW_MODEL_H
#define MAINWINDOW_MODEL_H
#pragma once

#include <QVector>
#include <QString>

class Mainwindow_model
{
public:
    inline static QString textBrowserData;
    inline static QString plainTextBrowserData;
    inline static QString urlText;
    inline static QVector<QString> tokens;
};

#endif // MAINWINDOW_MODEL_H
