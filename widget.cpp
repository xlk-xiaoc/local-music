#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QLabel>
#include<QFileDialog>
#include <QPalette>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("xiaoc");
    setFixedSize(400,300);
    setAttribute(Qt::WA_TranslucentBackground, false);
    //绘图
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/u=3208516513,310800692&fm=26&gp=0.jpg")));
    setPalette(pal);
    init();//初始化
    connect(myt,&QTimer::timeout,[=]
    {
      static int k=0;
      k++;
      if(k==1)
      {
          myt->stop();
          this->close();
      }
    });
    connect(ui->volumeSlider,&QSlider::valueChanged,[=]
    {
        int k=ui->volumeSlider->value();
        musicPlayer->setVolume(k);
    });//设置音量
    connect(musicPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(dealm(qint64)));//处理进度条
    connect(&jd,&QTimer::timeout,[=]{
        int m=ui->horizontalSlider->value();
        int w=ui->horizontalSlider->maximum();
        if(m+1!=w)
        {
            ui->horizontalSlider->setValue(m+1);
        }
        else {
            jd.stop();
        }

    });
    connect(df,&QTimer::timeout,[=]
    {
        int p=playlist->currentIndex();
        ui->tableWidget->selectRow(p);
    });
}
//进度条的处理
void Widget::dealm( qint64 playtime)
{
    playtime=musicPlayer->duration();
    int s=playtime/1000;
    ui->horizontalSlider->setMaximum(s);
    ui->horizontalSlider->setMinimum(0);
    jd.start(1000);
    emit jingdu();
}
//初始化
void Widget::init()
{
    myt=new QTimer(this);
    df=new QTimer(this);
    playlist=new QMediaPlaylist(this);
    musicPlayer = new QMediaPlayer(this);
    ui->volumeSlider->setMaximum(100);
    ui->volumeSlider->setMinimum(0);
    sendtable();
}
Widget::~Widget()
{
    delete ui;
}
//将信息打印在窗口上
void Widget::sendtable()
{
    ret=getFileNames("E:\music");
    for(auto p: ret)
    {
        int k= ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(k);
        ui->tableWidget->setItem(k,0,new QTableWidgetItem(QString(p)));
        QString absDir = QString("E:\\music\\%1").arg(p);
        playlist->addMedia(QUrl::fromLocalFile(absDir));
    }
}
//播放函数
void Widget::playmusic(int k,int p)
{
    //absDir.replace(QString("/"), QString("\\"));/*替换为windown的目录格式*/
   // musicPlayer = new QMediaPlayer();
    //musicPlayer->setMedia(QUrl::fromLocalFile(absDir));
    musicPlayer->setPlaylist(playlist);
    int temp=playlist->currentIndex();
    ui->tableWidget->selectRow(temp);
    musicPlayer->setVolume(50);
    ui->volumeSlider->setValue(50);
    musicPlayer->setPosition(p);
    musicPlayer->play();
    setpostion();
}
//button 播放按钮
void Widget::on_buttonplay_clicked()
{
    QString p=ui->buttonplay->text();
    if(p.toUtf8()=="play")
    {
        //QString runPath = QCoreApplication::applicationDirPath();//获取当前路径
       //QString temp=ui->tableWidget->verticalHeaderItem(t)->text();
       // QString absDir = QString("E:\\music\\%1").arg(temp);
        int n=musicPlayer->position();
        playmusic(t,n);
        ui->buttonplay->setText("stop");

    }
    else {
        musicPlayer->pause();
        ui->buttonplay->setText("play");
    }
}
//获取指定文件夹下的歌曲
QStringList Widget::getFileNames(const QString &path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.mp3";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}
//下一首歌曲
void Widget::on_buttonright_clicked()
{
    if(playlist->mediaCount()-playlist->currentIndex()!=1)//这里注意的是，和我们正常数数一样，mediaCount是从1开始计数的
    {
        //myPlayer->setMedia(playList->media(playList->nextIndex()));
        playlist->setCurrentIndex(playlist->nextIndex());
        musicPlayer->play();
        df->start(100);

    }
    else
    {
        playlist->setCurrentIndex(0);
       // myPlayer->setMedia(playList->media(0));
        musicPlayer->play();
    }
    int temp=playlist->currentIndex();
    ui->tableWidget->selectRow(temp);

}
//上一首歌曲
void Widget::on_buttonleft_clicked()
{
    if(playlist->currentIndex()!=0)//这里注意的是，和我们正常数数一样，mediaCount是从1开始计数的
    {
        //myPlayer->setMedia(playList->media(playList->nextIndex()));
        playlist->setCurrentIndex(playlist->previousIndex());
        musicPlayer->play();
    }
    else
    {
        playlist->setCurrentIndex(playlist->mediaCount()-1);
       // myPlayer->setMedia(playList->media(0));
        musicPlayer->play();
    }
    int temp=playlist->currentIndex();
    ui->tableWidget->selectRow(temp);
}
//设置进度条
void Widget::setpostion()
{
    qint64 len=musicPlayer->position();
    ui->horizontalSlider->setMaximum(musicPlayer->duration());
    ui->horizontalSlider->setSliderPosition(len);
}
//定时器
void Widget::on_buttontime_clicked()
{
    QMessageBox::information(this,"sure","reboot");
    if(myt->isActive()==false)
    myt->start(60000);//一分钟一个信号
}
//增加音量
void Widget::on_toolButton_clicked()
{
    int k=ui->volumeSlider->value();
    if(k+5<=100)
    {
        ui->volumeSlider->setValue(k+5);//设置值
        musicPlayer->setVolume(k+5);
    }
}
//降低音量
void Widget::on_toolButton_2_clicked()
{
    int k=ui->volumeSlider->value();
    if(k-5>=0)
    {
        ui->volumeSlider->setValue(k-5);
        musicPlayer->setVolume(k-5);
    }
}
//添加音乐文件信息
void Widget::on_pushButton_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"open","../");
    //获取文件信息
    filename="";
    QFileInfo info(path);
    filename=info.fileName();
    path= info.path();
    path.replace(QString("/"), QString("\\"));/*替换为windown的目录格式*/
    int k= ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(k);
    ui->tableWidget->setItem(k,0,new QTableWidgetItem(QString(filename)));
    QString absDir = QString("%1\\%2").arg(path).arg(filename);
    playlist->addMedia(QUrl::fromLocalFile(absDir));

}
//关机
void Widget::on_pushButton_2_clicked()
{
    QProcess pre;
    pre.start("shutdown -s -t 10");
    QMessageBox::information(this,"shutdown","shutdown");
    pre.waitForStarted();
    pre.waitForFinished();
    close();
}
