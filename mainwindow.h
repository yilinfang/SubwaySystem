#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QPainter"
#include "subwaystation.h"
#include "QFont"
#include "QColor"
#include "QDebug"

typedef struct Point Point;
typedef struct DLine DLine;
struct Point{
    Station* station;
    QColor color;
};
struct DLine
{
    Track* track;
    QColor color;
};

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
    QList<Point> points2draw;
    QList<DLine> lines2draw;
    QList<Point> drawedPoints;
    QList<DLine> drawedTracks;

    void ShowStation(Station* station);
    void ShowTrack(Track* track);
    Station* IsStationAround(QPoint pos);
    Track* IsTrackAround(QPoint pos);
    QList<Point> SetPointColor(QList<Station> &stations, QColor color);
    QList<DLine> SetLineColor(QList<Track> &tracks, QColor color);
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
