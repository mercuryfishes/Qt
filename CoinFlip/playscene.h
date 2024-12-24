#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>

#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
//    explicit PlayScene(QWidget *parent = nullptr);

    PlayScene(int levelNum);

    int levelIndex; // 内部成员属性，记录所选的关卡号

    //重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    int gameArray[4][4]; // 二维数组 维护每个关卡的具体数据

    MyCoin * coinBtn[4][4]; //金币按钮数组，必须要有，只使用二维数据组无法找到指定金币触发connect事件

    bool isWin = true; // 是否胜利的标志

signals:
    // 写一个自定义的信号，告诉主场景，点击后返回
    void playSceneBack(); // 自定义的信号只需要声明不需要实现


public slots:
};

#endif // PLAYSCENE_H
