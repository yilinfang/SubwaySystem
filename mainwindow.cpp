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
    LoadAdmins();
    //ui->horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
    points2Draw = NULL;
    drawedPoints = NULL;
    lines2Draw = NULL;
    drawedTracks = NULL;
    chosenLines = NULL;
    subwaySystem.lineList = NULL;
    subwaySystem.trackList = NULL;
    subwaySystem.stationList = NULL;
    subwaySystem.edgeNum = 0;
    subwaySystem.lineNum = 0;
    subwaySystem.stationNum = 0;
    //temp = "qt";
}

MainWindow::~MainWindow()
{
    SubwaySystemDestroy(subwaySystem);
    //DestroyMap(map);
    PointListDestroy(points2Draw);
    PointListDestroy(drawedPoints);
    DLineListDestroy(lines2Draw);
    DLineListDestroy(drawedTracks);
    delete ui;
}

void MainWindow::ShowStation(Station *station)
{
    QString str;
    str += station->name;
    str += " ";
    P2LineNode* p = station->transferLines;
    while(p)
    {
        str += p->p2Line->name;
        str += " ";
        p = p->next;
    }
    ui->statusBar->showMessage(str);
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
        if(dis <= 4)
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
        if((pos.x() - p->track->s1->pos.x()) * (p->track->s2->pos.y() - p->track->s1->pos.y())  == (p->track->s1->pos.x() - p->track->s1->pos.x()) * (pos.y() - p->track->s1->pos.y()))
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
        ui->displayLabel2->setText("未选中任何站点!");
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
        ui->displayLabel1->setText("未选中任何边!");
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
        ui->displayLabel3->setText("未选中任何边!");
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

void MainWindow::ShowPath_minestTime(P2StationList path)
{
    if(!path)
    {
            //ui->displayLabelHints->setText("不存在路径!");
        return;
    }
    else
    {
        PointListEmpty(points2Draw);
        DLineListEmpty(drawedTracks);
        PointList pList = NULL;
        DLineList dList = NULL;
        SetPointColor(subwaySystem.stationList, Qt::black, pList);
        //SetLineColor(subwaySystem.stationList, Qt::yellow, dList);
        //SetDLineColor(subwaySystem.stationList, Qt::yellow, dList);
        P2StationNode* p = path;
//        QString str1;
//        while(p && p->next)
//        {

//            str += p->p2Station->name;
//        }
    }
}

void MainWindow::LoadAdmins()
{
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
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
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
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
    fileName = "data.dat";
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/";
    QString outputBufa;
    InitSubwaySystem(str + fileName, subwaySystem, outputBufa);
    qDebug() << outputBufa;
    fileName = "data.txt";
    SaveSubwaySystem(str + fileName, subwaySystem, outputBufa);
    //qDebug() << 0;
    PointList pointList = NULL;
    SetPointColor(subwaySystem.stationList, Qt::black, pointList);
    //qDebug() << pointList;
    PointListAppendList(points2Draw, pointList);
    PointListDestroy(pointList);
    DLineList dLineList = NULL;
    SetLineColor(subwaySystem.trackList, Qt::yellow, dLineList);
    DLineListAppendList(lines2Draw, dLineList);
    DLineListDestroy(dLineList);
    InitMap(subwaySystem, map);
    ShowMap(map);
    //qDebug() << 3;
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
            ShowStation(ptr);
            return;
        }
    }
    if(drawedTracks)
    {
        Track* ptr = IsTrackAround(pos);
        if(ptr)
        {
            ShowTrack(ptr);
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
        ui->displayLabel3->setText(str);
    }
    P2LineListEmpty(chosenLines);
}

void MainWindow::on_getMinestTimePath_clicked()
{
    P2StationNode* p = chosenStations;
    if(!p || !p->next)
    {
        //ui->displayLabelHints->setText("未选中足够的站点!");
        return;
    }
    return;
}

void MainWindow::on_editMap_clicked()
{
    ui->groupBoxMap->show();
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data.dat";
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
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data1.dat";
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
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
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
    QString str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/admins.dat";
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

    }
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
