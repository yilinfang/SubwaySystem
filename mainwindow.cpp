#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subwaystation.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1024,768);
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    //temp = "qt";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowStation(Station *station)
{
    ui->statusBar->showMessage(station->name);
}

void MainWindow::ShowTrack(Track *track)
{
    //ui->displayLabel->clear();
    //QFont font("Microsoft YaHei", 10, 75);
    //ui->displayLabel->setFont(font);
    ui->statusBar->showMessage(track->s1->name + "---" + track->s2->name);
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
    if(drawedPoints.length == 0)
        return NULL;
    for(int i = 0; i < drawedPoints.length - 1; i++)
    {
        long dis = (drawedPoints.arr[i].station->pos.x() - pos.x()) * (drawedPoints.arr[i].station->pos.x() - pos.x()) + (drawedPoints.arr[i].station->pos.y() - pos.y()) * (drawedPoints.arr[i].station->pos.y() - pos.y());
        if(dis <= 4)
        {
            return drawedPoints.arr[i].station;
        }
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
    if(drawedTracks.length == 0)
    {
        return NULL;
    }
    for(int i = 0; i < drawedTracks.length - 1; i++)
    {
        if((pos.x() - drawedTracks.arr[i].track->s1->pos.x()) * (drawedTracks.arr[i].track->s2->pos.y() - drawedTracks.arr[i].track->s1->pos.y()) == (drawedTracks.arr[i].track->s1->pos.x() - drawedTracks.arr[i].track->s2->pos.x()) * (pos.y() - drawedTracks.arr[i].track->s1->pos.y()))
        {
            if((pos.x() - drawedTracks.arr[i].track->s1->pos.x()) * (pos.x() - drawedTracks.arr[i].track->s2->pos.x()) <= 0 && (pos.y() - drawedTracks.arr[i].track->s1->pos.y()) * (pos.y() - drawedTracks.arr[i].track->s2->pos.y()) <= 0)
            {
                return  drawedTracks.arr[i].track;
            }
        }
    }
    return NULL;
}

void MainWindow::SetPointColor(StationList &stations, QColor color, PointList &list)
{
    if(stations.size != 0)
    {
        for(int i = 0; i < stations.length - 1; i++)
        {
            Point point;
            point.station = &stations.arr[i];
            point.color = color;
            PointListAppend(list, point);
        }
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
    if(tracks.length != 0)
    {
        for(int i = 0; i < tracks.length - 1; i++)
        {
            DLine line;
            line.track = &tracks.arr[i];
            line.color = color;
            DLineListAppend(list, line);
        }
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
    if(chosenStations.length == 0)
    {
        ui->displayLabel2->setText("未选中任何站点!");
    }
    else{
        QString str;
        for(int i = 0; i < chosenStations.length - 1; i++)
        {
            str += chosenStations.arr[i]->name;
            str += " ";
        }
        ui->displayLabel2->setText(str);
    }
}

void MainWindow::ShowChosenTracks()
{
    //ui->displayLabel3->setText(track->s1->name + "---" + track->s2->name);
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
    if(points2Draw.length != 0)
    {
        signal = 1;
        PointListEmpty(drawedPoints);
        PointListAppendList(drawedPoints, points2Draw);
        for(int i = 0; i < points2Draw.length - 1; i++)
        {
            painter.setPen(QPen(points2Draw.arr[i].color,10,Qt::SolidLine,Qt::RoundCap));
            painter.drawPoint(points2Draw.arr[i].station->pos);
        }
    }
    if(lines2Draw.length != 0)
    {
        signal = 1;
        DLineListEmpty(drawedTracks);
        DLineListAppendList(drawedTracks, lines2Draw);
        for(int i = 0; i < lines2Draw.length - 1; i++)
        {
            painter.setPen(QPen(lines2Draw.arr[i].color,5,Qt::SolidLine,Qt::RoundCap));
            painter.drawLine(lines2Draw.arr[i].track->s1->pos, lines2Draw.arr[i].track->s2->pos);
        }
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
    fileName = "data.dat";
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/";
    QString outputBufa;
    InitSubwaySystem(str + fileName, subwaySystem, outputBufa);
    qDebug() << outputBufa;
    fileName = "data.txt";
    SaveSubwaySystem(str + fileName, subwaySystem, outputBufa);
    PointList pointList;
    SetPointColor(subwaySystem.stationList, Qt::red, pointList);
    PointListAppendList(points2Draw, pointList);
    PointListDestroy(pointList);
    DLineList dLineList;
    SetLineColor(subwaySystem.trackList, Qt::blue, dLineList);
    DLineListAppendList(lines2Draw, dLineList);
    DLineListDestroy(dLineList);
    QWidget::update();
}



void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(drawedPoints.length != 0)
    {
        Station* ptr = IsStationAround(pos);
        if(ptr)
        {
            ShowStation(ptr);
            return;
        }
    }
    if(drawedTracks.length != 0)
    {
        Track* ptr = IsTrackAround(pos);
        if(ptr)
        {
            ShowTrack(ptr);
            return;
        }
    }
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
        P2TrackListAppend(chosenTracks, ptr2);
        ShowChosenTracks();
        return;
    }
}

void MainWindow::on_saveFile_clicked()
{
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data.txt";
    QString outputBufa;
    SaveSubwaySystem(str, subwaySystem, outputBufa);
}

void MainWindow::on_loadFile_clicked()
{
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data.dat";
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
        PointNode* p = (PointNode*)malloc(sizeof(PointNode));
        if(!p)
        {
            return ERROR;
        }
        p->point = point;
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
        PointNode* q = (PointNode*)malloc(sizeof(PointNode));
        if(!q)
        {
            return ERROR;
        }
        q->point = point;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state PointListAppendList(PointList &pointList1, PointList pointList2)
{
    if(!pointList1)
    {
        pointList1 = pointList2;
        return OK;
    }
    else{
        PointNode* p = pointList2;
        while(p)
        {
            PointListAppend(pointList1, p->point);
            p = p->next;
        }
        return OK;
    }
}

state PointListEmpty(PointList &pointList)
{
    PointNode* p = pointList;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
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
        DLineNode* p = (DLineNode*)malloc(sizeof(DLineNode));
        if(!p)
        {
            return ERROR;
        }
        p->dLine = dLine;
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
        DLineNode* q = (DLineNode*)malloc(sizeof(DLineNode));
        if(!q)
        {
            return ERROR;
        }
        q->dLine = dLine;
        q->next = NULL;
        p->next = q;
        return OK;
    }
}

state DLineListAppendList(DLineList &dLineList1, DLineList dLineList2)
{
    if(!dLineList1)
    {
        dLineList1 = dLineList2;
        return OK;
    }
    else{
        DLineNode* p = dLineList2;
        while(p)
        {
            DLineListAppend(dLineList1, p->dLine);
            p = p->next;
        }
        return OK;
    }
}

state DLineListEmpty(DLineList &dLineList)
{
    DLineNode* p = dLineList;
    while(p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    return OK;

}

state DLineListDestroy(DLineList &dLineList)
{
    return DLineListEmpty(dLineList);
}
