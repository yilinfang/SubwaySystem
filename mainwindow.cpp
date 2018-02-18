#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subwaystation.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1024,768);
    points2draw.clear();
    drawedPoints.clear();
    lines2draw.clear();
    drawedTracks.clear();
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
    if(drawedPoints.isEmpty())
        return NULL;
    QList<Point>::iterator iter;
    for(iter = drawedPoints.begin(); iter != drawedPoints.end(); iter++)
    {
        long dis = (iter->station->pos.x() - pos.x()) * (iter->station->pos.x() - pos.x()) + (iter->station->pos.y() - pos.y()) * (iter->station->pos.y() - pos.y());
        if(dis <= 4)
        {
            return iter->station;
        }
    }
    return NULL;
}

Track *MainWindow::IsTrackAround(QPoint pos)
{
    if(drawedTracks.isEmpty())
        return NULL;
    QList<DLine>::iterator iter;
    for(iter = drawedTracks.begin(); iter != drawedTracks.end(); iter++)
    {
        if((pos.x() - iter->track->s1->pos.x()) * (iter->track->s2->pos.y() - iter->track->s1->pos.y()) == (iter->track->s1->pos.x() - iter->track->s2->pos.x()) * (pos.y() - iter->track->s1->pos.y())) //叉乘为0则三点共线
        {
            if((pos.x() - iter->track->s1->pos.x()) * (pos.x() - iter->track->s2->pos.x()) <= 0 && (pos.y() - iter->track->s1->pos.y()) * (pos.y() - iter->track->s2->pos.y()) <= 0) //如果pos在两点之间，则pos处在该条线段中
            {
                return iter->track;
            }
        }
    }
    return NULL;
}

QList<Point> MainWindow::SetPointColor(QList<Station> &stations, QColor color)
{
    QList<Point> list;
    list.clear();
    if(!stations.isEmpty())
    {
        QList<Station>::iterator iter;
        for(iter = stations.begin(); iter != stations.end(); iter++)
        {
            Point point;
            point.station = &(*iter);
            point.color = color;
            list.append(point);
        }
    }
    return list;
}

QList<DLine> MainWindow::SetLineColor(QList<Track> &tracks, QColor color)
{
    QList<DLine> list;
    list.clear();
    if(!tracks.isEmpty())
    {
        QList<Track>::iterator iter;
        for(iter = tracks.begin(); iter != tracks.end(); iter++)
        {
            DLine dLine;
            dLine.track = &(*iter);
            dLine.color = color;
            list.append(dLine);
        }
    }
    return list;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //qDebug()<<"AAA";
    //    // 设置画笔颜色
    if(!points2draw.isEmpty())
    {
        drawedPoints.clear();
        drawedPoints.append(points2draw);
        QList<Point>::iterator iter;
        for(iter = points2draw.begin(); iter != points2draw.end(); iter++)
        {
            painter.setPen(QPen(iter->color, 10, Qt::SolidLine, Qt::RoundCap));
            //qDebug() << iter->station->name;
            painter.drawPoint(iter->station->pos.x(), iter->station->pos.y());
        }
    }
    if(!lines2draw.isEmpty())
    {
        drawedTracks.clear();
        drawedTracks.append(lines2draw);
        painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
        QList<DLine>::iterator iter;
        //qDebug() << lines2draw.size();
        for(iter = lines2draw.begin(); iter != lines2draw.end(); iter++)
        {
            painter.setPen(QPen(iter->color,5,Qt::SolidLine,Qt::RoundCap));
            painter.drawLine(iter->track->s1->pos, iter->track->s2->pos);
            //painter.drawLine(iter->s1->pos.x(),iter->s1->pos.y(),iter->s2->pos.x(),iter->s2->pos.y());
            //qDebug() << iter->s1->pos.x() << iter->s1->pos.y() << iter->s2->pos.x() << iter->s2->pos.y();
        }
    }
    else
    {
        // 设置字体：微软雅黑、点大小50、斜体
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(50);
        font.setItalic(true);
        painter.setPen(Qt::blue);
        painter.setFont(font);

        // 绘制文本
        painter.drawText(rect(), Qt::AlignCenter, "Thank you for using!!!");
    }
}


void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    //temp = str;
    fileName = "data.dat";
    str = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/";
    qDebug() << str;
    QString outputBufa;
    InitSubwaySystem(str + fileName, subwaySystem, outputBufa);
    ui->pushButton->setText(outputBufa);
    fileName = "data.txt";
    SaveSubwaySystem(str + fileName, subwaySystem, outputBufa);
    points2draw.clear();
    //points2draw.append();
    points2draw.append(SetPointColor(subwaySystem.stationTable, Qt::blue));
    lines2draw.clear();
    lines2draw.append(SetLineColor(subwaySystem.tracks, Qt::red));
    //lines2draw.append(subwaySystem.tracks);
    QWidget::update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(!drawedPoints.isEmpty())
    {
        Station* ptr = IsStationAround(pos);
        if(ptr)
        {
            ShowStation(ptr);
            return;
        }
    }
    if(!drawedTracks.isEmpty())
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
}
