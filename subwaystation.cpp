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
    tempLine.inLineStations = NULL;
    tempLine.inLineTrack = NULL;
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
//            qDebug() << subwaySystem.lineList.length << subwaySystem.lineList.size;
            LineListAppend(subwaySystem.lineList, tempLine);
            p2line= LineListLast(subwaySystem.lineList);
            p2line->name = line;
            qDebug() << p2line->name;
//            qDebug() << 1;
//            p2line = &subwaySystem.lineList.arr[subwaySystem.lineList.length - 1];
//            p2line->name = line;
        }
        else    //读取到站点以及路径信息
        {
            QStringList tempList = line.split(',', QString::SkipEmptyParts);
            QList<QString>::iterator iter = tempList.begin();
            QString name1 = *iter;
            qDebug() << name1;
            StationNode *ptr1, *ptr2;
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


Station *FindOrNewStation(QString stationName, Line *p2line, SubwaySystem &subwaySystem)
{
    Station* ptr = FindStationByName(stationName, subwaySystem);
    qDebug() << ptr;
    if(!ptr) //该站点第一次出现
    {
        Station temp;
        temp.trackList = NULL;
        temp.transferLines = NULL;
        temp.name = stationName;
        //temp.p2transferLines.append(p2line);
        //P2LineListInit(temp.transferLines);
        P2LineListAppend(temp.transferLines, p2line);
        //subwaySystem.stationTable.append(temp);
        StationListAppend(subwaySystem.stationList, temp);
        ptr = StationListLast(subwaySystem.stationList);
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
//        for(int i = 0; i < ptr->transferLines.length; i++)
//        {
//            if(ptr->transferLines.arr[i] == p2line)
//            {
//                signal = 1;
//                break;
//            }
//        }
        P2LineNode* p = ptr->transferLines;
        while(p)
        {
            if(p->p2Line == p2line)
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
//    for(int i = 0; i < subwaySystem.stationList.length; i++)
//    {
//        if(subwaySystem.stationList.arr[i].name == stationName)
//            return &subwaySystem.stationList.arr[i];
//    }
//    return NULL;
    if(!subwaySystem.stationList)
        return NULL;
    StationNode* p = subwaySystem.stationList;
    while(p)
    {
        qDebug() << p->name;
        if(p->name == stationName)
        {
            return p;
        }
        p = p->next;
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
//    for(int i = 0; i < subwaySystem.trackList.length - 1; i++)
//    {
//        if((subwaySystem.trackList.arr[i].s1 == station1 && subwaySystem.trackList.arr[i].s2 == station2) || (subwaySystem.trackList.arr[i].s1 == station2 && subwaySystem.trackList.arr[i].s2 == station1))
//        {
//            P2LineListAppend(subwaySystem.trackList.arr[i].LineList, p2line);
//            return &subwaySystem.trackList.arr[i];
//        }
//    }
//    Track temp;
//    temp.s1 = station1;
//    temp.s2 = station2;
//    temp.weight = weight;
//    //temp.p2inTrakLines.append(p2line);
//    P2LineListAppend(temp.LineList, p2line);
//    subwaySystem.tracks.append(temp);
//    TrackListAppend(subwaySystem.trackList, temp);
//    Track* ptr = &subwaySystem.trackList.arr[subwaySystem.trackList.length - 1];
//    //qDebug() << ptr->s1->pos.y() << ptr->s2->pos.y();
//    station1->p2Tracks.append(ptr);
//    P2TrackListAppend(station1->trackList, ptr);
//    station2->p2Tracks.append(ptr);
//    P2TrackListAppend(station2->trackList, ptr);
//    p2line->p2inLineTracks.append(ptr);
//    P2TrackListAppend(p2line->inLineTrack, ptr);
    Track* p = subwaySystem.trackList;
    while(p)
    {
        if((p->s1 == station1 && p->s2 == station2) || (p->s1 == station2 && p->s2 == station1))
        {
            P2LineListAppend(p->LineList, p2line);
            return p;
        }
        p = p->next;
    }
    Track temp;
    temp.s1 = station1;
    temp.s2 = station2;
    temp.LineList = NULL;
    temp.weight = weight;
    P2LineListAppend(temp.LineList, p2line);
    TrackListAppend(subwaySystem.trackList, temp);
    TrackNode* ptr = TrackListLast(subwaySystem.trackList);
    P2TrackListAppend(station1->trackList, ptr);
    P2TrackListAppend(station2->trackList, ptr);
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
    if(!subwaySystem.lineList)
    {
        outputBufa = "地铁系统为空!";
        file.close();
        return OK;
    }
//    for(int i = 0; i < subwaySystem.lineList.length - 1; i++)
//    {
//        stream << subwaySystem.lineList.arr[i].name << endl;
//        if(subwaySystem.lineList.arr[i].inLineTrack.length != 0)
//        {
//            for(int j = 0; j < subwaySystem.lineList.arr[i].inLineTrack.length - 1; i++)
//            {
//                stream << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s1->name << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s1->pos.x() << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s1->pos.y() << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->weight << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s2->name << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s2->pos.x() << "," << subwaySystem.lineList.arr[i].inLineTrack.arr[j]->s2->pos.y() << endl;
//            }
//        }
//    }
    LineNode* p = subwaySystem.lineList;
    while(p)
    {
        stream << p->name << endl;
        P2TrackNode* q = p->inLineTrack;
        while(q)
        {
            stream << q->p2Track->s1->name << "," << q->p2Track->s1->pos.x() << "," << q->p2Track->s1->pos.y() << "," << q->p2Track->weight << "," << q->p2Track->s2->name << "," << q->p2Track->s2->pos.x() << "," << q->p2Track->s2->pos.y() << endl;
            q = q->next;
        }
        p = p->next;
    }
    file.close();
    outputBufa = "文件保存成功!";
    return OK;
}

state StationListInit(StationList &stationList)
{
    if(stationList)
    {
        StationListEmpty(stationList);
    }
    return OK;
}

state StationListAppend(StationList &stationList, Station station)
{
    if(!stationList)
    {
        StationNode* p = new StationNode;
        if(!p)
        {
            return ERROR;
        }
        *p = station;
        p->next = NULL;
        stationList = p;
        return OK;
    }
    else{
        StationNode* p = stationList;
        while(p->next)
        {
            p = p->next;
        }
        StationNode* q = new StationNode;
        if(!q)
        {
            return ERROR;
        }
        *q = station;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state StationListAppendList(StationList &stationList1, StationList stationList2)
{
    if(!stationList1)
    {
        stationList1 = stationList2;
        return OK;
    }
    else{
        StationNode* p = stationList2;
        while(p)
        {
            StationListAppend(stationList1, *p);
            p = p->next;
        }
        return OK;
    }
}

Station *StationListLast(StationList stationList)
{
    StationNode* p = stationList;
    if(!p)
    {
        return NULL;
    }
    while (p->next)
    {
        p=p->next;
    }
    return p;
}

state StationListEmpty(StationList &stationList)
{
    StationNode* p = stationList;
    StationNode* q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    stationList = NULL;
    return OK;

}

state StationListDestroy(StationList &stationList)
{
    return StationListEmpty(stationList);
}

state LineListInit(LineList &lineList)
{
    if(lineList)
    {
        LineListEmpty(lineList);
    }
    return OK;
}

state LineListAppend(LineList &lineList, Line &line)
{
    if(!lineList)
    {
        LineNode* p = new LineNode;
        if(!p)
        {
            return ERROR;
        }
        *p = line;
        p->next = NULL;
        lineList = p;
        return OK;
    }
    else{
        LineNode* p = lineList;
        while(p->next)
        {
            p = p->next;
        }
        LineNode* q = new LineNode;
        if(!q)
        {
            return ERROR;
        }
        *q = line;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state LineListAppendList(LineList &lineList1, LineList lineList2)
{
    if(!lineList1)
    {
        lineList1 = lineList2;
        return OK;
    }
    else{
        LineNode* p = lineList2;
        while(p)
        {
            LineListAppend(lineList1, *p);
            p = p->next;
        }
        return OK;
    }
}

Line *LineListLast(LineList &lineList)
{
    LineNode* p = lineList;
    if(!p)
    {
        return NULL;
    }
    while(p->next)
    {
        p = p->next;
    }
    return p;
}

state LineListEmpty(LineList &lineList)
{
    LineNode* p = lineList;
    LineNode* q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    lineList = NULL;
    return OK;

}

state LineListDestroy(LineList &lineList)
{
    return LineListEmpty(lineList);
}

state TrackListInit(TrackList &trackList)
{
    if(trackList)
    {
        TrackListEmpty(trackList);
    }
    return OK;
}

state TrackListAppend(TrackList &trackList, Track track)
{
    if(!trackList)
    {
        TrackNode* p = new TrackNode;
        if(!p)
        {
            return ERROR;
        }
        *p = track;
        p->next = NULL;
        trackList = p;
        return OK;
    }
    else{
        TrackNode* p = trackList;
        while(p->next)
        {
            p = p->next;
        }
        TrackNode* q = new TrackNode;
        if(!q)
        {
            return ERROR;
        }
        *q = track;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state TrackListAppendList(TrackList &trackList1, TrackList trackList2)
{
    if(!trackList1)
    {
        trackList1 = trackList2;
        return OK;
    }
    else{
        TrackNode* p = trackList2;
        while(p)
        {
            TrackListAppend(trackList1, *p);
            p = p->next;
        }
        return OK;
    }
}

TrackNode* TrackListLast(TrackList trackList)
{
    TrackNode* p = trackList;
    if(!p)
    {
        return NULL;
    }
    while(p->next)
    {
        p = p->next;
    }
    return p;
}

state TrackListEmpty(TrackList &trackList)
{
    TrackNode* p = trackList;
    TrackNode* q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    trackList = NULL;
    return OK;

}

state TrackListDestroy(TrackList &trackList)
{
    return TrackListEmpty(trackList);
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
    if(p2StationList)
    {
        P2StationListEmpty(p2StationList);
    }
    return OK;
}

state P2StationListAppend(P2StationList &p2StationList, Station* p2Station)
{
    if(!p2StationList)
    {
        P2StationNode* p = new P2StationNode;
        if(!p)
        {
            return ERROR;
        }
        p->p2Station = p2Station;
        p->next = NULL;
        p2StationList = p;
        return OK;
    }
    else{
        P2StationNode* p = p2StationList;
        while(p->next)
        {
            p = p->next;
        }
        P2StationNode* q = new P2StationNode;
        if(!q)
        {
            return ERROR;
        }
        q->p2Station = p2Station;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state P2StationListAppendList(P2StationList &p2StationList1, P2StationList p2StationList2)
{
    if(!p2StationList1)
    {
        p2StationList1 = p2StationList2;
        return OK;
    }
    else{
        P2StationNode* p = p2StationList2;
        while(p)
        {
            P2StationListAppend(p2StationList1, p->p2Station);
            p = p->next;
        }
        return OK;
    }
}

state P2StationListEmpty(P2StationList &p2StationList)
{
    P2StationNode* p = p2StationList;
    P2StationNode* q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    p2StationList = NULL;
    return OK;

}

state P2StationListDestroy(P2StationList &p2StationList)
{
    return P2StationListEmpty(p2StationList);
}

state P2LineListInit(P2LineList &p2LineList)
{
    if(p2LineList)
    {
        P2LineListEmpty(p2LineList);
    }
    return OK;
}

state P2LineListAppend(P2LineList &p2LineList, Line* p2Line)
{
    if(!p2LineList)
    {
        P2LineNode* p = new P2LineNode;
        if(!p)
        {
            return ERROR;
        }
        p->p2Line = p2Line;
        p->next = NULL;
        p2LineList = p;
        return OK;
    }
    else{
        P2LineNode* p = p2LineList;
        while(p->next)
        {
            p = p->next;
        }
        P2LineNode* q = new P2LineNode;
        if(!q)
        {
            return ERROR;
        }
        q->p2Line = p2Line;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state P2LineListAppendList(P2LineList &p2LineList1, P2LineList p2LineList2)
{
    if(!p2LineList1)
    {
        p2LineList1 = p2LineList2;
        return OK;
    }
    else{
        P2LineNode* p = p2LineList2;
        while(p)
        {
            P2LineListAppend(p2LineList1, p->p2Line);
            p = p->next;
        }
        return OK;
    }
}

state P2LineListEmpty(P2LineList &p2LineList)
{
    P2LineNode* p = p2LineList;
    P2LineNode* q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    p2LineList = NULL;
    return OK;

}

state P2LineListDestroy(P2LineList &p2LineList)
{
    return P2LineListEmpty(p2LineList);
}

//state P2TrackListInit(P2TrackList &p2TrackList)
//{
//    p2TrackList.length = 0;
//    p2TrackList.arr = (Track**)malloc(INITIALSIZE * sizeof(Track*));
//    if(!p2TrackList.arr)
//        return ERROR;
//    p2TrackList.size = INITIALSIZE;
//    return OK;
//}

state P2TrackListInit(P2TrackList &p2TrackList)
{
    if(p2TrackList)
    {
        P2TrackListEmpty(p2TrackList);
    }
    return OK;
}

state P2TrackListAppend(P2TrackList &p2TrackList, Track* p2Track)
{
    if(!p2TrackList)
    {
        P2TrackNode* p = new P2TrackNode;
        if(!p)
        {
            return ERROR;
        }
        p->p2Track = p2Track;
        p->next = NULL;
        p2TrackList = p;
        return OK;
    }
    else{
        P2TrackNode* p = p2TrackList;
        while(p->next)
        {
            p = p->next;
        }
        P2TrackNode* q = new P2TrackNode;
        if(!q)
        {
            return ERROR;
        }
        q->p2Track = p2Track;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state P2TrackListAppendList(P2TrackList &p2TrackList1, P2TrackList p2TrackList2)
{
    if(!p2TrackList1)
    {
        p2TrackList1 = p2TrackList2;
        return OK;
    }
    else{
        P2TrackNode* p = p2TrackList2;
        while(p)
        {
            P2TrackListAppend(p2TrackList1, p->p2Track);
            p = p->next;
        }
        return OK;
    }
}

state P2TrackListEmpty(P2TrackList &p2TrackList)
{
    P2TrackNode* p = p2TrackList;
    P2TrackNode* q;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    p2TrackList = NULL;
    return OK;

}

state P2TrackListDestroy(P2TrackList &p2TrackList)
{
    return P2TrackListEmpty(p2TrackList);
}
