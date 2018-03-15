#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subwaysystem.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1024,768);
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->displayLabel1->setWordWrap(true);
    ui->displayLabel2->setWordWrap(true);
    ui->displayLabel3->setWordWrap(true);
    ui->displayLabel1->setAlignment(Qt::AlignTop);
    ui->displayLabel2->setAlignment(Qt::AlignTop);
    ui->displayLabel3->setAlignment(Qt::AlignTop);
    ui->groupBoxMap->hide();
    ui->groupBoxAdmins->hide();
    ui->groupBoxAdminTools->hide();
//    dirPos = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/";
    dirPos = qApp->applicationDirPath() + "/";
    isFirstStart = 1;
    LoadAdmins();
    //ui->horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
    points2Draw = NULL;
    drawedPoints = NULL;
    lines2Draw = NULL;
    drawedTracks = NULL;
    chosenLines = NULL;
    chosenStations = NULL;
    chosenTracks = NULL;
    subwaySystem.lineList = NULL;
    subwaySystem.trackList = NULL;
    subwaySystem.stationList = NULL;
    subwaySystem.edgeNum = 0;
    subwaySystem.lineNum = 0;
    subwaySystem.stationNum = 0;
    map.dist = NULL;
    map.matrix = NULL;
    map.matrix_transfer = NULL;
    map.path = NULL;
    map.vertexTable = NULL;
    map.edgeNum = 0;
    map.vertexNum = 0;
    //temp = "qt";
}

MainWindow::~MainWindow()
{
    SubwaySystemDestroy(subwaySystem);
    DestroyMap(map);
    PointListDestroy(points2Draw);
    PointListDestroy(drawedPoints);
    DLineListDestroy(lines2Draw);
    DLineListDestroy(drawedTracks);
    delete ui;
}

void MainWindow::ShowStation(Station *station, QPoint pos)
{
    QString str;
    str += station->name;
    str += " ";
    pos.setX(station->pos.x());
    pos.setY(station->pos.y());
    P2LineNode* p = station->transferLines;
    while(p)
    {
        str += p->p2Line->name;
        str += " ";
        p = p->next;
    }
    ui->statusBar->showMessage("(" + QString::number(pos.x(), 10) + "," + QString::number(pos.y(), 10) + ")" + str);
}

void MainWindow::ShowTrack(Track *track, QPoint pos)
{
    //ui->displayLabel->clear();
    //QFont font("Microsoft YaHei", 10, 75);
    //ui->displayLabel->setFont(font);
    ui->statusBar->showMessage("(" + QString::number(pos.x(), 10) + "," + QString::number(pos.y(), 10) + ")" + track->s1->name + "---" + track->s2->name);
}

Station *MainWindow::IsStationAround(QPoint pos)
{
//    if(drawedPoints.isEmpty())
//        return NULL;
//    QList<Point>::iterator iter;
//    for(iter = drawedPoints.begin(); iter != drawedPoints.end(); iter++)
//    {
//        long dis = (iter->station->pos.x() - pos.x()) * (iter->station->pos.x() - pos.x()) + (iter->station->pos.y() - pos.y()) * (iter->station->pos.y() - pos.y());
//        if(dis <= 4)
//        {
//            return iter->station;
//        }
//    }
//    return NULL;
    if(!drawedPoints)
        return NULL;
//    for(int i = 0; i < drawedPoints.length - 1; i++)
//    {
//        long dis = (drawedPoints.arr[i].station->pos.x() - pos.x()) * (drawedPoints.arr[i].station->pos.x() - pos.x()) + (drawedPoints.arr[i].station->pos.y() - pos.y()) * (drawedPoints.arr[i].station->pos.y() - pos.y());
//        if(dis <= 4)
//        {
//            return drawedPoints.arr[i].station;
//        }
//    }
    PointNode* p = drawedPoints;
    while(p)
    {
        long dis = (p->station->pos.x() - pos.x()) * (p->station->pos.x() - pos.x()) + (p->station->pos.y() - pos.y()) * (p->station->pos.y() - pos.y());
        if(dis <= 16)
        {
            return p->station;
        }
        p = p->next;
    }
    return NULL;
}

Track *MainWindow::IsTrackAround(QPoint pos)
{
//    if(drawedTracks.isEmpty())
//        return NULL;
//    QList<DLine>::iterator iter;
//    for(iter = drawedTracks.begin(); iter != drawedTracks.end(); iter++)
//    {
//        if((pos.x() - iter->track->s1->pos.x()) * (iter->track->s2->pos.y() - iter->track->s1->pos.y()) == (iter->track->s1->pos.x() - iter->track->s2->pos.x()) * (pos.y() - iter->track->s1->pos.y())) //叉乘为0则三点共线
//        {
//            if((pos.x() - iter->track->s1->pos.x()) * (pos.x() - iter->track->s2->pos.x()) <= 0 && (pos.y() - iter->track->s1->pos.y()) * (pos.y() - iter->track->s2->pos.y()) <= 0) //如果pos在两点之间，则pos处在该条线段中
//            {
//                return iter->track;
//            }
//        }
//    }
//    return NULL;
    if(!drawedTracks)
    {
        return NULL;
    }
//    for(int i = 0; i < drawedTracks.length - 1; i++)
//    {
//        if((pos.x() - drawedTracks.arr[i].track->s1->pos.x()) * (drawedTracks.arr[i].track->s2->pos.y() - drawedTracks.arr[i].track->s1->pos.y()) == (drawedTracks.arr[i].track->s1->pos.x() - drawedTracks.arr[i].track->s2->pos.x()) * (pos.y() - drawedTracks.arr[i].track->s1->pos.y()))
//        {
//            if((pos.x() - drawedTracks.arr[i].track->s1->pos.x()) * (pos.x() - drawedTracks.arr[i].track->s2->pos.x()) <= 0 && (pos.y() - drawedTracks.arr[i].track->s1->pos.y()) * (pos.y() - drawedTracks.arr[i].track->s2->pos.y()) <= 0)
//            {
//                return  drawedTracks.arr[i].track;
//            }
//        }
//    }
    DLineNode *p = drawedTracks;
    while(p)
    {
        int m = (pos.x() - p->track->s1->pos.x()) * (p->track->s2->pos.y() - p->track->s1->pos.y());
        int n = (p->track->s2->pos.x() - p->track->s1->pos.x()) * (pos.y() - p->track->s1->pos.y());
        int t = m - n;
        if(-1000 < t && t < 1000)
        {
            if((pos.x() - p->track->s1->pos.x()) * (pos.x() - p->track->s2->pos.x()) <= 0 && (pos.y() - p->track->s1->pos.y()) * (pos.y() - p->track->s2->pos.y()) <= 0)
            {
                return p->track;
            }
        }
        p = p->next;
    }
    return NULL;
}

void MainWindow::SetPointColor(StationList &stations, QColor color, PointList &list)
{
//    if(stations.size != 0)
//    {
//        for(int i = 0; i < stations.length - 1; i++)
//        {
//            Point point;
//            point.station = &stations.arr[i];
//            point.color = color;
//            PointListAppend(list, point);
//        }
//    }
    StationNode* p = stations;
    while (p)
    {
        //qDebug() << p->name;
        Point point;
        point.station = p;
        point.color = color;
        point.next = NULL;
        PointListAppend(list, point);
        p = p->next;
    }
}

//QList<Point> MainWindow::SetPointColor(QList<Station> &stations, QColor color)
//{
//    QList<Point> list;
//    list.clear();

//    if(!stations.isEmpty())
//    {
//        QList<Station>::iterator iter;
//        for(iter = stations.begin(); iter != stations.end(); iter++)
//        {
//            Point point;
//            point.station = &(*iter);
//            point.color = color;
//            list.append(point);
//        }
//    }
//    return list;
//}

void MainWindow::SetLineColor(TrackList &tracks, QColor color, DLineList &list)
{
//    if(tracks.length != 0)
//    {
//        for(int i = 0; i < tracks.length - 1; i++)
//        {
//            DLine line;
//            line.track = &tracks.arr[i];
//            line.color = color;
//            DLineListAppend(list, line);
//        }
//    }
    TrackNode* p = tracks;
    while(p)
    {
        DLine line;
        line.track = p;
        line.color = color;
        line.next = NULL;
        DLineListAppend(list, line);
        p = p->next;
    }
}

void MainWindow::ShowChosenStation()
{
//    if(chosenStations.isEmpty())
//    {
//        ui->displayLabel2->setText("未选中任何站点!");
//    }
//    else{
//        QList<Station*>::iterator iter;
//        QString str;
//        for(iter = chosenStations.begin(); iter != chosenStations.end(); iter++)
//        {
//            str += (*iter)->name;
//            str += ' ';
//        }
//        ui->displayLabel2->setText(str);
//    }
//    if(chosenStations.length == 0)
//    {
//        ui->displayLabel2->setText("未选中任何站点!");
//    }
//    else{
//        QString str;
//        for(int i = 0; i < chosenStations.length - 1; i++)
//        {
//            str += chosenStations.arr[i]->name;
//            str += " ";
//        }
//        ui->displayLabel2->setText(str);
//    }
    if(!chosenStations)
    {
        //ui->displayLabel2->adjustSize();
        //ui->displayLabel2->setAlignment(Qt::AlignTop);
        ui->displayLabel2->setText("");
    }
    else{
        //ui->displayLabel2->setWordWrap(true);
        P2StationNode* p = chosenStations;
        QString str;
        while(p)
        {
            str += p->p2Station->name;
            str += " ";
            p = p->next;
        }
        ui->displayLabel2->setText(str);
    }
}

void MainWindow::ShowChosenTracks()
{
    //ui->displayLabel3->setText(track->s1->name + "---" + track->s2->name);
    if(!chosenTracks)
    {
        ui->displayLabel1->setText("");
    }
    else
    {
        P2TrackNode* p = chosenTracks;
        QString str;
        while(p)
        {
            str += p->p2Track->s1->name;
            str += "---";
            str += p->p2Track->s2->name;;
            str +="\n";
            p = p->next;
        }
        ui->displayLabel1->setText(str);
    }
}

void MainWindow::ShowChosenLines()
{
    if(!chosenLines)
    {
        ui->displayLabel3->setText("");
    }
    else{
        QString str;
        P2LineList p = chosenLines;
        while(p)
        {
            str += p->p2Line->name;
            str += " ";
            p = p->next;
        }
        ui->displayLabel3->setText(str);
    }
}


void MainWindow::LoadAdmins()
{
    QString str = dirPos + "admins.dat";
    QFile file(str);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;
        adminsNum = 0;
        while(!stream.atEnd() && adminsNum < MAX_ADMIN_NUM)
        {
            line = stream.readLine();
            if(line.startsWith("#", Qt::CaseSensitive))
            {
                continue;
            }
            if(line.startsWith("@", Qt::CaseSensitive))
            {
                admins[adminsNum].username = line;
                continue;
            }
            admins[adminsNum].password = line;
            adminsNum++;
        }
    }
    file.close();
}

void MainWindow::SaveAdmins()
{
    //QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
    QString str = dirPos + "admins.dat";
    QFile file(str);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for(int i = 0; i < adminsNum; i++)
        {
            stream << admins[i].username;
            stream << admins[i].password;
        }
    }
    file.close();
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    int signal = 0;
    QPainter painter(this);
    //qDebug()<<"AAA";
    //    // 设置画笔颜色
//    if(!points2draw.isEmpty())
//    {
//        drawedPoints.clear();
//        drawedPoints.append(points2draw);
//        QList<Point>::iterator iter;
//        for(iter = points2draw.begin(); iter != points2draw.end(); iter++)
//        {
//            painter.setPen(QPen(iter->color, 10, Qt::SolidLine, Qt::RoundCap));
//            //qDebug() << iter->station->name;
//            painter.drawPoint(iter->station->pos.x(), iter->station->pos.y());
//        }
//    }
    if(lines2Draw)
    {
        signal = 1;
        DLineListEmpty(drawedTracks);
        DLineListAppendList(drawedTracks, lines2Draw);
        //DLineListEmpty(lines2Draw);
        DLineNode* p = lines2Draw;
        while(p)
        {
            //qDebug() << "track";
            painter.setPen(QPen(p->color, 5, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(p->track->s1->pos, p->track->s2->pos);
            p = p->next;
        }
        //DLineListEmpty(lines2Draw);
    }
    if(points2Draw)
    {
        signal = 1;
        PointListEmpty(drawedPoints);
        PointListAppendList(drawedPoints, points2Draw);
        PointNode* p = points2Draw;
        while(p)
        {
            //qDebug() << "point";
            painter.setPen(QPen(p->color, 10, Qt::SolidLine, Qt::RoundCap));
            painter.drawPoint(p->station->pos);
            p = p->next;
        }
        //PointListEmpty(points2Draw);
    }
//    if(!lines2draw.isEmpty())
//    {
//        drawedTracks.clear();
//        drawedTracks.append(lines2draw);
//        painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
//        QList<DLine>::iterator iter;
//        //qDebug() << lines2draw.size();
//        for(iter = lines2draw.begin(); iter != lines2draw.end(); iter++)
//        {
//            painter.setPen(QPen(iter->color,5,Qt::SolidLine,Qt::RoundCap));
//            painter.drawLine(iter->track->s1->pos, iter->track->s2->pos);
//            //painter.drawLine(iter->s1->pos.x(),iter->s1->pos.y(),iter->s2->pos.x(),iter->s2->pos.y());
//            //qDebug() << iter->s1->pos.x() << iter->s1->pos.y() << iter->s2->pos.x() << iter->s2->pos.y();
//        }
//    }
//    if(points2Draw.length != 0)
//    {
//        signal = 1;
//        PointListEmpty(drawedPoints);
//        PointListAppendList(drawedPoints, points2Draw);
//        for(int i = 0; i < points2Draw.length - 1; i++)
//        {
//            painter.setPen(QPen(points2Draw.arr[i].color,10,Qt::SolidLine,Qt::RoundCap));
//            painter.drawPoint(points2Draw.arr[i].station->pos);
//        }
//    }
//    if(lines2Draw.length != 0)
//    {
//        signal = 1;
//        DLineListEmpty(drawedTracks);
//        DLineListAppendList(drawedTracks, lines2Draw);
//        for(int i = 0; i < lines2Draw.length - 1; i++)
//        {
//            painter.setPen(QPen(lines2Draw.arr[i].color,5,Qt::SolidLine,Qt::RoundCap));
//            painter.drawLine(lines2Draw.arr[i].track->s1->pos, lines2Draw.arr[i].track->s2->pos);
//        }
//    }
    //qDebug() << "test";
    if(!signal)
    {
        // 设置画笔颜色
        painter.setPen(QColor(Qt::red));

        // 设置字体：微软雅黑、点大小50、斜体
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(25);
        font.setItalic(true);
        painter.setFont(font);

        // 绘制文本
        painter.drawText(rect(), Qt::AlignBaseline, "Thank you for using!");
    }
}


void MainWindow::on_pushButton_clicked()
{
//    QString str = ui->lineEdit->text();
//    //temp = str;
//    fileName = "data.dat";
//    str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/";
//    qDebug() << str;
//    QString outputBufa;
//    InitSubwaySystem(str + fileName, subwaySystem, outputBufa);
//    ui->pushButton->setText(outputBufa);
//    fileName = "data.txt";
//    SaveSubwaySystem(str + fileName, subwaySystem, outputBufa);
//    points2draw.clear();
//    //points2draw.append();
//    points2draw.append(SetPointColor(subwaySystem.stationTable, Qt::blue));
//    lines2draw.clear();
//    lines2draw.append(SetLineColor(subwaySystem.tracks, Qt::red));
//    //lines2draw.append(subwaySystem.tracks);
//    QWidget::update();
    //QString str = ui->inputLine1->testAttribute
//    fileName = "data.dat";
//    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/";
//    QString outputBufa;
//    InitSubwaySystem(str + fileName, subwaySystem, outputBufa);
//    qDebug() << outputBufa;
//    fileName = "data.txt";
//    SaveSubwaySystem(str + fileName, subwaySystem, outputBufa);
//    //qDebug() << 0;
//    PointList pointList = NULL;
//    SetPointColor(subwaySystem.stationList, Qt::black, pointList);
//    //qDebug() << pointList;
//    PointListAppendList(points2Draw, pointList);
//    PointListDestroy(pointList);
//    DLineList dLineList = NULL;
//    SetLineColor(subwaySystem.trackList, Qt::yellow, dLineList);
//    DLineListAppendList(lines2Draw, dLineList);
//    DLineListDestroy(dLineList);
//    InitMap(subwaySystem, map);
//    ShowMap(map);
//    //qDebug() << 3;
//    QWidget::update();
    if(isFirstStart)
    {
        QString outputBufa;
        InitSubwaySystem(dirPos + "data.dat", subwaySystem, outputBufa);
        PointList pointList = NULL;
        SetPointColor(subwaySystem.stationList, Qt::yellow, pointList);
        PointListAppendList(points2Draw, pointList);
        PointListDestroy(pointList);
        DLineList dLineList = NULL;
        SetLineColor(subwaySystem.trackList, Qt::black, dLineList);
        DLineListAppendList(lines2Draw, dLineList);
        DLineListDestroy(dLineList);
        isFirstStart = 0;
    }
    else
    {
        PointListEmpty(points2Draw);
        DLineListEmpty(lines2Draw);
        PointList pointList = NULL;
        SetPointColor(subwaySystem.stationList, Qt::yellow, pointList);
        PointListAppendList(points2Draw, pointList);
        PointListDestroy(pointList);
        DLineList dLineList = NULL;
        SetLineColor(subwaySystem.trackList, Qt::black, dLineList);
        DLineListAppendList(lines2Draw, dLineList);
        DLineListDestroy(dLineList);
    }
    QWidget::update();
}



void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(drawedPoints)
    {
        Station* ptr = IsStationAround(pos);
        if(ptr)
        {
            ShowStation(ptr, pos);
            return;
        }
    }
    if(drawedTracks)
    {
        Track* ptr = IsTrackAround(pos);
        if(ptr)
        {
            ShowTrack(ptr, pos);
            return;
        }
    }
    QString message ="(" + QString::number(pos.x(), 10) + "," + QString::number(pos.y(), 10) + ")";
    ui->statusBar->showMessage(message);
    qDebug() << pos.x() << pos.y() << "Hover";
}





void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    qDebug() << pos.x() << pos.y() << "Double";
    Station* ptr1 = IsStationAround(pos);
    Track* ptr2 = IsTrackAround(pos);
    if(ptr1)
    {
        P2StationListAppend(chosenStations, ptr1);
        ShowChosenStation();
        return;
    }
    if(ptr2)
    {
//        chosenTracks.append(ptr2);
        qDebug() << "tracks";
        P2TrackListAppend(chosenTracks, ptr2);
        if(ptr2->LineList->next == NULL)
        {
            P2LineListAppend(chosenLines, ptr2->LineList->p2Line);
            qDebug() << "lines";
            ShowChosenLines();
        }
        ShowChosenTracks();
        return;
    }
}

void MainWindow::on_saveFile_clicked()
{
    QString str =  dirPos + "data.dat";
    QString outputBufa;
    SaveSubwaySystem(str, subwaySystem, outputBufa);
}

void MainWindow:: on_loadFile_clicked()
{
    isFirstStart = 0;
    P2LineListEmpty(chosenLines);
    P2TrackListEmpty(chosenTracks);
    P2StationListEmpty(chosenStations);
    PointListEmpty(points2Draw);
    PointListEmpty(drawedPoints);
    DLineListEmpty(lines2Draw);
    DLineListEmpty(drawedTracks);
    SubwaySystemDestroy(subwaySystem);
    DestroyMap(map);
    qDebug() << "test";
    QString str = dirPos+ "data.dat";
    QString outputBufa;
    InitSubwaySystem(str, subwaySystem, outputBufa);
}

state PointListInit(PointList &pointList)
{
    if(pointList)
    {
        PointListEmpty(pointList);
    }
    return OK;
}

state PointListAppend(PointList &pointList, Point point)
{
    if(!pointList)
    {
        PointNode* p = new PointNode;
        if(!p)
        {
            return ERROR;
        }
        *p = point;
        p->next = NULL;
        pointList = p;
        return OK;
    }
    else{
        PointNode* p = pointList;
        while(p->next)
        {
            p = p->next;
        }
        PointNode* q = new PointNode;
        if(!q)
        {
            return ERROR;
        }
        *q = point;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state PointListAppendList(PointList &pointList1, PointList pointList2)
{
    PointNode* p = pointList2;
    while(p)
    {
        //qDebug() << p->station->name;
        PointListAppend(pointList1, *p);
        p = p->next;
    }
    return OK;
}

state PointListEmpty(PointList &pointList)
{
    PointNode* p = pointList;
    PointNode* q;
    while(p)
    {
        q = p->next;
        delete p;
        p = q;
    }
    pointList = NULL;
    return OK;

}

state PointListDestroy(PointList &pointList)
{
    return PointListEmpty(pointList);
}

state DLineListInit(DLineList &dLineList)
{
    if(dLineList)
    {
        DLineListEmpty(dLineList);
    }
    return OK;
}

state DLineListAppend(DLineList &dLineList, DLine dLine)
{
    if(!dLineList)
    {
        DLineNode* p = new DLineNode;
        if(!p)
        {
            return ERROR;
        }
        *p = dLine;
        p->next = NULL;
        dLineList = p;
        return OK;
    }
    else{
        DLineNode* p = dLineList;
        while(p->next)
        {
            p = p->next;
        }
        DLineNode* q = new DLineNode;
        if(!q)
        {
            return ERROR;
        }
        *q = dLine;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state DLineListAppendList(DLineList &dLineList1, DLineList dLineList2)
{
    DLineNode* p = dLineList2;
    while(p)
    {
        DLineListAppend(dLineList1, *p);
        p = p->next;
    }
    return OK;
}

state DLineListEmpty(DLineList &dLineList)
{
    DLineNode* p = dLineList;
    DLineNode* q;
    while(p)
    {
        q = p->next;
        delete p;
        p = q;
    }
    dLineList = NULL;
    return OK;

}

state DLineListDestroy(DLineList &dLineList)
{
    return DLineListEmpty(dLineList);
}

void MainWindow::on_updateDisplay_clicked()
{
    P2StationListEmpty(chosenStations);
    P2TrackListEmpty(chosenTracks);
    P2LineListEmpty(chosenLines);
    ShowChosenStation();
    ShowChosenTracks();
    ShowChosenLines();
}

void MainWindow::on_showLineStation_clicked()
{
    if(!chosenLines)
    {
        ui->outputTextEdit->setText("未选中任何线路!");
        return;
    }
    else{
        QString str;
        P2TrackNode* p = chosenLines->p2Line->inLineTrack;
        while(p)
        {
            str+= p->p2Track->s1->name + "--" + QString::number(p->p2Track->weight, 10).toUpper() + "--" + p->p2Track->s2->name + "\n";
            p = p->next;
        }
        ui->outputTextEdit->setText(str);
        ui->displayLabel3->clear();
    }
    P2LineListEmpty(chosenLines);
}

void MainWindow::on_getMinestTimePath_clicked()
{
    P2StationNode* p = chosenStations;
    if(!p || !p->next)
    {
        ui->outputTextEdit->setText("未选中足够的站点!");
        return;
    }
    else{
        DestroyMap(map);
        InitMap(subwaySystem, map);
        //ShowMap(map);
        Floyd_time(map);
        P2StationList path = NULL;
        int time = DisplayPath(map, p->p2Station, p->next->p2Station, path);
        ui->outputTextEdit->setText(QString::number(time, 10));
        P2StationNode* q = path;
        Point point;
        point.color = Qt::blue;
        point.station = p->p2Station;
        point.next = NULL;
        PointListAppend(points2Draw, point);
        point.station = p->next->p2Station;
        PointListAppend(points2Draw, point);
        while(q)
        {
            point.color = Qt::red;
            point.station = q->p2Station;
            PointListAppend(points2Draw, point);
            q = q->next;
        }
        P2StationListDestroy(path);
        ui->outputTextEdit->setText("最短所需时间为" + QString::number(time, 10));
        QWidget::update();
    }
    return;
}

void MainWindow::on_editMap_clicked()
{
    ui->groupBoxMap->show();
    //QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data.dat";
    QString str = dirPos + "data.dat";
    QFile file(str);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString text = stream.readAll();
        ui->textEditMap->setText(text);
    }
    file.close();
}

void MainWindow::on_confirmMap_clicked()
{
    //QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data1.dat";
    QString str = dirPos + "data.dat";
    QFile file(str);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << ui->textEditMap->toPlainText();
    }
    file.close();
    ui->groupBoxMap->hide();
}

void MainWindow::on_cancelMap_clicked()
{
    ui->groupBoxMap->hide();
}

void MainWindow::on_editAdmins_clicked()
{
    ui->groupBoxAdmins->show();
    //QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
    QString str = dirPos + "admins.dat";
    QFile file(str);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString text = stream.readAll();
        ui->textEditAdmins->setText(text);
    }
    file.close();
}

void MainWindow::on_cancelAdmins_clicked()
{
    ui->groupBoxAdmins->hide();
}

void MainWindow::on_confirmAdmins_clicked()
{
    //QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
    QString str = dirPos + "admins.dat";
    QFile file(str);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << ui->textEditAdmins->toPlainText();
    }
    file.close();
    ui->groupBoxAdmins->hide();
    ui->outputTextEdit->setText("新的管理员账户将在软件重启后投入使用。");
}

void MainWindow::on_showAllLineStation_clicked()
{
    QString str;
    if(!subwaySystem.lineList)
    {
        str = "不存在任何线路";
    }
    else{
        LineList p = subwaySystem.lineList;
        while(p)
        {
            str += p->name + "\n";
            P2TrackList q = p->inLineTrack;
            while(q)
            {
                str += q->p2Track->s1->name + "--" + QString::number(q->p2Track->weight) + "--" + q->p2Track->s2->name + "\n";
                q = q->next;
            }
            p=p->next;
        }
    }
    ui->outputTextEdit->setText(str);
}

void MainWindow::on_login_clicked()
{
    //int signal = 0;
    if(ui->inputLine1->text() == "" || ui->inputLine2->text() == "")
    {
        ui->outputTextEdit->setText("请在输入框1输入账号,输入框2中输入密码后，再次点击登录按钮。");
        return;
    }
    else
    {
        for(int i = 0; i < adminsNum; i++)
        {
            QString username = "@" + ui->inputLine1->text();
            QString password = ui->inputLine2->text();
            if(admins[i].username == username && admins[i].password == password)
            {
                ui->outputTextEdit->setText("欢迎回来！" + ui->inputLine1->text());
                ui->inputLine1->clear();
                ui->inputLine2->clear();
                ui->groupBoxAdminTools->show();
                return;
            }
        }
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        ui->outputTextEdit->setText("账号或密码错误!");
    }

}

void MainWindow::on_editStationName_clicked()
{
    if(!chosenStations)
    {
        ui->outputTextEdit->setText("未选中任何站点。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    if(ui->inputLine1->text() == "")
    {
        ui->outputTextEdit->setText("请在输入框1中输入选中的第一个站点的新名字。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    ui->outputTextEdit->setText("站点" + chosenStations->p2Station->name + "已改为" + ui->inputLine1->text());
    chosenStations->p2Station->name = ui->inputLine1->text();
    P2StationListEmpty(chosenStations);
    ShowChosenStation();
    //ui->outputTextEdit->setText();
    ui->inputLine1->clear();
    ui->inputLine2->clear();
}

void MainWindow::on_editStationPos_clicked()
{
    if(!chosenStations)
    {
        ui->outputTextEdit->setText("未选中任何站点。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    if(ui->inputLine1->text() == "" || ui->inputLine2->text() == "")
    {
        ui->outputTextEdit->setText("请在输入框1中输入选中的第一个站点的横坐标，在输入框2中输入选中的第一个站点的纵坐标，坐标值可参考状态栏输出的光标位置。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    bool ok;
    int x = ui->inputLine1->text().toInt(&ok, 10);
    int y = ui->inputLine2->text().toInt(&ok, 10);
    chosenStations->p2Station->pos.setX(x);
    chosenStations->p2Station->pos.setY(y);
    QWidget::update();
    ui->outputTextEdit->setText("站点" + chosenStations->p2Station->name + "的位置已改为" + "(" + QString::number(x, 10)+","+ QString::number(y, 10) + ")");
    P2StationListEmpty(chosenStations);
    ShowChosenStation();
    ui->inputLine1->clear();
    ui->inputLine2->clear();
}

void MainWindow::on_editLineName_clicked()
{
    if(!chosenLines)
    {
        ui->outputTextEdit->setText("未选中任何线路。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    if(ui->inputLine1->text() == "")
    {
        ui->outputTextEdit->setText("请在输入框1中输入选中的第一条线路的新名字。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    ui->outputTextEdit->setText("站点" + chosenLines->p2Line->name + "已改为" + ui->inputLine1->text());
    chosenLines->p2Line->name = ui->inputLine1->text();
    P2LineListEmpty(chosenLines);
    ShowChosenLines();
    //ui->outputTextEdit->setText();
    ui->inputLine1->clear();
    ui->inputLine2->clear();
}

void MainWindow::on_editTrackWeight_clicked()
{
    if(!chosenTracks)
    {
        ui->outputTextEdit->setText("未选中任何边。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    if(ui->inputLine1->text() == "")
    {
        ui->outputTextEdit->setText("请在输入框1中输入选中的第一条边的新权值。");
        ui->inputLine1->clear();
        ui->inputLine2->clear();
        return;
    }
    bool ok;
    int w = ui->inputLine1->text().toInt(&ok, 10);
    ui->outputTextEdit->setText(chosenTracks->p2Track->s1->name + "--" + QString::number(chosenTracks->p2Track->weight, 10) + "--" + chosenTracks->p2Track->s2->name + "的权值已改为" +  QString::number(w, 10));
    chosenTracks->p2Track->weight = w;
    P2TrackListEmpty(chosenTracks);
    ShowChosenTracks();
    //ui->outputTextEdit->setText();
    ui->inputLine1->clear();
    ui->inputLine2->clear();
}

void MainWindow::on_getMinestTransferPath_clicked()
{
    P2StationNode* p = chosenStations;
    if(!p || !p->next)
    {
        ui->outputTextEdit->setText("未选中足够的站点!");
        return;
    }
    else{
        DestroyMap(map);
        InitMap(subwaySystem, map);
        //ShowMap(map);
        Floyd_transfer(map);
        P2StationList path = NULL;
        DisplayPath(map, p->p2Station, p->next->p2Station, path);
        //ui->outputTextEdit->setText(QString::number(time, 10));
        P2StationNode* q = path;
        Point point;
        point.color = Qt::blue;
        point.station = p->p2Station;
        point.next = NULL;
        PointListAppend(points2Draw, point);
        point.station = p->next->p2Station;
        PointListAppend(points2Draw, point);
        while(q)
        {
            point.color = Qt::red;
            point.station = q->p2Station;
            PointListAppend(points2Draw, point);
            q = q->next;
        }
        P2StationListAppend(path, p->next->p2Station);
        P2StationList q2 = path;
        Station* q1 = p->p2Station;
        P2LineList list = NULL;
        QString str;
        while(q2)
        {
            TransferLine(q1, q2->p2Station, list);
            P2LineNode* ptr = list;
            str += "在" + q1->name + "搭乘";
            while(ptr)
            {
                str += " " + ptr->p2Line->name + " ";
                ptr = ptr->next;
            }
            str += "至" + q2->p2Station->name + "\n";
            q1 = q2->p2Station;
            q2 = q2->next;
            P2LineListEmpty(list);
        }

        ui->outputTextEdit->setText(str);
        P2StationListDestroy(path);
        QWidget::update();
    }
    return;
}
