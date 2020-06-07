#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

class HTTPManager : public QObject
{
    Q_OBJECT
public:
    explicit HTTPManager(QObject *parent = nullptr);
    ~HTTPManager();
    void sendImageRequest();

signals:
    void ImageReady(QPixmap *image);


private slots:
    void ImageDownloadedHandler(QNetworkReply *reply);

private:
    QNetworkAccessManager *imageDownloadManager;
    QByteArray downloadedData;
};

#endif // HTTPMANAGER_H
