#include "MainScene.h"

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
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(MainScene::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

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
	this->addChild(m_backgroud, 0);

    m_bIsTouchEnabled = true;

	
	return true;
}

void MainScene::registerWithTouchDispatcher(void)
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}

void MainScene::menuCloseCallback(CCObject* pSender)
{
	cocos2d::CCDirector::sharedDirector()->end();
}

bool MainScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint srppos = pTouch->locationInView(0);
    m_touchSpr = querySpriteInMap(srppos);
    if (!m_touchSpr && m_curSelectedMapType > 0){
		char buf[128] = {0};
		sprintf(buf, "map%d.png", m_curSelectedMapType);
	    CCSprite* newmap =  CCSprite::spriteWithFile(buf);
        if (newmap == NULL){
            return false;
        }

        CCPoint pos = pTouch->locationInView(0);
        pos = coorScreen2coorRender(pos);
        newmap->setPosition(pos);
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
        CCPoint glpos = CCDirector::sharedDirector()->convertToGL(scrpos);
        bool iscontain = CCRect::CCRectContainsPoint(sprrect, glpos);
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
