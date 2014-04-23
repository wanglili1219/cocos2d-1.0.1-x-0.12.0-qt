#pragma once
#include <QWidget>  
#include <cocos2d.h>  
#include "CCEGLView.h"  
#include <AppDelegate.h>  
#include <QPushButton>  
#include <QFrame.h>  
#include "MainScene.h"  
#include <QTimerEvent>  
#include <QCloseEvent> 

USING_NS_CC;  
  
class Cocos2dWidget: public QWidget{  
    Q_OBJECT  
public:  
    Cocos2dWidget(QWidget*parent= 0,const char * name = 0);  
    ~Cocos2dWidget();  

   void initCocos2d();  

   void closeEvent(QCloseEvent *event);
   virtual void paintEvent(QPaintEvent *evt);  
   virtual void timerEvent(QTimerEvent* evt);  
   virtual void keyPressEvent(QKeyEvent *evt);  
   virtual void mousePressEvent(QMouseEvent* evt);  
   virtual void mouseReleaseEvent(QMouseEvent *);
   virtual void mouseMoveEvent(QMouseEvent *);
   virtual void mouseDoubleClickEvent(QMouseEvent *);

private:   
    void updateDraw(); 
    CCEGLView * eglView;  
    CCDirector *pDirector ;  
    CCScene * pScene;  
    int cocosTimerId;  
    MainScene * layer;  
    CCParticleSun * m_emitter;  
    CCActionInterval *pEffect;   
};  
