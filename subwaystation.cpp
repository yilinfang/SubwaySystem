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
//    LineListInit(subwaySystem.lineList);
//    StationListInit(subwaySystem.stationList);
//    TrackListInit(subwaySystem.trackList);
    while(!stream.atEnd())
    {
        line = stream.readLine();
        qDebug() << line;
        if(line.startsWith("#", Qt::CaseSensitive)) //读取到线路信息
        {
            //subwaySystem.lineTable.append(tempLine);
            qDebug() << subwaySystem.lineList.length << subwaySystem.lineList.size;
            LineListAppend(subwaySystem.lineList, tempLine);
            qDebug() << 1;
            p2line = &subwaySystem.lineList.arr[subwaySystem.lineList.length - 1];
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
                file.close();
                return ERROR;
            }
            ++iter;
            y = iter->toInt(ptr, 10);
            qDebug() << y;
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                file.close();
                return ERROR;
            }
            ptr1->pos.setX(x);
            ptr1->pos.setY(y);
            ++iter;
            int weight = iter->toInt(ptr, 10);
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                file.close();
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
                file.close();
                return ERROR;
            }
            qDebug() << x;
            ++iter;
            y = iter->toInt(ptr, 10);
            qDebug() << y;
            if(!flag)
            {
                outputBufa = "文件格式错误！";
                file.close();
                return ERROR;
            }
            ptr2->pos.setX(x);
            ptr2->pos.setY(y);
            FindOrNewTrack(ptr1, ptr2, weight, p2line, subwaySystem);
        }
    }
    file.close();
    return OK;
}


Station* FindOrNewStation(QString stationName, Line *p2line, SubwaySystem &subwaySystem)
{
    Station* ptr = FindStationByName(stationName, subwaySystem);
    if(!ptr) //该站点第一次出现
    {
        Station temp;
        temp.name = stationName;
        //temp.p2transferLines.append(p2line);
        //P2LineListInit(temp.transferLines);
        P2LineListAppend(temp.transferLines, p2line);
        //subwaySystem.stationTable.append(temp);
        StationListAppend(subwaySystem.stationList, temp);
        ptr = &subwaySystem.stationList.arr[subwaySystem.stationList.length - 1];
        P2StationListAppend(p2line->inLineStations, ptr);
    }
    else //该站点已经出现过
    {
        //QList<Line*>::iterator iter;
        int signal = 0;
//        for(iter = ptr->p2transferLines.begin(); iter != ptr->p2transferLines.end(); iter++)
//        {
//            if(*iter == p2line)
//            {
//                signal = 1;
//                break;
//            }
//        }
        for(int i = 0; i < ptr->transferLines.length; i++)
        {
            if(ptr->transferLines.arr[i] == p2line)
            {
                signal = 1;
                break;
            }
        }
        if(!signal) //该站点出现在一条新线路中
        {
            //ptr->p2transferLines.append(p2line);
            P2LineListAppend(ptr->transferLines, p2line);
            //p2line->p2inLineStations.append(ptr);
            P2StationListAppend(p2line->inLineStations, ptr);
        }
    }
    return ptr;
}

Station* FindStationByName(QString stationName, SubwaySystem &subwaySystem)
{
//    QList<Station>::iterator iter;
//    for(iter = subwaySystem.stationTable.begin(); iter != subwaySystem.stationTable.end(); iter++)
//    {
//        if(iter->name == stationName)
//            return &(*iter);
//    }
    for(int i = 0; i < subwaySystem.stationList.length; i++)
    {
        if(subwaySystem.stationList.arr[i].name == stationName)
            return &subwaySystem.stationList.arr[i];
    }
    return NULL;
}

Track* FindOrNewTrack(Station *station1, Station *station2, int weight, Line *p2line, SubwaySystem &subwaySystem)
{
//    QList<Track>::iterator iter;
//    for(iter = subwaySystem.tracks.begin(); iter != subwaySystem.tracks.end(); iter++)
//    {
//        if((iter->s1 == station1 && iter->s2 == station2) || (iter->s1 == station2 && iter->s2 == station1))
//        {
//            iter->p2inTrakLines.append(p2line);
//            return &(*iter);
//        }
//    }
    for(int i = 0; i < subwaySystem.trackList.length - 1; i++)
    {
        if((subwaySystem.trackList.arr[i].s1 == station1 && subwaySystem.trackList.arr[i].s2 == station2) || (subwaySystem.trackList.arr[i].s1 == station2 && subwaySystem.trackList.arr[i].s2 == station1))
        {
            P2LineListAppend(subwaySystem.trackList.arr[i].LineList, p2line);
            return &subwaySystem.trackList.arr[i];
        }
    }
    Track temp;
    temp.s1 = station1;
    temp.s2 = station2;
    temp.weight = weight;
    //temp.p2inTrakLines.append(p2line);
    P2LineListAppend(temp.LineList, p2line);
//    subwaySystem.tracks.append(temp);
    TrackListAppend(subwaySystem.trackList, temp);
    Track* ptr = &subwaySystem.trackList.arr[subwaySystem.trackList.length - 1];
    //qDebug() << ptr->s1->pos.y() << ptr->s2->pos.y();
//    station1->p2Tracks.append(ptr);
    P2TrackListAppend(station1->trackList, ptr);
//    station2->p2Tracks.append(ptr);
    P2TrackListAppend(station2->trackList, ptr);
//    p2line->p2inLineTracks.append(ptr);
    P2TrackListAppend(p2line->inLineTrack, ptr);
    return ptr;
}

state SaveSubwaySystem(QString fileName, SubwaySystem &subwaySystem, QString &outputBufa)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        outputBufa = "文件打开失败!";
        return ERROR;
    }
    QTextStream stream(&file);
//    if(subwaySystem.lineTable.isEmpty())
//    {
//        outputBufa = "地铁系统为空!";
//        file.close();
//        return OK;
//    }
//    QList<Line>::iterator l_iter;
//    for(l_iter = subwaySystem.lineTable.begin(); l_iter != subwaySystem.lineTable.end(); l_iter++)
//    {
//        stream << l_iter->name << endl;
//        qDebug() << l_iter->name << endl;
//        if(!l_iter->p2inLineTracks.isEmpty())
//        {
//            QList<Track*>::iterator t_iter;
//            for(t_iter = l_iter->p2inLineTracks.begin(); t_iter != l_iter->p2inLineTracks.end(); t_iter++)
//            {
//                stream << (*t_iter)->s1->name << "," << (*t_iter)->s1->pos.x() << "," << (*t_iter)->s1->pos.y() << "," << (*t_iter)->weight << "," << (*t_iter)->s2->name << "," << (*t_iter)->s2->pos.x() << "," << (*t_iter)->s2->pos.y() << endl;
//                qDebug() << (*t_iter)->s1->name << "," << (*t_iter)->s1->pos.x() << "," << (*t_iter)->s1->pos.y() << "," << (*t_iter)->weight << "," << (*t_iter)->s2->name << "," << (*t_iter)->s2->pos.x() << "," << (*t_iter)->s2->pos.y() << endl;
//            }
//        }
//    }
    if(subwaySystem.lineList.length == 0)
    {
        outputBufa = "地铁系统为空!";
        file.close();
        return OK;
    }
    for(int i = 0; i < subwaySystem.lineList.length - 1; i++)
    {
        stream << subwaySystem.lineList.arr[i].name << endl;
        if(subwaySystem.lineList.arr[i].inLineTrack.length != 0)
        {
            for(int j = 0; j < subwaySystem.lineList.arr[i].inLineTrack.length - 1; i++)
            {
                stream << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s1->name << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s1->pos.x() << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s1->pos.y() << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->weight << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s2->name << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s2->pos.x() << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s2->pos.y() << endl;
            }
        }
    }
    file.close();
    outputBufa = "文件保存成功!";
    return OK;
}

state StationListInit(StationList &stationList)
{
    stationList.length = 0;
    stationList.arr = (Station*)malloc(INITIALSIZE * sizeof(Station));
    if(!stationList.arr)
        return ERROR;
    stationList.size = INITIALSIZE;
    return OK;
}

state StationListAppend(StationList &stationList, Station station)
{
    if(stationList.length == stationList.size)
    {
        stationList.size += STEPSIZE;
        stationList.arr = (Station*)realloc(stationList.arr, stationList.size * sizeof(Station));
        if(!stationList.arr)
        {
            return ERROR;
        }
    }
    stationList.arr[stationList.length] = station;
    stationList.length++;
    return OK;
}

state StationListAppendList(StationList &stationList1, StationList stationList2)
{
    for(int i = 0; i < stationList2.length; i++)
    {
        if(StationListAppend(stationList1, stationList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state StationListEmpty(StationList &stationList)
{
    stationList.length = 0;
    return OK;
}

state StationListDestroy(StationList &stationList)
{
    stationList.size = stationList.length = 0;
    free(stationList.arr);
    return OK;
}

state LineListInit(LineList &lineList)
{
    lineList.length = 0;
    lineList.arr = (Line*)malloc(INITIALSIZE * sizeof(Line));
    if(!lineList.arr)
        return ERROR;
    lineList.size = INITIALSIZE;
    return OK;
}

state LineListAppend(LineList &lineList, Line line)
{
    if(lineList.length == lineList.size)
    {
        lineList.size += STEPSIZE;
        qDebug() << lineList.arr;
        lineList.arr = (Line*)realloc(lineList.arr, lineList.size * sizeof(Line));
        //qDebug() << lineList.size;
        //qDebug() << lineList.arr;
        if(!lineList.arr)
        {
            return ERROR;
        }
    }
    lineList.arr[lineList.length] = line;
    lineList.length++;
    qDebug() << lineList.length;
    return OK;
}

state LineListAppendList(LineList &lineList1, LineList lineList2)
{
    for(int i = 0; i < lineList2.length; i++)
    {
        if(LineListAppend(lineList1, lineList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state LineListEmpty(LineList &lineList)
{
    lineList.length = 0;
    return OK;
}

state LineListDestroy(LineList &lineList)
{
    lineList.size = lineList.length = 0;
    free(lineList.arr);
    return OK;
}

state TrackListInit(TrackList &trackList)
{
    trackList.length = 0;
    trackList.arr = (Track*)malloc(INITIALSIZE * sizeof(Track));
    if(!trackList.arr)
        return ERROR;
    trackList.size = INITIALSIZE;
    return OK;
}

state TrackListAppend(TrackList &trackList, Track track)
{
    if(trackList.length == trackList.size)
    {
        trackList.size += STEPSIZE;
        trackList.arr = (Track*)realloc(trackList.arr, trackList.size * sizeof(Track));
        if(!trackList.arr)
        {
            return ERROR;
        }
    }
    trackList.arr[trackList.length] = track;
    trackList.length++;
    return OK;
}

state TrackListAppendList(TrackList &trackList1, TrackList trackList2)
{
    for(int i = 0; i < trackList2.length; i++)
    {
        if(TrackListAppend(trackList1, trackList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state TrackListEmpty(TrackList &trackList)
{
    trackList.length = 0;
    return OK;
}

state TrackListDestroy(TrackList &trackList)
{
    trackList.size = trackList.length = 0;
    free(trackList.arr);
    return OK;
}

state EdgeListInit(EdgeList &edgeList)
{
    edgeList.length = 0;
    edgeList.arr = (Edge*)malloc(INITIALSIZE * sizeof(Edge));
    if(!edgeList.arr)
        return ERROR;
    edgeList.size = INITIALSIZE;
    return OK;
}

state EdgeListAppend(EdgeList &edgeList, Edge edge)
{
    if(edgeList.length == edgeList.size)
    {
        edgeList.size += STEPSIZE;
        edgeList.arr = (Edge*)realloc(edgeList.arr, edgeList.size * sizeof(Edge));
        if(!edgeList.arr)
        {
            return ERROR;
        }
    }
    edgeList.arr[edgeList.length] = edge;
    edgeList.length++;
    return OK;
}

state EdgeListAppendList(EdgeList &edgeList1, EdgeList edgeList2)
{
    for(int i = 0; i < edgeList2.length; i++)
    {
        if(EdgeListAppend(edgeList1, edgeList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state EdgeListEmpty(EdgeList &edgeList)
{
    edgeList.length = 0;
    return OK;
}

state EdgeListDestroy(EdgeList &edgeList)
{
    edgeList.size = edgeList.length = 0;
    free(edgeList.arr);
    return OK;
}

state P2StationListInit(P2StationList &p2StationList)
{
    p2StationList.length = 0;
    p2StationList.arr = (Station**)malloc(INITIALSIZE * sizeof(Station*));
    if(!p2StationList.arr)
        return ERROR;
    p2StationList.size = INITIALSIZE;
    return OK;
}

state P2StationListAppend(P2StationList &p2StationList, Station* p2Station)
{
    if(p2StationList.length == p2StationList.size)
    {
        p2StationList.size += STEPSIZE;
        p2StationList.arr = (Station**)realloc(p2StationList.arr, p2StationList.size * sizeof(Station*));
        if(!p2StationList.arr)
        {
            return ERROR;
        }
    }
    p2StationList.arr[p2StationList.length] = p2Station;
    p2StationList.length++;
    return OK;
}

state P2StationListAppendList(P2StationList &p2StationList1, P2StationList p2StationList2)
{
    for(int i = 0; i < p2StationList2.length; i++)
    {
        if(P2StationListAppend(p2StationList1, p2StationList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state P2StationListEmpty(P2StationList &p2StationList)
{
    p2StationList.length = 0;
    return OK;
}

state P2StationListDestroy(P2StationList &p2StationList)
{
    p2StationList.size = p2StationList.length = 0;
    free(p2StationList.arr);
    return OK;
}

state P2LineListInit(P2LineList &p2LineList)
{
    p2LineList.length = 0;
    p2LineList.arr = (Line**)malloc(INITIALSIZE * sizeof(Line*));
    if(!p2LineList.arr)
        return ERROR;
    p2LineList.size = INITIALSIZE;
    return OK;
}

state P2LineListAppend(P2LineList &p2LineList, Line* p2Line)
{
    if(p2LineList.length == p2LineList.size)
    {
        p2LineList.size += STEPSIZE;
        p2LineList.arr = (Line**)realloc(p2LineList.arr, p2LineList.size * sizeof(Line*));
        if(!p2LineList.arr)
        {
            return ERROR;
        }
    }
    p2LineList.arr[p2LineList.length] = p2Line;
    p2LineList.length++;
    return OK;
}

state P2LineListAppendList(P2LineList &p2LineList1, P2LineList p2LineList2)
{
    for(int i = 0; i < p2LineList2.length; i++)
    {
        if(P2LineListAppend(p2LineList1, p2LineList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state P2LineListEmpty(P2LineList &p2LineList)
{
    p2LineList.length = 0;
    return OK;
}

state P2LineListDestroy(P2LineList &p2LineList)
{
    p2LineList.size = p2LineList.length = 0;
    free(p2LineList.arr);
    return OK;
}

state P2TrackListInit(P2TrackList &p2TrackList)
{
    p2TrackList.length = 0;
    p2TrackList.arr = (Track**)malloc(INITIALSIZE * sizeof(Track*));
    if(!p2TrackList.arr)
        return ERROR;
    p2TrackList.size = INITIALSIZE;
    return OK;
}

state P2TrackListAppend(P2TrackList &p2TrackList, Track* p2Track)
{
    if(p2TrackList.length == p2TrackList.size)
    {
        p2TrackList.size += STEPSIZE;
        p2TrackList.arr = (Track**)realloc(p2TrackList.arr, p2TrackList.size * sizeof(Track*));
        if(!p2TrackList.arr)
        {
            return ERROR;
        }
    }
    p2TrackList.arr[p2TrackList.length] = p2Track;
    p2TrackList.length++;
    return OK;
}

state P2TrackListAppendList(P2TrackList &p2TrackList1, P2TrackList p2TrackList2)
{
    for(int i = 0; i < p2TrackList2.length; i++)
    {
        if(P2TrackListAppend(p2TrackList1, p2TrackList2.arr[i]) == ERROR)
        {
            return ERROR;
        }
    }
    return OK;
}

state P2TrackListEmpty(P2TrackList &p2TrackList)
{
    p2TrackList.length = 0;
    return OK;
}

state P2TrackListDestroy(P2TrackList &p2TrackList)
{
    p2TrackList.size = p2TrackList.length = 0;
    free(p2TrackList.arr);
    return OK;
}
