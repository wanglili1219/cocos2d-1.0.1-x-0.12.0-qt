#include "Cocos2dWidget.h"
#include "CCEGLView_qt.h"

#include<cmath>  
USING_NS_CC;  
  
Cocos2dWidget::Cocos2dWidget(QWidget* parent,const char* name):QWidget(parent,Qt::MSWindowsOwnDC)  
{  
    this->setFixedSize(480, 320);  
    this->setWindowTitle("Editor");  
    this->setAccessibleName("Editor");  
   
    m_emitter=NULL;  
}  
Cocos2dWidget::~Cocos2dWidget()  
{  
    killTimer(cocosTimerId);
  

    //release pointers  
    pDirector->release();  
    if(!m_emitter)  
        m_emitter->release();  
    pEffect->release();  
}  
void Cocos2dWidget::initCocos2d()  
{  
	HWND wnd = (HWND)this->winId();
	AppDelegate app;
	
	CCApplication::sharedApplication().run(wnd, TEXT("Hello"), 480, 320);
    cocosTimerId = startTimer(1.0/60.0*1000);
}  

void Cocos2dWidget::paintEvent(QPaintEvent *evt){  
    Q_UNUSED(evt);  
}  

void Cocos2dWidget::timerEvent(QTimerEvent *evt){  
    Q_UNUSED(evt);  
    if(evt->timerId()==cocosTimerId)  
    {  
        updateDraw();  
    }  
}  
void Cocos2dWidget::updateDraw()
{  
    CCDirector::sharedDirector()->mainLoop();  
}  
void Cocos2dWidget::mousePressEvent(QMouseEvent* evt)  
{  
	short x = (short)evt->x();
	short y = (short)evt->y();
    CCEGLView::sharedOpenGLView().WindowProc(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(x, y));
}  

void Cocos2dWidget::mouseReleaseEvent(QMouseEvent * evt)
{
	short x = (short)evt->x();
	short y = (short)evt->y();
	CCEGLView::sharedOpenGLView().WindowProc(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(x, y));
}

void Cocos2dWidget::mouseDoubleClickEvent(QMouseEvent *)
{

}

void Cocos2dWidget::mouseMoveEvent(QMouseEvent * evt)
{
    short x = (short)evt->x();
	short y = (short)evt->y();
    CCEGLView::sharedOpenGLView().WindowProc(WM_MOUSEMOVE, MK_LBUTTON, MAKELONG(x, y));
}

void Cocos2dWidget::keyPressEvent(QKeyEvent *evt)  
{  
 
}

void Cocos2dWidget::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}
