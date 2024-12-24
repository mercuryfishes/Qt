#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "playscene.h"

class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);

    //重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    // 游戏场景对象指针
    PlayScene *pScene = NULL; // 此处为声明的 =NULL 方式

signals:
    // 写一个自定义的信号，告诉主场景，点击后返回
    void chooseSceneBack(); // 自定义的信号只需要声明不需要实现

public slots:
};

#endif // CHOOSELEVELSCENE_H
