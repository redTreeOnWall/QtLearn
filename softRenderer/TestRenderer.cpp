
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

using namespace std;


class Line2D{
    public:
    float k;
    float b;
    bool isVerticle;
    float verticleX;
    void SetFromTwoPoint(float x1,float y1,float x2,float y2){

        //直线方程: y = k * x + b;
        // 解方程 k = (y1 - y2 ) /(x1 - x2)      b = y1 - b * x1;
        //左边直线方程的K 斜率 注意 , 垂直的情况 要特殊表示

        if(x1 == x2){
            this->isVerticle = true;
            this->verticleX = x1;
            return;
        }
        this -> isVerticle = false;
        //y = kx + b;
        this->k = (y1 -y2) / (x1 - x2);
        this->b = y1 - x1 * k;
    };

    float GetXFromY(float y){

        if(this -> isVerticle){
            return this->verticleX;
        }
        return (y - b) / k;
    };

};

class Color{
    public:
    Color(unsigned char r, unsigned char g, unsigned char b,unsigned char a){ 
        this -> r = r;
        this -> g = g;
        this -> b = b;
        this -> a = a;
    }

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color operator+(Color &colorb){
        Color c;
        c.r = r + colorb.r;
        c.g = g + colorb.g;
        c.b = b + colorb.b;
        c.a = a + colorb.a;
        return c;
    };


    Color operator-(Color &colorb){
        Color c;
        c.r = r - colorb.r;
        c.g = g - colorb.g;
        c.b = b - colorb.b;
        c.a = a - colorb.a;
        return c;
    };

    Color operator*(float x){
        Color c;
        c.r = r * x;
        c.g = g * x;
        c.b = b * x;
        c.a = a * x;
        return c;
    };

};

class Vector3{
    public:
    Vector3(){};
    Vector3(float x, float y,float z){
        this -> x = x;
        this -> y = y;
        this -> z = z;
    };
    float x;
    float y;
    float z;
};

class Vertx{
    public:
    Vector3 position;
    Color color;
};


class RenderBuff{
    public :
    int w;
    int h;
    Color *data;
    RenderBuff(int w,int h){
        this -> w = w;
        this -> h = h;
        this -> data = new Color[w * h];
    };

    void clear(){
        int count = w * h;
        for(int i = 0 ; i < count ; i++ ){
            data[i].r=0;
            data[i].g=0;
            data[i].b=0;
            data[i].a=0;
        }
    }

    ~RenderBuff(){
        delete[] this -> data;
    }


};

// r 0f ~ 1f
Color LerpColor(Color &col0 , Color &col1 , float x){
    Color col3 = (col1 - col0) * x;
    col3 = col3 + col0;
    return col3;
};

void RenderToShell(RenderBuff *buff){

    cout << "\033[2J\033[1;1H";
    //y倒过来输出
    for(int  y = buff -> h -1 ; y >=0 ; y-- ){
        cout << endl;
        for(int  x = 0 ; x < buff -> w; x++ ){
            int index = y * buff -> w + x;
            Color col = buff->data[index];

            char c =48 + 9 * (col.a / 255.0f);
            if(c == 48){
                c = '_';
                cout << c ;
                cout << c ;
            }else{
                cout << c ;
                cout << ']' ;
            }

            // if(col.a == 0){
            //     cout << "__" ;
            // }else{
            //     cout << "口" ;
            // }
            // cout.flush();
            // usleep(10 * 1000);
        }
    }
    cout << endl;
}

void DrawPixel(RenderBuff *buff,int x,int y, Color color){

    //超出屏幕的点被舍弃
    if(x <0 || x >= buff->w || y <0 || y >= buff->h){
        return ;
    }

    int index = y * buff -> w + x;
    (buff -> data) [index] = color;
}



void DrawTra(RenderBuff *buff, Vertx *v1,Vertx *v2,Vertx *v3){
    // Vertx *vTop1 = v1;
    // Vertx *vTop2 = v2;
    // Vertx *vTop3 = v3;

    //排序
    if(v1->position.y < v2->position.y){
        swap(v1,v2);
    }

    if(v1->position.y < v3->position.y){
        swap(v1,v3);
    }

    if(v2->position.y < v3->position.y){
        swap(v2,v3);
    }

    int yTopPixel = (v1->position.y + 0.5f )  * buff->h;
    int yBottomPixel = (v3->position.y + 0.5f)  * buff->h;


        /**
         * 
         * 
         *                 1
         *                /|                                                                                               
         *               / |  up                                                                                        
         *              /  |
         *           2 /___|
         *             \   |                                                                              
         *              \  | down                                                                     
         *               \ |           
         *                \|     
         *                 3
         */

    // 1 2
    Line2D line12;
    line12.SetFromTwoPoint(v1->position.x , v1->position.y,v2->position.x, v2->position.y);

    // 1 3
    Line2D line13;
    line13.SetFromTwoPoint(v1->position.x , v1->position.y,v3->position.x, v3->position.y);

    // 2 3
    Line2D line23;
    line23.SetFromTwoPoint(v2->position.x , v2->position.y,v3->position.x, v3->position.y);


    int startLine = yTopPixel;
    int endLine = yBottomPixel;
    /*
    if(!isUpTran){
        // 倒三角形的扫描从三角形的"底边"开始
        startLine = yBottomPixel;
        endLine = yTopPixel;
    }
    */

    // TODO: 屏幕内裁剪

    if(startLine > buff->h - 1){
        startLine = buff->h -1;
    }

    if(endLine < 0){
        endLine = 0;
    }

    for(int row = startLine ; row > endLine ; row-- ){

        float y = ((float)row / buff->h) - 0.5f ;
        bool isTopShape =  y >= v2->position.y;

        float leftX;
        float rightX;

        Color leftColor;
        Color rightColor;

        if(isTopShape){
            //use ling 1_3 1_2
            leftX = line12.GetXFromY(y);
            rightX = line13.GetXFromY(y);


            if(v1->position.y == v2->position.y){
                leftColor = v2->color;
                rightColor = v1->color;
            }else{
                float r = (y - v2->position.y) /( v1->position.y - v2->position.y);
                leftColor = LerpColor(v2->color,v1->color,r);
                r = (y - v3->position.y) /( v1->position.y - v3->position.y);
                rightColor = LerpColor(v3->color,v1->color,r);
            }

        }else{
            //use ling 1_3 2_3
            leftX = line23.GetXFromY(y);
            rightX = line13.GetXFromY(y);


            if(v3->position.y == v2->position.y){
                leftColor = v2->color;
                rightColor = v3->color;
            }else{
                float r = (y - v3->position.y) /( v2->position.y - v3->position.y);
                leftColor = LerpColor(v3->color,v2->color,r);
                r = (y - v3->position.y) /( v1->position.y - v3->position.y);
                rightColor = LerpColor(v3->color,v1->color,r);
            }
        }

        if(leftX > rightX){
            swap(leftX,rightX);
            swap(leftColor,rightColor);
        }

        int leftIntX = (leftX + 0.5f) * buff->w;
        int rightIntX = (rightX + 0.5f) * buff -> w;

        //裁剪
        if(leftIntX < 0){
            leftIntX = 0;
        }
        if(rightIntX > buff->w-1){
            rightIntX = buff->w -1;
        }


        float stepColorR;
        if(leftIntX == rightIntX){
            stepColorR = 0;
        }else{
            stepColorR = 1.0f / (rightIntX - leftIntX);
        }

        for(int x = leftIntX  ; x <= rightIntX ; x ++){
            //fragment shader
            float r = stepColorR * (x - leftIntX);
            Color c = LerpColor(leftColor,rightColor,r);
            // c = Color(0.0,0.0,0.0,100 + 100 *  isTopShape );
            // Color c = v1->color;
            DrawPixel(buff,x,row,c);
        }
    }
    //end

}


float GetRandomPos(){ 
    float r = random() % 1000 / 1000.0f;
    return  r  - 0.5f;
};

void rotatePos(Vector3 *v3,float eler){
    float r = atan2(v3->y,v3->x);
    float l =pow( pow(v3->x,2) + pow(v3->y,2) , 0.5);
    r = r + eler;
    v3->y =l * sin(r);
    v3->x =l * cos(r);
};


int main(int argc, char const *argv[]) {

    // Color &col1 = Color(0,0,0,255);
    // Color &col2 = Color(0,0,0,0);
    // for(float i = 0.0f ; i < 1.0f;i = i + 0.1f){
    //     Color c = LerpColor(col1,col2, i);
    //     cout << c.a << endl;;
    // };

    return 0;

    //Esc[Line;Columnf
    cout  << "soft renderer :" << endl;

    char esc = 0x1b;
    // cout << esc << "["<< 1 <<  ";" << 2 ;

    // cout << "\033[2J\033[1;1H";

    /*

    cout << "\033[2J\033[1;1H";

    These are ANSI escape codes. The first one (\033[2J) clears the entire screen (J) from top to bottom (2). The second code (\033[1;1H) positions the cursor at row 1, column 1.

    All ANSI escapes begin with the sequence ESC[, have zero or more parameters delimited by ;, and end with a command letter (J and H in your case). \033 is the C-style octal sequence for the escape character.

     */

    int screenW = 40;
    int screenH = 40;

    RenderBuff *buff = new RenderBuff(screenW,screenH);

    // DrawPixel(buff,16,8 , Color(255,255,255,255));

    Vertx *v1 = new Vertx();
    v1->color = Color(0,0,0,80);
    // v1->color = Color(0,0,0,255);


    Vertx *v2 = new Vertx();
    v2->color = Color(0,0,0,160);
    // v2->color = Color(0,0,0,255);


    Vertx *v3 = new Vertx();
    v3->color = Color(0,0,0,255);

    // v1->position = Vector3(GetRandomPos(),GetRandomPos(),0.0f);
    // v2->position = Vector3(GetRandomPos(),GetRandomPos(),0.0f);
    // v3->position = Vector3(GetRandomPos(),GetRandomPos(),0.0f);

    v1->position = Vector3(0.0,0.3,0.0);
    v2->position = Vector3(-0.3,0.0,0.0);
    v3->position = Vector3(0.3,-0.3,0.0);


    for(int i = 0 ; i < 1 ; i++){

        // v1->position.x -= 0.01;
        // v2->position.x -= 0.01;
        // v3->position.x -= 0.01;

        float r =  0.1f;
        // rotatePos(&(v1->position),r);
        // rotatePos(&(v2->position),r);
        // rotatePos(&(v3->position),r);

        buff -> clear();
        DrawTra(buff,v1,v2,v3);
        RenderToShell(buff);
        // sleep(1);
        usleep(1000 * 100);
    }






    delete buff;

}




