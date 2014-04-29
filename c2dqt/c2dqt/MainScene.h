#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <vector>
#include "cocos2d.h"
#include <QtGui>


USING_NS_CC;

struct CPoint{
	CPoint::CPoint(int _x, int _y):x(_x), y(_y){}
	CPoint(){x = y = 0;};
	int x, y;

};

struct itemInfo
{
    CCSprite* spr;
    int tileNum;

    itemInfo()
        : spr(0)
        , tileNum(1)
        {

        }
};

class MainScene : public cocos2d::CCLayer
{
public:
     MainScene();
     ~MainScene();

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	void setSelMapType(int listRow);

    virtual void registerWithTouchDispatcher(void);
    virtual void draw(void);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(MainScene);


private:
    CCPoint coorScreen2coorRender(CCPoint pos);
    CCSprite* querySpriteInMap(CCPoint scrpos);
    void drawMap();
	void addTile2Map(CCSprite* spr, CPoint pcenter, int rpltile);

private:
	int m_curSelectedMapType;
    CCSprite* m_backgroud;
    CCSprite* m_touchSpr;
    std::vector<itemInfo> m_spriteInMap;
};

#endif // __HELLOWORLD_SCENE_H__
