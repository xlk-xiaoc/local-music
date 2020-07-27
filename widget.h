#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QDir>
#include <QStringList>
#include <QMediaPlaylist>
#include <QTimer>
#include <QFile>
#include <QFileInfo>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    int t=0;
    QTimer jd;
    QString filename;
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QStringList getFileNames(const QString& path);

    void sendtable();

    void init();

    void playmusic(int,int);

    void setpostion();

    void dealjingdu();


private slots:

    void on_buttonplay_clicked();

    void on_buttonright_clicked();

    void on_buttonleft_clicked();


    void on_buttontime_clicked();


    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void dealm(qint64 );


    void on_pushButton_clicked();

    void dealdf();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
     QMediaPlayer *musicPlayer;
     QMediaPlaylist* playlist;
     QStringList ret;
     QTimer* myt;
     QTimer* df;
signals:
     void mysignal();
     void durationChanged(qint64);
     void jingdu();
};
#endif // WIDGET_H
