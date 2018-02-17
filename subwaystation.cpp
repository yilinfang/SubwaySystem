#include "subwaystation.h"

state InitSubwaySystem(QString fileName, SubwaySystem &subwaySystem, QString &outputBufa)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        outputBufa = "文件打开失败！";
        return ERROR;
    }
    QTextStream stream(&file);
    QString line;
    Line tempLine;
    Line* p2line;
    while(!stream.atEnd())
    {
        line = stream.readLine();
        qDebug() << line;
        if(line.startsWith("#", Qt::CaseSensitive)) //读取到线路信息
        {
            subwaySystem.lineTable.append(tempLine);
            p2line = &subwaySystem.lineTable.last();
            p2line->name = line;
        }
        else    //读取到站点以及路径信息
        {
            QStringList tempList = line.split(',', QString::SkipEmptyParts);
            QList<QString>::iterator iter = tempList.begin();
            QString name1 = *iter;
            qDebug() << name1;
            Station *ptr1, *ptr2;
            ptr1 = ptr2 = NULL;
            ptr1 = FindOrNewStation(name1, p2line, subwaySystem);
            ++iter;
            bool flag;
            bool* ptr = &flag;
            int x,y;
            x = iter->toInt(ptr, 10);
            qDebug() << x;
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                return ERROR;
            }
            ++iter;
            y = iter->toInt(ptr, 10);
            qDebug() << y;
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                return ERROR;
            }
            ptr1->pos.setX(x);
            ptr1->pos.setY(y);
            ++iter;
            int weight = iter->toInt(ptr, 10);
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                return ERROR;
            }
            qDebug() << weight;
            ++iter;
            QString name2 = *iter;
            qDebug() << name2;
            ptr2 = FindOrNewStation(name2, p2line, subwaySystem);
            ++iter;
            x = iter->toInt(ptr, 10);
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                return ERROR;
            }
            qDebug() << x;
            ++iter;
            y = iter->toInt(ptr, 10);
            qDebug() << y;
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                return ERROR;
            }
            ptr2->pos.setX(x);
            ptr2->pos.setY(y);
            FindOrNewTrack(ptr1, ptr2, weight, p2line, subwaySystem);
        }
    }
    return OK;
}


Station* FindOrNewStation(QString stationName, Line *p2line, SubwaySystem &subwaySystem)
{
    Station* ptr = FindStationByName(stationName, subwaySystem);
    if(!ptr) //该站点第一次出现
    {
        Station temp;
        temp.name = stationName;
        temp.p2transferLines.append(p2line);
        subwaySystem.stationTable.append(temp);
        ptr = &subwaySystem.stationTable.last();
        p2line->p2inLineStations.append(ptr);
    }
    else //该站点已经出现过
    {
        QList<Line*>::iterator iter;
        int signal = 0;
        for(iter = ptr->p2transferLines.begin(); iter != ptr->p2transferLines.end(); iter++)
        {
            if(*iter == p2line)
            {
                signal = 1;
                break;
            }
        }
        if(!signal) //该站点出现在一条新线路中
        {
            ptr->p2transferLines.append(p2line);
            p2line->p2inLineStations.append(ptr);
        }
    }
    return ptr;
}

Station* FindStationByName(QString stationName, SubwaySystem &subwaySystem)
{
    QList<Station>::iterator iter;
    for(iter = subwaySystem.stationTable.begin(); iter != subwaySystem.stationTable.end(); iter++)
    {
        if(iter->name == stationName)
            return &(*iter);
    }
    return NULL;
}

Track* FindOrNewTrack(Station *station1, Station *station2, int weight, Line *p2line, SubwaySystem &subwaySystem)
{
    QList<Track>::iterator iter;
    for(iter = subwaySystem.tracks.begin(); iter != subwaySystem.tracks.end(); iter++)
    {
        if((iter->s1 == station1 && iter->s2 == station2) || (iter->s1 == station2 && iter->s2 == station1))
        {
            iter->p2inTrakLines.append(p2line);
            return &(*iter);
        }
    }
    Track temp;
    temp.s1 = station1;
    temp.s2 = station2;
    temp.weight = weight;
    temp.p2inTrakLines.append(p2line);
    subwaySystem.tracks.append(temp);
    Track* ptr = &subwaySystem.tracks.last();
    //qDebug() << ptr->s1->pos.y() << ptr->s2->pos.y();
    station1->p2Tracks.append(ptr);
    station2->p2Tracks.append(ptr);
    p2line->p2inLineTracks.append(ptr);
    return ptr;
}
