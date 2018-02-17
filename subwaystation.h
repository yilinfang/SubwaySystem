#ifndef SUBWAYSTATION_H
#define SUBWAYSTATION_H

#include <QVector>
#include <string>
#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#define INF 10000
#define OK 1
#define FAIL 0
#define ERROR -1

using namespace std;

typedef int state;
typedef struct Station Station;
typedef struct Line Line;
typedef struct Track Track;
typedef struct Edge Edge;
typedef struct SubwaySystem SubwaySystem;
typedef struct Map Map;

struct Station
{
    QString name;
    QPoint pos;
    QList<Line*> p2transferLines;
    QList<Track*> p2Tracks;
};

struct Line{
    QString name;
    QList<Station*> p2inLineStations;
    QList<Track*> p2inLineTracks;
};

struct Track{
    //long i,j;
    Station *s1, *s2;
    int weight;
    QList<Line*> p2inTrakLines;
};

struct SubwaySystem
{
    int lineNum;
    int stationNum;
    int edgeNum;
    QList<Line> lineTable;
    QList<Station> stationTable;
    QList<Track> tracks;
};

struct Edge
{
    int i,j;
    int weight;
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
#endif // SUBWAYSTATION_H
