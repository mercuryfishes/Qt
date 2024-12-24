#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyPushButton(QWidget *parent = nullptr); // 初始自带的构造函数，不符合要求，不要了

    // 构造函数
    //normalImg 代表正常显示的图片
    //pressImg  代表按下后显示的图片，默认为空
    // 注：老师说声明和实现只能有一个默认参数，即双引号部分，但实践会发现只能在声明中写，在实现里写时不时会出错
    MyPushButton(QString normalImg,QString pressImg = "");

    // 成员属性
    QString normalImgPath;  //默认显示图片路径
    QString pressedImgPath; //按下后显示图片路径

    // 弹跳特效
    void zoom1(); // 向下跳
    void zoom2(); // 向上跳
    void zoom(int , int );  // 一次实现

    // 重写按钮 按下和释放事件
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
