#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QPainter"
#include "subwaysystem.h"
#include "QFont"
#include "QColor"
#include "QDebug"

#define MAX_ADMIN_NUM 5

typedef struct Point Point;
typedef Point PointNode;
typedef struct DLine DLine;
typedef DLine DLineNode;
typedef PointNode*  PointList;
typedef DLineNode* DLineList;
typedef struct Admin Admin;

struct Admin{
    QString username;
    QString password;
};

struct Point{
    Station* station;
    QColor color;
    Point* next;
};

struct DLine
{
    Track* track;
    QColor color;
    DLine* next;
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
    Admin admins[MAX_ADMIN_NUM];
    int adminsNum;
    QString fileName;
    SubwaySystem subwaySystem;
    Map map;
    PointList points2Draw;
    DLineList lines2Draw;
    PointList drawedPoints;
    DLineList drawedTracks;
    P2StationList chosenStations;
    P2TrackList chosenTracks;
    P2LineList chosenLines;
    void ShowStation(Station* station, QPoint pos);
    void ShowTrack(Track* track, QPoint pos);
    Station* IsStationAround(QPoint pos);
    Track* IsTrackAround(QPoint pos);
    QString dirPos;
    int isFirstStart;
    void SetPointColor(StationList &stations, QColor color, PointList &list);
    void SetLineColor(TrackList &tracks, QColor color, DLineList &list);
    // QWidget interface
    void ShowChosenStation();
    void ShowChosenTracks();
    void ShowChosenLines();
    void LoadAdmins();
    void SaveAdmins();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_saveFile_clicked();
    void on_loadFile_clicked();
    void on_updateDisplay_clicked();
    void on_showLineStation_clicked();
    void on_getMinestTimePath_clicked();
    void on_editMap_clicked();
    void on_confirmMap_clicked();
    void on_cancelMap_clicked();
    void on_editAdmins_clicked();
    void on_cancelAdmins_clicked();
    void on_confirmAdmins_clicked();
    void on_showAllLineStation_clicked();
    void on_login_clicked();
    void on_editStationName_clicked();
    void on_editStationPos_clicked();
    void on_editLineName_clicked();
    void on_editTrackWeight_clicked();
    void on_getMinestTransferPath_clicked();
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
