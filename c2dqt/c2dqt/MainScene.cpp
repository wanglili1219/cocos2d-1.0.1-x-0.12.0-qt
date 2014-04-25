#include "MainScene.h"
#include "GLES/gl.h"
#include "CCAffineTransform.h"


#include<math.h>
float pi = 3.1415926;
float raid_30 = pi / 6;

struct CPoint{
	CPoint::CPoint(int _x, int _y):x(_x), y(_y){}
	CPoint(){x = y = 0;};
	int x, y;

};
int m_iGridWidth = 60;
int m_iGridHeigh = 30;
int m_yTile = 1024;
int m_xTile = 1024;
int m_screenHeight = 320;
int m_screenWidth = 480;
struct  TileData  
{  
    CCPoint tile;  
    BYTE flag;  
  
    TileData() : flag(0) {}  
};


//参数： 
// POINT p 指定的某个点
// LPPOINT ptPolygon 多边形的各个顶点坐标（首末点可以不一致） 
// int nCount 多边形定点的个数 
BOOL PtInPolygon(CPoint p, LPPOINT ptPolygon, int nCount) 
{ 
	int nCross = 0; 
	for (int i = 0; i < nCount; i++)
	{ 
		POINT p1 = ptPolygon[i]; 
		POINT p2 = ptPolygon[(i + 1) % nCount]; 
		// 求解 y=p.y 与 p1p2 的交点 
		if ( p1.y == p2.y ) // p1p2 与 y=p0.y平行
			continue; 
		if ( p.y < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
			continue;
		if ( p.y >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
			continue; 
		// 求交点的 X 坐标 -------------------------------------------------------------- 
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 
		if ( x > p.x ) 
			nCross++; // 只统计单边交点
	} 
	// 单边交点为偶数，点在多边形之外 --- 
	return (nCross % 2 == 1); 
} 



//判断点在不在菱形中
//pt-点指针
//x--菱形中心点横坐标
//y--菱形中心点纵坐标
BOOL IsPtInDiamond(CPoint& pt, int x, int y)
{
	POINT point4[4];
	point4[0].x = x - m_iGridWidth * 0.5;
	point4[0].y = y;
	point4[1].x = x;
	point4[1].y = y - m_iGridHeigh * 0.5;
	point4[2].x = x + m_iGridWidth * 0.5;
	point4[2].y = y;
	point4[3].x = x;
	point4[3].y = y + m_iGridHeigh * 0.5;
	return PtInPolygon(pt, point4, 4);	
}

//获取鼠标点中的那个菱形的中心点
//pt-------鼠标位置
//pCenter--返回中心点坐标
BOOL GetCursorDiamond(CPoint& pt, CPoint& pCenter)
{
	RECT Rect;
	Rect.left = pt.x / m_iGridWidth * m_iGridWidth;
	Rect.top = pt.y / m_iGridHeigh * m_iGridHeigh;
	Rect.right = Rect.left + m_iGridWidth;
	Rect.bottom = Rect.top + m_iGridHeigh;

	if((Rect.left / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.left, pCenter.y = Rect.top;
		if(IsPtInDiamond(pt, Rect.left, Rect.top) == TRUE) return TRUE;
	}
	if((Rect.right / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.right, pCenter.y = Rect.top;
		if(IsPtInDiamond(pt, Rect.right, Rect.top) == TRUE) return TRUE;
	}
	if((Rect.left / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.left, pCenter.y = Rect.bottom;
		if(IsPtInDiamond(pt, Rect.left, Rect.bottom) == TRUE) return TRUE;
	}
	if((Rect.right / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.right, pCenter.y = Rect.bottom;
		if(IsPtInDiamond(pt, Rect.right, Rect.bottom)==TRUE) return TRUE;
	}
	return FALSE;
}

BOOL myGetCursorDiamond(CPoint& pt, CPoint& pCenter)
{
	RECT Rect;
	Rect.left = pt.x / m_iGridWidth * m_iGridWidth;
	Rect.bottom = pt.y / m_iGridHeigh * m_iGridHeigh;
	Rect.right = Rect.left + m_iGridWidth;
	Rect.top = Rect.bottom + m_iGridHeigh;

	if((Rect.left / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.left, pCenter.y = Rect.top;
		if(IsPtInDiamond(pt, Rect.left, Rect.top) == TRUE) return TRUE;
	}
	if((Rect.right / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.right, pCenter.y = Rect.top;
		if(IsPtInDiamond(pt, Rect.right, Rect.top) == TRUE) return TRUE;
	}
	if((Rect.left / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.left, pCenter.y = Rect.bottom;
		if(IsPtInDiamond(pt, Rect.left, Rect.bottom) == TRUE) return TRUE;
	}
	if((Rect.right / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.right, pCenter.y = Rect.bottom;
		if(IsPtInDiamond(pt, Rect.right, Rect.bottom)==TRUE) return TRUE;
	}

	pCenter.x = Rect.left +  m_iGridWidth % 2;
	pCenter.y = Rect.bottom + m_iGridHeigh % 2;
	return FALSE;
}

BOOL GetRealDiamond(CPoint& lbpos, CPoint& mousePos, CPoint& pCenter)
{
	RECT Rect;
	Rect.left = lbpos.x;
    Rect.top = lbpos.y + m_iGridHeigh;
	Rect.right = lbpos.x + m_iGridWidth;
	Rect.bottom = lbpos.y;

	if((Rect.left / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.left, pCenter.y = Rect.top;
		if(IsPtInDiamond(mousePos, Rect.left, Rect.top) == TRUE) return TRUE;
	}
	if((Rect.right / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.right, pCenter.y = Rect.top;
		if(IsPtInDiamond(mousePos, Rect.right, Rect.top) == TRUE) return TRUE;
	}
	if((Rect.left / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.left, pCenter.y = Rect.bottom;
		if(IsPtInDiamond(mousePos, Rect.left, Rect.bottom) == TRUE) return TRUE;
	}
	if((Rect.right / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	{
		pCenter.x = Rect.right, pCenter.y = Rect.bottom;
		if(IsPtInDiamond(mousePos, Rect.right, Rect.bottom)==TRUE) return TRUE;
	}
	return FALSE;
}


//屏幕坐标转换成游戏坐标 
static int getGx(float x,  float y) 
{
     return (int) (0.5f * (y / (m_iGridHeigh >> 1) + x / (m_iGridWidth >> 1))); 
}

 static int getGy(float x, float y) 
{
     return (int) (0.5f * (y / (m_iGridHeigh >> 1) - x / (m_iGridWidth >> 1))); 
}


CPoint Space2Tile(CPoint& screen)  
{  
    CPoint center;  
    GetCursorDiamond(screen, center);  
    int x=(center.x/(m_iGridWidth * 2)+(center.y+m_iGridHeigh)/(m_iGridHeigh*2));   
    int y=center.x/(m_iGridWidth * 2)-center.y/(m_iGridHeigh*2) + m_yTile;  
    return CPoint(x, y);  
}  

#define BACKGROUD_SCALE 1.5

MainScene* g_mainScene = 0;

MainScene::MainScene()
    : m_backgroud(0)
    , m_touchSpr(0)
    , m_curSelectedMapType(-1)
{

}

MainScene::~MainScene()
{

}

CCScene* MainScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	scene->retain();
	
	// 'layer' is an autorelease object
	MainScene *layer = MainScene::node();

	// add layer as a child to scene
	scene->addChild(layer);

	g_mainScene = layer; 
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	
	/////////////////////////////
	// 3. add your codes below...
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// add "HelloWorld" splash screen"
	m_backgroud = CCSprite::spriteWithFile("backgroud.jpg");
    if (m_backgroud == NULL){
        return false;
    }

    m_backgroud->setScale(BACKGROUD_SCALE);
	// position the sprite on the center of the screen
	m_backgroud->setPosition( ccp(size.width/2, size.height/2) );
	// add the sprite as a child to this layer
	this->addChild(m_backgroud, -1);

	//drawMap();

    m_bIsTouchEnabled = true;

	return true;
}

void MainScene::registerWithTouchDispatcher(void)
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}

void MainScene::draw(void)
{
   // drawMap();

}

void MainScene::menuCloseCallback(CCObject* pSender)
{
	cocos2d::CCDirector::sharedDirector()->end();
}

bool MainScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint pos = pTouch->locationInView(0);
    CCPoint mappos = coorScreen2coorRender(pos);
    //m_touchSpr = querySpriteInMap(pos);
    if (!m_touchSpr && m_curSelectedMapType > 0){
		char buf[128] = {0};
		sprintf(buf, "map%d.png", m_curSelectedMapType);
	    CCSprite* newmap =  CCSprite::spriteWithFile(buf);
        if (newmap == NULL){
            return false;
        }
     
		CPoint pCenter;
		CPoint mypos(mappos.x, mappos.y);
        myGetCursorDiamond(mypos, pCenter);

        int tilex = getGx(pCenter.x, pCenter.y);
        int tiley = getGy(pCenter.x, pCenter.y);

        float px  = (m_iGridWidth >> 1) * (tiley - tilex);
        float py = (m_iGridHeigh >> 1) * (tilex + tiley);
		

       
        //GetRealDiamond(ccp(px, py), pos, pCenter);

        // int realtilex = getGx(pCenter.x, pCenter.y);
        // int realtiley = getGy(pCenter.x, pCenter.y);

        // float realpx  = (m_iGridWidth >> 1) * (realtiley - realtilex);
        // float realpy = (m_iGridHeigh >> 1) * (realtilex + realtiley);
         
		//CCLOG("<%d , %d> %f %f real <%d , %d> %f %f", tilex, tiley, px, py, realtilex, realtiley, realpx, realpy);
		CCSize s(m_iGridWidth, m_iGridHeigh);
		newmap->setContentSize(s);
       
		newmap->setAnchorPoint(ccp(0.5, 0.5));
		newmap->setPosition(ccp(pCenter.x, pCenter.y));

        this->addChild(newmap);
        m_spriteInMap.push_back(newmap);
        m_touchSpr = newmap;
    }

    return true;
}

void MainScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_backgroud){
        return;
    }

    CCPoint pos = pTouch->locationInView(0);

    if (m_touchSpr){
        CCPoint newpos = coorScreen2coorRender(pos);
        m_touchSpr->setPosition(newpos);
        return;
    }

    CCPoint prepos = pTouch->previousLocationInView(0);
    float nx = pos.x - prepos.x;
    float ny = pos.y - prepos.y;
    float sprx = this->getPositionX() + nx;
    float spry = this->getPositionY() - ny;

    CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSize sprsize = m_backgroud->getContentSize();
	float sprscale = m_backgroud->getScale();
	float lx = (sprsize.width * sprscale * 0.5 - size.width * 0.5);
	float ly = (sprsize.height * sprscale * 0.5 - size.height * 0.5);
    if (sprx > lx){
        sprx = lx;
    }

    if (spry > ly){
        spry = ly;
    }

    if (sprx < - lx){
        sprx = - lx;
    }

    if (spry < - ly){
        spry = - ly;
    }

    this->setPosition(ccp(sprx, spry));
}

void MainScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	
    if (m_touchSpr){
        m_touchSpr = 0;
    }
}

void MainScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}

CCPoint MainScene::coorScreen2coorRender(CCPoint pos)
{
    CCPoint glpos = CCDirector::sharedDirector()->convertToGL(pos);
    CCPoint mappos = this->getPosition();
	CCPoint newscpos = ccpSub(glpos, mappos);
    return newscpos;
}

CCSprite* MainScene::querySpriteInMap(CCPoint scrpos)
{
	std::vector<CCSprite*>::iterator iter = m_spriteInMap.begin();
    for (; iter != m_spriteInMap.end(); ++iter){
        CCSprite* spr = *iter;
        CCPoint pos = spr->getPosition();
        CCPoint arcpos = spr->getAnchorPoint();
        CCSize  sprsize = spr->getContentSize();
        CCRect sprrect(pos.x - (arcpos.x * sprsize.width)
                       , pos.y - (arcpos.y * sprsize.height)
                       , sprsize.width
                       , sprsize.height);
      
        bool iscontain = CCRect::CCRectContainsPoint(sprrect, scrpos);
        if (iscontain){
            return spr;
        }
    }

    return 0;
}

void MainScene::setSelMapType(int listRow)
{
    m_curSelectedMapType = listRow;
}



void MainScene::drawMap()
{
	return;
	static float an = 1;
    glColor4ub(255, 255, 0, 255);
    glLineWidth(2);

    int orgx = 480;
    int orgy = 0;
	int mw = 60;
	int mh = 30;

    CCAffineTransform lastform = CCAffineTransformMakeIdentity();
	
    lastform = CCAffineTransformTranslate(lastform, 300, 220);

	 //当paintY为CHIP_H / 2的奇数倍时,paintX需要偏移CHIP_W / 2 
	//int m_iGridWidth = 60;
 //int m_iGridHeigh = 30;



 static int offset = 0;
 for (int paintY = 0; paintY <= 480 + m_iGridHeigh; paintY += m_iGridHeigh / 2) 
{
     for (int paintX = 0; paintX <= 320 + m_iGridWidth; paintX += m_iGridWidth) 
    {
         int gx = getGx(paintX + offset, paintY) + 0;
         int gy = getGy(paintX + offset, paintY) + 0; 
        
         
		   float px  = (m_iGridWidth >> 1) * (gy - gx) + orgx;
           float py = (m_iGridHeigh >> 1) * (gy + gx) + orgy;

		  CCSprite* item = CCSprite::spriteWithFile("map3.png");
			CCSize s(m_iGridWidth, m_iGridHeigh);
			 item->setContentSize(s);
			//CCPoint glpos = CCDirector::sharedDirector()->convertToGL(ccp(px, py));

			item->setPosition(ccp(px, py));
			this->addChild(item);
		 
		
     } 
     
	 offset = offset == 0 ? m_iGridWidth / 2 : 0;
 }

	
}  
	
