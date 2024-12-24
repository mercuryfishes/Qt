#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>

#include "chooselevelscene.h" // 自建的类用""，原有的库用<>

namespace Ui {
class MainScene;
}

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = nullptr);
    ~MainScene();

    //重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    //选择关卡场景
    ChooseLevelScene *chooseScene = new ChooseLevelScene; // 也可以在此处=NULL 在cpp里定义实例
private:
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H
