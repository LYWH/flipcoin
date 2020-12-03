#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include<QSound>
#include"mainscene.h"
#include "ui_mainscene.h"

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //extern bool levelWinArr[20];
    //配置选择关卡场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar*bar=menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu *startMenu=bar->addMenu("开始");

    //创建退出 菜单项

    QAction * quitAction =startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //选择关卡按钮音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);

    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);


    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png" , ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width() , this->height() - backBtn->height());
    QLabel *label =new QLabel;
    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        //qDebug()<<"点击了返回按钮";
        //告诉主场景 我返回了 主场景监听ChooseLevelScene的返回按钮
        //延时返回
        //播放返回按钮音效
        backSound->play();

        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });

    });

    //创建选择关卡的按钮
    for( int i = 0 ; i < 20 ;i ++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move( 25 + i%4 * 70 , 130 + i/4 * 70  );
        menuBtn->indexNum = i+1;

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            if(judgePlanceIsavailabel(menuBtn->indexNum)){
                //播放选择关卡音效
                chooseSound->play();

                QString str = QString("您选择的是第 %1 关 ").arg( i + 1);
                qDebug() <<str;

                //进入到游戏场景
                this->hide(); //将选关场景隐藏掉
                play = new PlayScene(this,i+1); //创建游戏场景
                //设置游戏场景的初始位置
                play->setGeometry(this->geometry());
                play->show();//显示游戏场景
            }
            else {
                qDebug()<<"xieshangtishiyonghudeneirong";
                //QLabel *label =new QLabel;
                label->setParent(this);
                QFont font;
                font.setFamily("华文新魏");
                font.setPointSize(13);
                QString str1=QString("前面存在未成功的关卡");
                //将字体设置到标签控件中
                label->setFont(font);
                label->setText(str1);
                label->setGeometry(30,this->height()-80,170,50);
                label->setStyleSheet("color:red;");
                label->show();
                QTimer::singleShot(5000,this,[=](){
                    label->hide();
                });
            }
            connect(play,&PlayScene::chooseSceneBack,[=](int levelIndex,bool isWin){
                this->setGeometry(play->geometry());
                this->show();
                play->hide();
                play = NULL;
                if(isWin){
                    levelWinArr[levelIndex-1]=true;
                    QPixmap pix;
                    pix.load(":/res/Coin0001.png");
                    menuBtn->setIcon(pix);
                }
                delete play;
                label->hide();
                qDebug()<<"back to playscane";
                qDebug()<<levelIndex;
                qDebug()<<isWin;
            });
        });

        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 + i%4 * 70 , 130 + i/4 * 70 );

        //设置 label上的文字对齐方式 水平居中和 垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透   51号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }



}

bool ChooseLevelScene::judgePlanceIsavailabel(int indexnum){
    qDebug()<<"choose";
    qDebug()<<indexnum;
    for(int i = 0; i <indexnum-1;i++){
        if(!levelWinArr[i]){
            return false;
        }
    }
    return true;
}


void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
