#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QPaintEvent>
#include <math.h>

namespace Ui {
class MainWindow;
class LCanvas ;
}
class Fly;

class Vector2{
public:
    Vector2(){

    }
    Vector2(float x,float y){
        this->x = x;
        this->y = y;
    }
    float x;
    float y;
    Vector2 operator + (Vector2 v){
        return Vector2(v.x + this->x,v.y+this->y);
    }
    Vector2 operator * (float f){
        return Vector2(this->x*f,this->y*f);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ConnectSlot();
    void InitUI();

private slots:
    void TestSlot2();
private:
    Ui::MainWindow *ui;
    QPushButton* btn;
};

class LCanvas : public QWidget{
    Q_OBJECT

public:
    LCanvas(QWidget *parent=0);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void LInit();
    int flyNum = 1000;
    Fly *flys[1000];
    int w;
    int h;
    int time;
    Vector2 pos;
};

#endif // MAINWINDOW_H
