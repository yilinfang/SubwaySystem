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
typedef struct PointList PointList;
typedef struct DLineList DLineList;

struct Point{
    Station* station;
    QColor color;
};

struct DLine
{
    Track* track;
    QColor color;
};

struct PointList{
    int size;
    int length;
    Point* arr;
    PointList()
    {
        size = length = 0;
        arr = NULL;
    }
};


struct DLineList{
    int size;
    int length;
    DLine* arr;
    DLineList()
    {
        size = length = 0;
        arr = NULL;
    }
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
    PointList points2Draw;
    QList<DLine> lines2draw;
    DLineList lines2Draw;
    //QList<Point> drawedPoints;
    PointList drawedPoints;
    //QList<DLine> drawedTracks;
    //PointList drawedTracks;
    DLineList drawedTracks;
    //QList<Station*> chosenStations;
    P2StationList chosenStations;
    //QList<Track*> chosenTracks;
    P2TrackList chosenTracks;
    void ShowStation(Station* station);
    void ShowTrack(Track* track);
    Station* IsStationAround(QPoint pos);
    Track* IsTrackAround(QPoint pos);
    //QList<Point> SetPointColor(QList<Station> &stations, QColor color);
    void SetPointColor(StationList &stations, QColor color, PointList &list);
    //QList<DLine> SetLineColor(QList<Track> &tracks, QColor color);
    void SetLineColor(TrackList &tracks, QColor color, DLineList &list);
    // QWidget interface
    void ShowChosenStation();
    void ShowChosenTracks();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_saveFile_clicked();
    void on_loadFile_clicked();
};

state PointListInit(PointList &pointList);
state PointListAppend(PointList &pointList, Point point);
state PointListAppendList(PointList &pointList1, PointList pointList2);
state PointListEmpty(PointList &pointList);
state PointListDestroy(PointList &pointList);

state DLineListInit(DLineList &dLineList);
state DLineListAppend(DLineList &dLineList, DLine dLine);
state DLineListAppendList(DLineList &dLineList1, DLineList dLineList2);
state DLineListEmpty(DLineList &dLineList);
state DLineListDestroy(DLineList &dLineList);


#endif // MAINWINDOW_H
