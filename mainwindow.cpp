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


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //    // 设置画笔颜色
    if(!points2draw.isEmpty())
    {
        drawedPoints.clear();
        drawedPoints.append(points2draw);
        QList<Station>::iterator iter;
        painter.setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
        for(iter = points2draw.begin(); iter != points2draw.end(); iter++)
        {
            painter.drawPoint(iter->pos.x(), iter->pos.y());
        }
    }
    if(!lines2draw.isEmpty())
    {
        drawedTracks.clear();
        drawedTracks.append(lines2draw);
        painter.setPen(QPen(Qt::blue,5,Qt::SolidLine,Qt::RoundCap));
        QList<Track>::iterator iter;
        //qDebug() << lines2draw.size();
        for(iter = lines2draw.begin(); iter != lines2draw.end(); iter++)
        {
            painter.drawLine(iter->s1->pos, iter->s2->pos);
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
        painter.setFont(font);

        // 绘制文本
        painter.drawText(rect(), Qt::AlignCenter, "Thank you for using!!!");
    }
}


void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    //temp = str;
    fileName = "/Users/leo/Desktop/build-SubwaySystem-Desktop_Qt_5_9_4_clang_64bit-Debug/data.dat";
    str = fileName;
    qDebug() << str;
    QString outputBufa;
    InitSubwaySystem(str, subwaySystem, outputBufa);
    ui->pushButton->setText(outputBufa);
    points2draw.clear();
    points2draw.append(subwaySystem.stationTable);
    lines2draw.clear();
    lines2draw.append(subwaySystem.tracks);
    QWidget::update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(!drawedPoints.isEmpty())
    {
        QList<Station>::iterator iter;
        for(iter = drawedPoints.begin(); iter != drawedPoints.end(); iter++)
        {
            long dis = (iter->pos.x() - pos.x()) * (iter->pos.x() - pos.x()) + (iter->pos.y() - pos.y()) * (iter->pos.y() - pos.y());
            if(dis <= 4)
            {
                ShowStation(&(*iter));
                return;
            }
        }
    }
    if(!drawedTracks.isEmpty())
    {
        QList<Track>::iterator iter;
        for(iter = drawedTracks.begin(); iter != drawedTracks.end(); iter++)
        {
            if((pos.x() - iter->s1->pos.x()) * (iter->s2->pos.y() - iter->s1->pos.y()) == (iter->s1->pos.x() - iter->s2->pos.x()) * (pos.y() - iter->s1->pos.y())) //叉乘为0则三点共线
            {
                if((pos.x() - iter->s1->pos.x()) * (pos.x() - iter->s2->pos.x()) <= 0 && (pos.y() - iter->s1->pos.y()) * (pos.y() - iter->s2->pos.y()) <= 0) //如果pos在两点之间，则pos处在该条线段中
                {
                    ShowTrack(&(*iter));
                    return;
                }
            }
        }
    }
    qDebug() << pos.x() << pos.y() << "Hover";
}





void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    qDebug() << pos.x() << pos.y() << "Double";
}
