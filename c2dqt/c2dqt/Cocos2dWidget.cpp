#include "Cocos2dWidget.h"
#include "ParticleView.h"

#include<cmath>  
USING_NS_CC;  
  
Cocos2dWidget::Cocos2dWidget(QWidget* parent,const char* name):QWidget(parent,Qt::MSWindowsOwnDC)  
{  
    //  
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
    cocosTimerId = startTimer(1.0/60.0*1000);//设置定时器，调用cocos2d刷新  
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
void Cocos2dWidget::updateDraw(){  
	
   //HWND hWnd = (HWND)this->winId();  
   //CCEGLView* pMainWnd = ParticleView::sharedOpenGLViewWithHwnd((HWND)this->winId());  
   // ShowWindow(pMainWnd->getHWnd(), SW_SHOW);  
    //绘制场景  
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

/************************************************************************/  
/* 3个demo                                                                     */  
/************************************************************************/  
//平移精灵  
void Cocos2dWidget::translate()  
{  
	CCArray* ch = CCDirector::sharedDirector()->getRunningScene()->getChildren();
	layer = (HelloWorld*)ch->objectAtIndex(0);

    CCSprite *spr = CCSprite::spriteWithFile("closeNormal.png");  
    srand( (unsigned)time( NULL ) );  
      
    spr->setPosition(ccp(rand()%100,rand()%100));  
    layer->addChild(spr);  
    int x = rand()%200;  
    int y = rand()%200;  
    CCActionInterval*  actionTo = CCMoveBy::actionWithDuration(rand()%4+2,ccp(x,y));  
    spr->runAction(actionTo);  
      
}  
//粒子  
void Cocos2dWidget::particleDemo()  
{  
	CCArray* ch = CCDirector::sharedDirector()->getRunningScene()->getChildren();
	layer = (HelloWorld*)ch->objectAtIndex(0);

    if(!m_emitter)  
    {  
        m_emitter = CCParticleSun::node();  
        m_emitter->retain();  
        layer->addChild(m_emitter);  
  
        m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("closeNormal.png") );  
  
        CCSize s = CCDirector::sharedDirector()->getWinSize();  
        if (m_emitter != NULL)  
        {  
            m_emitter->setPosition( ccp(s.width / 2, s.height / 2) );  
        }  
    }  
    else  
    {  
        layer->removeChild(m_emitter, true);  
        m_emitter->autorelease();  
        m_emitter=NULL;  
    }  
      
}  
//effect效果  
void Cocos2dWidget::effectDemo()  
{  
	CCArray* ch = CCDirector::sharedDirector()->getRunningScene()->getChildren();
	layer = (HelloWorld*)ch->objectAtIndex(0);
    CCShaky3D* pEffect = CCShaky3D::actionWithRange(10, true, ccg(15,10), 0.1);  
    layer->runAction(pEffect);  
}  
