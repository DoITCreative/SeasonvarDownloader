#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H
#pragma once

#include <QtCore>
#include <QtNetwork>
#include <QNetworkReply>

class Download_manager: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;
    public:
        Download_manager(QString url_str);
        void doDownload(const QUrl &url);
        static QString saveFileName(const QUrl &url);
        bool saveToDisk(const QString &filename, QIODevice *data);
        static bool isHttpRedirect(QNetworkReply *reply);
    signals:
        void finished();
    public slots:
        void execute();
        void downloadFinished(QNetworkReply *reply);
        void sslErrors(const QList<QSslError> &errors);
    private:
        QString url_str;
        QString download_path;
};
#endif // DOWNLOAD_MANAGER_H
