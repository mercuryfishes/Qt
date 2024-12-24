#include "playscene.h"
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"

#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSound>

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int levelNum) {
    qDebug() << "进入了第 " << levelNum << " 关";
    this->levelIndex = levelNum;

    // 初始化游戏场景
    //设置窗口固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币");

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

    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);

    // 返回按钮
    MyPushButton * closeBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()-closeBtn->width(),this->height()-closeBtn->height()); // 把按钮放到右下角

    //返回按钮功能实现
    connect(closeBtn, &MyPushButton::clicked, [=](){
        backSound->play(); // 播放返回按钮的音效

        QTimer::singleShot(50, this,[=](){ // 延时返回
//            this->hide(); // pScece 用完后直接删除，所以可以不用隐藏
            //触发自定义信号，关闭自身，该信号写到 signals下做声明
            emit this->playSceneBack();
        });
    });

    //当前关卡标题
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str = QString("Level: %1").arg(this->levelIndex);
    label->setText(str);
    // 设置大小和位置，此处方法相当于直接设定了一个矩形区域，函数的参数可以为一个矩形rect，也可以为设定矩形的四值(x，y，w, h)
    label->setGeometry(QRect(30, this->height() - 50,120, 50));

    // 胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());

    // 初始化每个关卡的二维数组
    dataConfig config;
    for(int i = 0 ; i < 4;i++) {
        for(int j = 0 ; j < 4; j++) {
            // Qt特殊的坐标体系 会使数组矩阵在实际显示按对角线翻转
            gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //创建金币的背景图片
    for(int i = 0 ; i < 4;i++) {
        for(int j = 0 ; j < 4; j++) {
           //绘制背景图片
            QLabel* label = new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode(1).png"));
            label->setParent(this);
            label->move(57 + i*50,200+j*50);

            // 创建金币
            QString img;
            if(gameArray[i][j] == 1) {
              img = ":/res/Coin0001.png";
            } else {
              img = ":/res/Coin0008.png";
            }

            MyCoin * coin = new MyCoin(img);
            coin->setParent(this);
            coin->move(59 + i*50,204+j*50);

            coin->posX = i;
            coin->posY = j;
            coin->flag = gameArray[i][j]; //记录正反标志

            // 将金币放入到 金币的二维数组里 以便于后期维护
            coinBtn[i][j] = coin;

            // 点击金币，进行翻转，connect监听与循环的执行是相对独立的，写在循环内只是为了保证每个金币绑定一个槽函数
            connect(coin,&MyCoin::clicked,[=](){
                flipSound->play(); // 播放点击翻金币按钮时的音效

                // 点击按钮，将所有的金币按钮都先禁用
                for(int i = 0 ; i < 4;i++) {
                    for(int j = 0 ; j < 4; j++) {
                        coinBtn[i][j]->isWin = true;
                    }
                }

                coin->changeFlag();
                gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0; //数组内部记录的标志同步修改

                // 翻转周围金币
                QTimer::singleShot(200, this,[=](){
                    if(coin->posX+1 <=3) {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posX-1>=0) {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY+1<=3) {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY-1>=0) {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }

                    // 翻完周围金币后，将所有金币按钮解开禁用
                    for(int i = 0 ; i < 4;i++) {
                        for(int j = 0 ; j < 4; j++) {
                            coinBtn[i][j]->isWin = false;
                        }
                    }

                    //判断是否胜利，必须写在 QTimer::singleShot 内，因为 QTimer::singleShot 的异步特性会导致执行顺序变化。
                    this->isWin = true;
                    for(int i = 0 ; i < 4;i++) {
                        for(int j = 0 ; j < 4; j++) {
                            if( coinBtn[i][j]->flag == false) {
                                this->isWin = false;
                                break;
                            }
                        }
                    }

                    if(this->isWin) {
                        winSound->play(); // 播放胜利时的音效

                        // 将所有金币按钮的胜利标志改为true，如果再次点击，重写的鼠标按下事件不做相应
                        for(int i = 0 ; i < 4;i++) {
                            for(int j = 0 ; j < 4; j++) {
                                coinBtn[i][j]->isWin = true;
                            }
                        }

                        // 将胜利的图片移动下来
                        QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel,"geometry");
                        // 设置时间间隔
                        animation1->setDuration(1000);
                        // 设置开始位置
                        animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        // 设置结束位置
                        animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        // 设置缓和曲线
                        animation1->setEasingCurve(QEasingCurve::OutBounce);
                        // 执行动画
                        animation1->start();
                    }
                });
            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);
}
