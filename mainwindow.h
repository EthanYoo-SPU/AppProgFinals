#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTimeZone>

#include "httpmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setCurrentTime();
    void setWorldTime();

    void loadGalleryPic();

    void on_timeZoneBox_activated(int index);

    void processImage(QPixmap *);
    void on_imageDownloadButton_clicked();

    void processWeatherJson(QJsonObject *json);

    void on_imageDownloadButton_2_clicked();

    void on_weatherDownloadButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    void changeGreetingLabel();
    int timeZoneDifference = 0;
    QTimeZone pacificTime = QTimeZone(0);
    QTimeZone easternTime = QTimeZone(10800);
    QTimeZone koreaTime = QTimeZone(-28800);

    QTimer *galleryTimer;
    int galleryLocation = 0;

    HTTPManager *httpManager;

    QPixmap imageCloud;
    QPixmap imageRain;
    QPixmap imageClear;
    QPixmap imageSun;
    QPixmap imageThunder;
    QPixmap imageSnow;

    QPixmap gallery;

    //Weather website address: https://api.openweathermap.org/data/2.5/weather?zip=98119,us&units=imperial&appid=69e6e9566c4c639b1751b6969c6fe1b5

};
#endif // MAINWINDOW_H
