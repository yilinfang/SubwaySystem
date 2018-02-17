#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "subwaystation.h"
#include <QDebug>
#include <QFont>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString fileName;
    SubwaySystem subwaySystem;
    Map map;
    QList<Station> points2draw;
    QList<Track> lines2draw;
    QList<Station> drawedPoints;
    QList<Track> drawedTracks;

    void ShowStation(Station* station);
    void ShowTrack(Track* track);
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
