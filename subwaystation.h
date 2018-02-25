#ifndef SUBWAYSTATION_H
#define SUBWAYSTATION_H

#include "QVector"
#include "QList"
#include "QString"
#include "QFile"
#include "QTextStream"
#include "QStringList"
#include "QDebug"
//#include "stdio.h"
//#include "stdlib.h"
//#include "malloc/malloc.h"

#define INF 10000
#define OK 1
#define FAIL 0
#define ERROR -1
#define INITIALSIZE 50
#define STEPSIZE 10

using namespace std;

typedef int state;
typedef struct Station Station;
typedef struct StationNode StationNode;
typedef struct P2StationNode P2StationNode;
typedef struct Line Line;
typedef struct LineNode LineNode;
typedef struct P2LineNode P2LineNode;
typedef struct Track Track;
typedef struct TrackNode TrackNode;
typedef struct P2TrackNode P2TrackNode;
typedef struct Edge Edge;
typedef struct SubwaySystem SubwaySystem;
typedef struct Map Map;
typedef StationNode* StationList;
typedef P2StationNode* P2StationList;
typedef TrackNode* TrackList;
typedef P2TrackNode* P2TrackList;
typedef LineNode* LineList;
typedef P2LineNode* P2LineList;
typedef struct EdgeList EdgeList;

//struct P2LineList{
//    int size;
//    int length;
//    Line** arr;
//    P2LineList()
//    {
//        size = length = 0;
//        arr = NULL;
//    }
//};

struct StationNode
{
    Station station;
    StationNode* next;
};

struct P2StationNode
{
    Station* p2Station;
    P2StationNode* next;
};

struct TrackNode
{
    Track track;
    TrackNode* next;
};

struct P2TrackNode
{
    Track* p2Track;
    P2TrackNode* next;
};
//struct P2TrackList{
//    int size;
//    int length;
//    Track** arr;
//    P2TrackList()
//    {
//        size = length = 0;
//        arr = NULL;
//    }
//};

struct LineNode
{
    Line line;
    LineNode* next;
};

struct P2LineNode
{
    Line* p2Line;
    P2LineNode* next;
};

struct Station
{
    QString name;
    QPoint pos;
    //QList<Line*> p2transferLines;
    P2LineList transferLines;
    //QList<Track*> p2Tracks;
    P2TrackList trackList;
};

//struct StationList
//{
//    int size;
//    int length;
//    Station* arr;
//    StationList()
//    {
//        size = length = 0;
//        arr = NULL;
//    }
//};

//struct P2StationList
//{
//    int size;
//    int length;
//    Station** arr;
//    P2StationList()
//    {
//        size = length = 0;
//        arr = NULL;
//    }
//};

struct Line{
    QString name;
    //QList<Station*> p2inLineStations;
    P2StationList inLineStations;
    //QList<Track*> p2inLineTracks;
    P2TrackList inLineTrack;
};

//struct LineList{
//    int size;
//    int length;
//    Line* arr;
//    LineList()
//    {
//        size = length = 0;
//        arr = NULL;
//    }
//};




struct Track{
    //long i,j;
    Station *s1, *s2;
    int weight;
    //QList<Line*> p2inTrakLines;
    P2LineList LineList;
};

//struct TrackList{
//    int size;
//    int length;
//    Track* arr;
//    TrackList()
//    {
//        size = length = 0;
//        arr = NULL;
//    }
//};


struct SubwaySystem
{
    int lineNum;
    int stationNum;
    int edgeNum;
    //QList<Line> lineTable;
    LineList lineList;
    //QList<Station> stationTable;
    StationList stationList;
    //QList<Track> tracks;
    TrackList trackList;
};

struct Edge
{
    int i,j;
    int weight;
};

struct EdgeList
{
    int size;
    int length;
    Edge* arr;
    EdgeList()
    {
        size = length = 0;
        arr = NULL;
    }
};

struct Map
{
    int vertexNum;
    int edgeNum;
    int** matrix;
    int** dist;
    int** path;
    QVector<Station*> vertexTable;
    QList<Edge> edges;
};

state InitSubwaySystem(QString fileName, SubwaySystem &subwaySystem, QString &outputBufa);
Station* FindOrNewStation(QString stationName, Line* p2line,SubwaySystem &subwaySystem);
Station* FindStationByName(QString stationName, SubwaySystem &subwaySystem);
Track* FindOrNewTrack(Station* station1, Station* station2, int weight,Line* p2line, SubwaySystem &subwaySystem);
state SaveSubwaySystem(QString fileName, SubwaySystem &subwaySystem, QString &outputBufa);

state StationListInit(StationList &stationList);
state StationListAppend(StationList &stationList, Station station);
state StationListAppendList(StationList &stationList1, StationList stationList2);
state StationListEmpty(StationList &stationList);
state StationListDestroy(StationList &stationList);

state P2StationListInit(P2StationList &p2StationList);
state P2StationListAppendList(P2StationList &p2StationList1, P2StationList p2StationList2);
state P2StationListAppend(P2StationList &P2StationList, Station* p2Station);
state P2StationListEmpty(P2StationList &P2StationList);
state P2StationListDestroy(P2StationList &P2StationList);

state LineListInit(LineList &lineList);
state LineListAppend(LineList &lineList, Line &line);
state LineListAppendList(LineList &lineList1, LineList lineList2);
state LineListEmpty(LineList &lineList);
state LineListDestroy(LineList &lineList);

state P2LineListInit (P2LineList  &p2LineList);
state P2LineListAppend (P2LineList  &p2LineList, Line* line);
state P2LineListAppendList (P2LineList &p2LineList1, P2LineList p2LineList2);
state P2LineListEmpty (P2LineList  &p2LineList);
state P2LineListDestroy (P2LineList  &p2LineList);

state TrackListInit(TrackList &trackList);
state TrackListAppend(TrackList &trackList, Track track);
state TrackListAppendList(TrackList &trackList1, TrackList trackList2);
state TrackListEmpty(TrackList &trackList);
state TrackListDestroy(TrackList &trackList);

state P2TrackListInit(P2TrackList &p2TrackList);
state P2TrackListAppendList(P2TrackList &p2TrackList1, P2TrackList p2TrackList2);
state P2TrackListAppend(P2TrackList &P2TrackList, Track* p2Track);
state P2TrackListEmpty(P2TrackList &P2TrackList);
state P2TrackListDestroy(P2TrackList &P2TrackList);

state EdgeListInit(EdgeList &edgeList);
state EdgeListAppend(EdgeList &edgeList, Edge edge);
state EdgeListAppendList(EdgeList &edgeList1, EdgeList edgeList2);
state EdgeListEmpty(EdgeList &edgeList);
state EdgeListDestroy(EdgeList &edgeList);

#endif // SUBWAYSTATION_H
