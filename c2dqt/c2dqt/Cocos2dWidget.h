#pragma once
#include <QWidget>  
#include <cocos2d.h>  
#include "CCEGLView.h"  
#include <AppDelegate.h>  
#include <QPushButton>  
#include <QFrame.h>  
#include "HelloWorldScene.h"  
#include <QTimerEvent>  
#include <QCloseEvent> 

USING_NS_CC;  
  
  
class Cocos2dWidget: public QWidget{  
    Q_OBJECT  
public:  
    Cocos2dWidget(QWidget*parent= 0,const char * name = 0);  
    ~Cocos2dWidget();  

   void initCocos2d(); //这里用于cocos2d的初始化  

   void closeEvent(QCloseEvent *event);
   virtual void paintEvent(QPaintEvent *evt);  
   virtual void timerEvent(QTimerEvent* evt);  
   virtual void keyPressEvent(QKeyEvent *evt);  
   virtual void mousePressEvent(QMouseEvent* evt);  
   virtual void mouseReleaseEvent(QMouseEvent *);
   virtual void mouseMoveEvent(QMouseEvent *);
   virtual void mouseDoubleClickEvent(QMouseEvent *);

public slots:  
    void translate();  
    void particleDemo();  
    void effectDemo();  

    //void resizeEvent(QResizeEvent* evt);//窗口可能缩放  
private:  
  
    void updateDraw();  //用一个定时器调用，实现刷新  
    CCEGLView * eglView;  
    CCDirector *pDirector ;  
    CCScene * pScene;  
    int cocosTimerId;  
    HelloWorld * layer;  
    CCParticleSun * m_emitter;  
    CCActionInterval *pEffect;  
 
  
  
};  
