#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <QDebug>
#include <QTimer> // 定时器所在库
#include <QSound> // 多媒体模块下的 音效头文件

#include "mypushbutton.h"

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 配置主场景

    // 设置固定大小
    this->setFixedSize(320, 588);

    // 设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置窗口标题
    this->setWindowTitle("翻金币主场景");

    //退出按钮的实现
    connect(ui->actionquit, &QAction::triggered, [=](){
        this->close();
    });

    // 准备开始按钮的音效
    QSound *startSound = new QSound(":/res/TapButtonSound.wav",this);

//    startSound->setLoops(10); // 设置循环播放的次数，参数为 -1 时为无限循环
//    startSound->play();

    // 开始按钮
    // 利用构造函数，定义一个 传了第一个参数 的自定义按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    // 把自定义的按钮绑定到主界面上
    startBtn->setParent(this);
    // 把按钮移动到界面中间
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);

    //监听点击事件，执行特效
    connect(startBtn,&MyPushButton::clicked,[=](){
        startSound->play(); // 播放开始按钮的音效资源

        // zoom的操作对象是MyPushButton，所以要在MyPushButton类中定义函数
        startBtn->zoom1(); //向下跳跃
        startBtn->zoom2(); //向上跳跃，事实上只使用zoom2就能够满足需求
//        startBtn->zoom(0, 10);
//        startBtn->zoom(10, 0);

        //延时0.5秒后 进入选择场景
        QTimer::singleShot(50, this,[=](){
            // 设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());

            this->hide();
            chooseScene->show();
        });
    });

    //监听选择场景的返回按钮
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry());

        this->show();
    });
}

void MainScene::paintEvent(QPaintEvent *) {
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/res/PlayLevelSceneBg.png");
    //绘制背景图
    //第三四参数用来让图片自适应屏幕，否则可以直接drawdrawPixmap(0, 0, pix);
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);


    //加载标题（背景上的小图标）
    pix.load(":/res/Title.png");
    //缩放图片
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    //绘制标题，第三四参数用来确定绘制时图片的宽高，此时正好为pix原来的
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}

MainScene::~MainScene()
{
    delete ui;
}
