#include "chooselevelscene.h"
#include "mypushbutton.h"

#include <QMenuBar> // 菜单栏的头文件
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //设置窗口固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("选择关卡");

    //创建菜单栏
    QMenuBar * bar = this->menuBar();
    this->setMenuBar(bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建按钮菜单项
    QAction * quitAction = startMenu->addAction("退出");
    //点击退出 退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //选择关卡按钮音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton * closeBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()-closeBtn->width(),this->height()-closeBtn->height()); // 把按钮放到右下角

    //返回按钮功能实现
    connect(closeBtn, &MyPushButton::clicked, [=](){
        backSound->play(); // 播放返回按钮的音效资源

        QTimer::singleShot(50, this,[=](){ // 延时返回
            this->hide();
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->chooseSceneBack();
        });
    });

    //创建关卡按钮
    for(int i = 0 ; i < 20;i++) {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + (i%4)*70 , 130+ (i/4)*70); // 使用 除|余 实现一层循环二维效果

        //监听选择关卡按钮的信号槽
        connect(menuBtn, &MyPushButton::clicked, [=](){
            chooseSound->play(); // 播放选择按钮的音效资源

            if(pScene == NULL)  //游戏场景最好不用复用，直接移除掉创建新的场景
            {
                // 进入到游戏场景
                this->hide();
                pScene = new PlayScene(i + 1); //将选择的关卡号 传入给PlayerScene

                // 设置游戏场景的初始位置
                pScene->setGeometry(this->geometry()); // setGeometry 是一个即时生效的操作，即使窗口已经显示，它也会立即调整窗口的位置和大小，即 放在show()后面也不会有错
                pScene->show();

                // PlayScene的返回按钮监听，删除该scene并且将指针指向空
                connect(pScene,&PlayScene::playSceneBack,[=](){ // 注意位置要在 点击关卡按钮 内才能生效
                    this->setGeometry(pScene->geometry());
                    this->show();
                    // 此处区别于 chooselevelscene，cls返回的界面要多次使用，返回后不删除；而 pScene 用一个创建一个，返回后要删除旧的
                    delete pScene;
                    pScene = NULL;
                });
            }
        });

//        menuBtn->setText(QString::number(i + 1)); // 直接在不规则图形上显示文字很难看

        //按钮上显示的文字
        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //设置label上文字 水平|垂直 居中
        label->move(25 + (i%4)*70 , 130+ (i/4)*70);
        label->setAttribute(Qt::WA_TransparentForMouseEvents,true);  //鼠标事件穿透 label层，若是直接把label的父对象设置为menuBtn则不需要这样
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *) { // 绘图事件一般用来处理的是静态图片
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/res/OtherSceneBg.png");
    //绘制背景图
    //第三四参数用来让图片自适应屏幕，否则可以直接drawdrawPixmap(0, 0, pix);
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);


    //加载标题（背景上的小图标）
    pix.load(":/res/Title.png");

    //在屏幕正中间绘制标题
    painter.drawPixmap((this->width() - pix.width())*0.5, 30,pix.width(),pix.height(),pix);
}
