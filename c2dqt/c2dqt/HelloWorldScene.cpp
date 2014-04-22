#include "HelloWorldScene.h"

#define BACKGROUD_SCALE 1.5

HelloWorld::HelloWorld()
    : m_backgroud(0)
{

}

HelloWorld::~HelloWorld()
{

}

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
										menu_selector(HelloWorld::menuCloseCallback) );
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

void HelloWorld::registerWithTouchDispatcher(void)
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	cocos2d::CCDirector::sharedDirector()->end();
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_backgroud){
        return;
    }

    CCPoint pos = pTouch->locationInView(0);
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

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void HelloWorld::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}
