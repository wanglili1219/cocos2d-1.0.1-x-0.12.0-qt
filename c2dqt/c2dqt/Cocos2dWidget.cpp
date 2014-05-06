#include "Cocos2dWidget.h"
#include "CCEGLView_qt.h"

#include<cmath>  
USING_NS_CC;  
 extern MainScene* g_mainScene;
Cocos2dWidget::Cocos2dWidget(QWidget* parent,const char* name)
		: QWidget(parent,Qt::MSWindowsOwnDC)  
		, layer(0)
        , m_actDelete(0)
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

    m_actDelete = new QAction(tr("delete"), this); 
    connect(m_actDelete, SIGNAL(triggered()), this, SLOT(slotDelete())); 
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
	if (evt->button() == Qt::LeftButton){
		CCEGLView::sharedOpenGLView().WindowProc(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(x, y));
	}else if (evt->button() == Qt::RightButton){
		CCEGLView::sharedOpenGLView().WindowProc(WM_RBUTTONDOWN, MK_LBUTTON, MAKELONG(x, y));
	}
	
}  

void Cocos2dWidget::mouseReleaseEvent(QMouseEvent * evt)
{
	short x = (short)evt->x();
	short y = (short)evt->y();
	if (evt->button() == Qt::LeftButton){
		CCEGLView::sharedOpenGLView().WindowProc(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(x, y));
	}else if (evt->button() == Qt::RightButton){
		CCEGLView::sharedOpenGLView().WindowProc(WM_RBUTTONUP, MK_LBUTTON, MAKELONG(x, y));
	}
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

void Cocos2dWidget::curveChanged(int row)
{
	//CCDirector *pDirector = CCDirector::sharedDirector();
	//MainScene* mainscene = (MainScene*)pDirector->getRunningScene();
	if (g_mainScene){
		g_mainScene->setSelMapType(row);
	}
}

void Cocos2dWidget::slotDelete()
{
    if (g_mainScene){
      g_mainScene->deleteTile();
	}
}

void Cocos2dWidget::contextMenuEvent(QContextMenuEvent * e) 
{ 
    if (g_mainScene){
        if (g_mainScene->onClickMouseKey(e->x(), e->y())){
            QCursor cur = this->cursor(); 
            QMenu *menu=new QMenu(this); 
            menu->addAction(m_actDelete); 
			int x = cur.pos().x();
			int y = cur.pos().y();
            menu->exec(cur.pos()); 
        }
    }
} 
