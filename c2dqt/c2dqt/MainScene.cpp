#include "MainScene.h"
#include "GLES/gl.h"
#include "CCAffineTransform.h"

#include<math.h>
float pi = 3.1415926;
float raid_30 = pi / 6;

const int m_iGridWidth = 60;
const int m_iGridHeigh = 30;
const int m_yTile = 0;
const int m_xTile = 0;
const int m_screenHeight = 320;
const int m_screenWidth = 480;
struct  TileData  
{  
    CCPoint tile;  
    BYTE flag;  
  
    TileData() : flag(0) {}  
};


int g_mapBit[m_screenWidth / m_iGridWidth][m_screenHeight / m_iGridHeigh] = { 0 };

void clearMapBit()
{
    memset(g_mapBit, 0, sizeof(g_mapBit));
}

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

BOOL IsPtIn4Diamond(CPoint& pt, int x, int y)
{
	POINT point4[4];
	point4[0].x = x - m_iGridWidth;
	point4[0].y = y;
	point4[1].x = x;
	point4[1].y = y - m_iGridHeigh;
	point4[2].x = x + m_iGridWidth;
	point4[2].y = y;
	point4[3].x = x;
	point4[3].y = y + m_iGridHeigh;
	return PtInPolygon(pt, point4, 4);	
}


//获取鼠标点中的那个菱形的中心点
//pt-------鼠标位置
//pCenter--返回中心点坐标
bool GetCursorDiamond(CPoint& pt, CPoint& pCenter)
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
	return true;
}

BOOL myGetCursorDiamond(CPoint& pt, CPoint& pCenter)
{
	RECT Rect;
	Rect.left = pt.x / m_iGridWidth * m_iGridWidth;
	Rect.bottom = pt.y / m_iGridHeigh * m_iGridHeigh;
	Rect.right = Rect.left + m_iGridWidth;
	Rect.top = Rect.bottom + m_iGridHeigh;

	//if((Rect.left / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	//{
		
		if(IsPtInDiamond(pt, Rect.left, Rect.top) == TRUE){
			pCenter.x = Rect.left, pCenter.y = Rect.top;
			return TRUE;
		}
	//}
	//if((Rect.right / m_iGridWidth % 2) == (Rect.top / m_iGridHeigh % 2))
	//{
		
		if(IsPtInDiamond(pt, Rect.right, Rect.top) == TRUE){
			pCenter.x = Rect.right, pCenter.y = Rect.top;
			return TRUE;
		}
	//}

	//if((Rect.left / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	//{
	
		if(IsPtInDiamond(pt, Rect.left, Rect.bottom) == TRUE){
			pCenter.x = Rect.left, pCenter.y = Rect.bottom;
			return TRUE;
		}
	//}
	//if((Rect.right / m_iGridWidth % 2) == (Rect.bottom / m_iGridHeigh % 2))
	//{
		
		if(IsPtInDiamond(pt, Rect.right, Rect.bottom)==TRUE){
			pCenter.x = Rect.right, pCenter.y = Rect.bottom;
			return TRUE;
		}
	//}

	pCenter.x = Rect.left + m_iGridWidth * 0.5 ;
	pCenter.y = Rect.bottom + m_iGridHeigh * 0.5;
	return true;
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


CPoint coorMap2coorLogic(CPoint center)
{
    int x = (center.x / (m_iGridWidth * 2) + (center.y + m_iGridHeigh) / (m_iGridHeigh*2));   
    int y = center.x /(m_iGridWidth * 2) - center.y / (m_iGridHeigh*2) + m_yTile;  
    return CPoint(x, y);
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
    , m_itemInfo(0)
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

// on "init" you need to initialize your instancecr
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

    clearMapBit();
	
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
    drawDiamondMap();
    checkCanAnchor();
}

void MainScene::menuCloseCallback(CCObject* pSender)
{
	cocos2d::CCDirector::sharedDirector()->end();
}

CCPoint MainScene::convertRenderCoor(int tilex, int tiley)
{
    float x =  (tilex - tiley) * m_iGridWidth / 2;
    float y = (tilex + tiley) * m_iGridHeigh / 2;
    y = y + m_iGridHeigh * 0.5;

    return ccp(x, y);
}

bool MainScene::canAddTile(int tilex, int tiley, int rpltilenum)
{
    if (rpltilenum == 1){
        return !g_mapBit[tilex][tiley];
    }else if (rpltilenum == 4){
      return (!g_mapBit[tilex][tiley]
				&& !g_mapBit[tilex + 1][tiley]
                && !g_mapBit[tilex + 1][tiley + 1]
                && !g_mapBit[tilex][tiley + 1]);

    }

    return false;
}

itemInfo* MainScene::addTile2DiamondMap(CCSprite* spr, int tilex, int tiley, int pltilenum)
{
    CCPoint renpos = convertRenderCoor(tilex, tiley);
    if (pltilenum == 1){
        CCSize s(m_iGridWidth, m_iGridHeigh);
		spr->setContentSize(s);
		spr->setAnchorPoint(ccp(0.5f, 0.5f));
		spr->setPosition(renpos);

    }else if (pltilenum == 4){
        renpos.y += m_iGridHeigh * 0.5;
        CCSize s(m_iGridWidth, m_iGridHeigh);
		spr->setContentSize(s);
		spr->setAnchorPoint(ccp(0.5f, 0.5f));
		spr->setPosition(renpos);
    }

    this->addChild(spr);
    itemInfo io;
    io.spr = spr;
    io.tileNum = pltilenum;
    io.tilex = tilex;
    io.tiley = tiley;
    m_spriteInMap.push_back(io);
    
    return &m_spriteInMap.back();
}


void MainScene::addTile2Map(CCSprite* spr, CPoint pcenter, int rpltile)
{
    // int x = getGx(pcenter.x, pcenter.y);
    // int y = getGy(pcenter.x, pcenter.y);

    // if (rpltile == 1){
    //     CCSize s(m_iGridWidth, m_iGridHeigh);
	// 	spr->setContentSize(s);
	// 	spr->setAnchorPoint(ccp(0.5f, 0.5f));
	// 	spr->setPosition(ccp(pcenter.x, pcenter.y));
    //     g_mapBit[x][y] = false;

    // }else if (rpltile == 4){
    //     CCPoint cpos(pcenter.x, pcenter.y + m_iGridHeigh * 0.5);
	// 	CCLOG("%f %f", cpos.x, cpos.y);
    //     CCSize s(m_iGridWidth, m_iGridHeigh);
	// 	spr->setContentSize(s);
	// 	spr->setAnchorPoint(ccp(0.5f, 0.5f));
	// 	spr->setPosition(cpos);
    //     g_mapBit[x][y] = false;
    //     g_mapBit[x + 1][y] = false;
    //     g_mapBit[x + 1][y + 1] = false;
    //     g_mapBit[x][y + 1] = false;
    // }

	//   this->addChild(spr);

    //   itemInfo io;
    //   io.spr = spr;
    //   io.tileNum = rpltile;
    //   m_spriteInMap.push_back(io);
}

void MainScene::checkCanAnchor()
{
    std::vector<itemInfo>::iterator iter = m_spriteInMap.begin();
    for (; iter != m_spriteInMap.end(); ++iter){
        itemInfo& io = *iter;
        const ccColor3B& color = io.spr->getColor();
        if (color.r == ccRED.r 
            && color.g == ccRED.g
            && color.b == ccRED.b){
            if (canAddTile(io.tilex, io.tiley, io.tileNum)){
                io.spr->setColor(ccGREEN);
                use4Tile(io.tilex, io.tiley);
            }
        }
    }
}

bool MainScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint pos = pTouch->locationInView(0);
    CCPoint mappos = coorScreen2coorRender(pos);
    int x = getGx(mappos.x, mappos.y);
    int y = getGy(mappos.x, mappos.y);
    CCLOG("%d %d", x, y);
    
    m_itemInfo = querySpriteInMap(mappos);
    if (m_itemInfo){
        if (m_itemInfo->tileNum == 1){
            unuseOneTile(m_itemInfo->tilex, m_itemInfo->tiley);
        }else if (m_itemInfo->tileNum == 4){
            unuse4Tile(m_itemInfo->tilex, m_itemInfo->tiley);
        }

        m_itemInfo->spr->setColor(ccGREEN);
    }

    if (!m_itemInfo && m_curSelectedMapType > 0){
		char buf[128] = {0};
		sprintf(buf, "map%d.png", m_curSelectedMapType);
	    CCSprite* newmap =  CCSprite::spriteWithFile(buf);
        if (newmap == NULL){
            return false;
        }

        const int rpnum = 4;
        m_itemInfo = addTile2DiamondMap(newmap, x, y, rpnum);
        assert(m_itemInfo);
        if (canAddTile(x, y, rpnum)){
            useOneTile(x, y);
            m_itemInfo->spr->setColor(ccGREEN);
        }else{
            use4Tile(x, y);
            m_itemInfo->spr->setColor(ccRED);
        }

		// CPoint pCenter;
		// CPoint mypos(mappos.x, mappos.y);
        // myGetCursorDiamond(mypos, pCenter);

        // int tilex = getGx(pCenter.x, pCenter.y);
        // int tiley = getGy(pCenter.x, pCenter.y);

        // float px  = (m_iGridWidth >> 1) * (tiley - tilex);
        // float py = (m_iGridHeigh >> 1) * (tilex + tiley);
       
        //GetRealDiamond(ccp(px, py), pos, pCenter);

        // int realtilex = getGx(pCenter.x, pCenter.y);
        // int realtiley = getGy(pCenter.x, pCenter.y);

        // float realpx  = (m_iGridWidth >> 1) * (realtiley - realtilex);
        // float realpy = (m_iGridHeigh >> 1) * (realtilex + realtiley);
         
		//CCLOG("<%d , %d> %f %f real <%d , %d> %f %f", tilex, tiley, px, py, realtilex, realtiley, realpx, realpy);
		//addTile2Map(newmap, pCenter, 4);
        //m_touchSpr = newmap;
    }

    return true;
}

void MainScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_backgroud){
        return;
    }

    CCPoint pos = pTouch->locationInView(0);
    if (m_itemInfo){
        CCPoint mappos = coorScreen2coorRender(pos);
        int tilex = getGx(mappos.x, mappos.y);
        int tiley = getGy(mappos.x, mappos.y);
        const int pltilenum = 4;
		CCPoint renpos;
        if (pltilenum == 1){
            renpos = convertRenderCoor(tilex, tiley);
        }else if (pltilenum == 4){
            renpos = convertRenderCoor(tilex, tiley);
            renpos.y += m_iGridHeigh * 0.5;
        }

        m_itemInfo->spr->setPosition(renpos);
        m_itemInfo->tilex = tilex;
        m_itemInfo->tiley = tiley;
        if (canAddTile(m_itemInfo->tilex, m_itemInfo->tiley, m_itemInfo->tileNum)){
            m_itemInfo->spr->setColor(ccGREEN);
        }else{
            m_itemInfo->spr->setColor(ccRED);
        }

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
    // if (m_itemInfo){
    //     if (m_itemInfo->tileNum == 1){
    //         useOneTile(m_itemInfo->tilex, m_itemInfo->tiley);
    //     }else if (m_itemInfo->tileNum == 4){
    //         use4Tile(m_itemInfo->tilex, m_itemInfo->tiley);
    //     }
    // }

    m_itemInfo = 0;
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

itemInfo* MainScene::querySpriteInMap(CCPoint scrpos)
{
	std::vector<itemInfo>::iterator iter = m_spriteInMap.begin();
    for (; iter != m_spriteInMap.end(); ++iter){
        itemInfo& io = *iter;
        CCPoint pos = io.spr->getPosition();
		CPoint csp(scrpos.x, scrpos.y);
        if (io.tileNum == 1){
            if (IsPtInDiamond(csp, pos.x, pos.y)){
                return &io;
            }
        } else if(io.tileNum == 4){
            if (IsPtIn4Diamond(csp, pos.x, pos.y)){
                return &io;
            }
        }
            /*
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
		*/
    }

    return 0;
}

void MainScene::setSelMapType(int listRow)
{
    m_curSelectedMapType = listRow;
    m_touchSpr = 0;
}

//tile pos -> screen pos -> render pos
void MainScene::drawDiamondMap()
{
    static float an = 1;
    glColor4ub(255, 255, 0, 255);
    glLineWidth(1);
	glPointSize(3);

    for (int tile_y = 0; tile_y < 10; ++tile_y){
        for (int tile_x = 0; tile_x < 10; ++tile_x){
            float x =  (tile_x - tile_y) * m_iGridWidth / 2;
            float y = (tile_x + tile_y) * m_iGridHeigh / 2;
            y = y + m_iGridHeigh * 0.5;

            //x y is within coordinate of Screen, but not call coorScreen2coorRender, as this method be called within coordinate of MapSprite.
            CCPoint renpos = ccp(x, y);//coorScreen2coorRender(ccp(x, y));

            CCPoint a = ccp(renpos.x + m_iGridWidth * 0.5, renpos.y);
            CCPoint b = ccp(renpos.x, renpos.y + m_iGridHeigh * 0.5);
            CCPoint c = ccp(renpos.x - m_iGridWidth * 0.5, renpos.y);
            CCPoint d = ccp(renpos.x, renpos.y -  m_iGridHeigh * 0.5);

            ccDrawLine(a, b);
            ccDrawLine(b, c);
            ccDrawLine(c, d);
            ccDrawLine(d, a);

            // char buf[128] = {0};
            // sprintf(buf, "%d, %d", tile_x, tile_y);
            // CCLabelTTF* lable = CCLabelTTF::labelWithString(buf, "Arial", 14);
				
            // lable->setPosition(renpos);
            // this->addChild(lable);
        }
    }
}

void MainScene::drawMap()
{
	static float an = 1;
    glColor4ub(255, 255, 0, 255);
    glLineWidth(1);
	glPointSize(3);

    int orgx = 480;
    int orgy = 0;
	int mw = 60;
	int mh = 30;

    CCAffineTransform lastform = CCAffineTransformMakeIdentity();
    lastform = CCAffineTransformTranslate(lastform, 300, 220);
    static int offset = 0;
    for (int paintY = m_iGridHeigh * 0.5; paintY <= 320 + m_iGridHeigh; paintY += m_iGridHeigh * 0.5) 
    {
        for (int paintX = m_iGridWidth * 0.5; paintX <= 480 + m_iGridWidth; paintX += m_iGridWidth * 0.5) 
        {
            CPoint pCenter;
            CPoint pos(paintX, paintY);
            if (myGetCursorDiamond(pos, pCenter)){
                // CCPoint rb = ccp(pCenter.x + (m_iGridWidth >> 1), pCenter.y -( m_iGridHeigh >> 1));
                // CCPoint lb = ccp(pCenter.x - ( m_iGridWidth >> 1), pCenter.y - (m_iGridHeigh >> 1));
                // CCPoint lt = ccp(pCenter.x  - ( m_iGridWidth >> 1), pCenter.y + (m_iGridHeigh >> 1));
                // CCPoint rt = ccp(pCenter.x + (m_iGridWidth >> 1), pCenter.y + (m_iGridHeigh >> 1));

				// ccDrawLine(lb, rb);
                // ccDrawLine(rb, rt);
                // ccDrawLine(rt, lt);
                // ccDrawLine(lt, lb);
                
                CCPoint a = ccp(pCenter.x + m_iGridWidth * 0.5, pCenter.y);
                CCPoint b = ccp(pCenter.x, pCenter.y + m_iGridHeigh * 0.5);
                CCPoint c = ccp(pCenter.x - m_iGridWidth * 0.5, pCenter.y);
                CCPoint d = ccp(pCenter.x, pCenter.y -  m_iGridHeigh * 0.5);

				ccDrawLine(a, b);
                ccDrawLine(b, c);
                ccDrawLine(c, d);
                ccDrawLine(d, a);
                
				static int aa = 0;
                int indbex = paintX / (m_iGridWidth * 0.5);
                int indey = paintY / (m_iGridHeigh * 0.5);
		
                int my = (int)(paintY /  (m_iGridHeigh * 0.5)) % 2;
                int mx = (int)(paintX /  (m_iGridWidth * 0.5)) % 2;
                if (((my != 0 && mx != 0)
                     || (my == 0 && mx == 0))){
				
                    int x = getGx(pos.x, pos.y);
                    int y = getGy(pos.x, pos.y);

                    // CPoint tilepos = coorMap2coorLogic(CPoint(pos.x, pos.y));
                    // char buf[128] = {0};
                    // sprintf(buf, "%d, %d", x, y);
                    // CCLabelTTF* lable = CCLabelTTF::labelWithString(buf, "Arial", 14);
				
                    // lable->setPosition(ccp(pos.x , pos.y ));
                    // this->addChild(lable);
                }
	
            }
        }  
    }
}  


void MainScene::useOneTile(int tilex, int tiley)
{
    g_mapBit[tilex][tiley] = true;
}

void MainScene::use4Tile(int tilex, int tiley)
{
    g_mapBit[tilex][tiley] = true;
    g_mapBit[tilex + 1][tiley] = true;
    g_mapBit[tilex + 1][tiley + 1] = true;
    g_mapBit[tilex][tiley + 1] = true;
}

void MainScene::unuseOneTile(int tilex, int tiley)
{
    g_mapBit[tilex][tiley] = false;
}

void MainScene::unuse4Tile(int tilex, int tiley)
{
    g_mapBit[tilex][tiley] = false;
    g_mapBit[tilex + 1][tiley] = false;
    g_mapBit[tilex + 1][tiley + 1] = false;
    g_mapBit[tilex][tiley + 1] = false;
}
