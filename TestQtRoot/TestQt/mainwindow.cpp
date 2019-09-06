#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QPainter>
#include <math.h>
#include <cstdlib>
#include <QTextStream>
QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);
QTextStream cerr(stderr, QIODevice::WriteOnly);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) //, ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
    InitUI();
    setWindowTitle("canvasTest");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: TestSlot2(){
    qDebug() << ("test");
    qDebug("test2");
}

void MainWindow::InitUI(){

    this->setFixedSize(800,600);

    this->btn = new QPushButton("testBtn");
    QHBoxLayout *lay  = new QHBoxLayout;
    lay -> addWidget(btn);
    connect(btn,SIGNAL(clicked()),this,SLOT(TestSlot2()));
    QHBoxLayout *lay2  = new QHBoxLayout;

    LCanvas *c = new LCanvas();
    lay2->addWidget(c);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay -> addLayout(lay);
    vlay -> addLayout(lay2);
    QWidget *p =  new QWidget();
    this -> setCentralWidget(p);
    p -> setLayout(vlay);

    qDebug("init uis");
}



class Fly{
public:
    Fly(LCanvas *c){
        this->canvas = c;
        int r = rand() % 255 ;
        int g = rand() % 255 ;
        int b = rand() % 255 ;
        this->color = new QColor(r,g,b);
        this->startTime = c->time;
        this->speed =30 + ( rand() % 30);
    }
    LCanvas *canvas;
    ~Fly(){
        delete this->color;
        this->color =0;
        qDebug("fly died\n");
    }
    Vector2 pos = Vector2(0,0);
    float w = 20;
    float h = 30;
    float aimDir;
    float lifeTime;
    int startTime;
    float speed;
    QColor *color;
    void paint(QPainter *p){
        QRect r(pos.x,pos.y,w,h);
        p->fillRect(r,QBrush(*color));
    }
    void update(){
        int thisTime = this->canvas->time - this-> startTime;
        float one  = 1000;
        float t = thisTime / one;
        Vector2 dv = Vector2(
                    speed * t *cos(this->aimDir),
                    speed * t *sin(this->aimDir)
                    );
        this->pos = this->pos + dv;
        if(thisTime>=lifeTime){
            this->pos = Vector2(canvas->pos);
            this->startTime = this->canvas->time;
        }
    }
};



LCanvas::LCanvas(QWidget *parent) : QWidget (parent){
    this->w = 800;
    this->h = 500;
    pos = Vector2(w/2,h/2);

    this -> setFixedSize(this -> w,this -> h);
    this -> startTimer(25);
    for(int i =0;i< this ->flyNum;i++){
        Fly *f = new Fly(this);
        f =  new Fly(this);
        f->w = 20 + rand()%50;
        f->h = f->w;
        f->aimDir =  rand() ;
        f->lifeTime = 1000 + (rand() %1000);
        f->pos =pos ;
        this->flys[i] = f;
    }
}
void LCanvas::timerEvent(QTimerEvent *e){
    this->time = this ->time + 25;
    update();
}
void LCanvas::paintEvent(QPaintEvent *){
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing,true);
    p.setPen(QColor(0,160,0));
    float l = 50;
    float t =5 * (float)time /1000;
    float y =h/2 + l * sin(t);
    float x =w /2 + l * cos(t);
    p.drawLine(QPointF(w/2,h/2),QPointF(x,y));
    for(int i = 0;i<this->flyNum;i++){
        this->flys[i] -> update() ;
    }
    for(int i = 0;i<this->flyNum;i++){
        this->flys[i] ->paint(&p);
    }
    qDebug() << this->time;

}


