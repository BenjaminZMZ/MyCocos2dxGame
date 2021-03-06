//
//  PopupUILayer.cpp
//  Tone_Skipper
//
//  Created by 创李 on 15/9/18.
//
//

#include "PopupUILayer.h"
#include "GameConfig.h"
#include "PopupUILayerManager.h"
#include "StatisticsManager.hpp"
USING_NS_CC;
PopupUILayer::PopupUILayer()
{
    m_pPopupType = ePopupInvalid;
    m_cActionType = eNone;
    m_pDarkLayer = nullptr;
    m_nIsPause = true;
    m_nIsBlankClose = true;
    
}
PopupUILayer::~PopupUILayer()
{
    
}
bool PopupUILayer::init()
{
    if (!WrapperUILayer::init()) {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan =  CC_CALLBACK_2(PopupUILayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(PopupUILayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PopupUILayer::onTouchEnded,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}
void PopupUILayer::onEnter()
{
    WrapperUILayer::onEnter();
}
void PopupUILayer::onExit()
{
    WrapperUILayer::onExit();
}
void PopupUILayer::enableDarkLayer(bool bEnable)
{
    if (bEnable)
    {
        if (m_pDarkLayer)
        {
            m_pDarkLayer->setVisible(true);
            m_pDarkLayer->setPosition(cocos2d::Vec2(-100,-100));
        }
        else
        {
            m_pDarkLayer = LayerColor::create(Color4B(0,0,0,150), SCREEN_WIDTH*2, SCREEN_HEIGHT*2);
            //有摄像机的情况需要设置对应的CameraMask
            m_pDarkLayer->setCameraMask((unsigned short)m_nCamerFlag);
            this->addChild(m_pDarkLayer);
            this->setSwallowTouches(true);
            m_pDarkLayer->setPosition(Vec2(-100,-100));
        }
    }else
    {
        if (m_pDarkLayer) {
            
            m_pDarkLayer->setVisible(false);
        }
    }
}
void PopupUILayer::openPopup()
{
    CCLOG("PopupUILayer::openPopup");
    //可以添加暂停监听
    enableDarkLayer(true);
    inAction();
    //只有打开的窗口是第一个窗口时才暂停
    if (m_nIsPause && PopupUILayerManager::getInstance()->getCurrentPopupTypeList().size()==0 &&
        PopupUILayerManager::getInstance()->getSceneType() == PopupUILayerManager::eSceneType::ST_GAME ) {
        schedulerPause();
    }
    if(PopupUILayerManager::getInstance()->getCurrentPopupTypeList().size()==0 &&
       PopupUILayerManager::getInstance()->getSceneType() == PopupUILayerManager::eSceneType::ST_GAME)
         StatisticsManager::getInstance()->pauseSchedu();
}
void PopupUILayer::closePopup()
{
    CCLOG("PopupUILayer::closePopup");
    outAction();
}
void PopupUILayer::inAction()
{
    switch (m_cActionType) {
        case eNone:
            break;
        case eCenterScale:
            inActionCenterScale();
            break;
        case eDownCenter:
            inActionDownCenter();
            break;
        case eTopCenter:
            inActionTopCenter();
            break;
        case eLeftCenter:
            inActionLeftCenter();
            break;
        case eRightCenter:
            inActionRightCenter();
            break;
        default:
            break;
    }
}
void PopupUILayer::outAction()
{
    switch (m_cActionType) {
        case eNone:
            removeSelfCallFunc();
            break;
        case eCenterScale:
            outActionCenterScale();
            break;
        case eDownCenter:
            outActionDownCenter();
            break;
        case eTopCenter:
            outActionTopCenter();
            break;
        case eLeftCenter:
            outActionLeftCenter();
            break;
        case eRightCenter:
            outActionRightCenter();
            break;
        default:
            break;
    }
}
void PopupUILayer::inActionCenterScale()
{
     if (m_pRootNode) {
        m_pRootNode->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        //    root->setPosition(WINDOW_CENTER);
        m_pRootNode->setScale(0.001f);
        
        EaseIn* scale1 = EaseIn::create(ScaleTo::create(0.3f,1.1f *1),0.3f);
        
        ScaleTo* scale2 = ScaleTo::create(0.1f,1.0f * 1);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::inActionCallFunc, this));
        m_pRootNode->runAction(Sequence::create(scale1,scale2, callFunc,nullptr));
    }
}
void PopupUILayer::outActionCenterScale()
{
   
    if (m_pRootNode) {
        ScaleTo* scale1 = ScaleTo::create(0.1f,1.1f * 1);
        EaseOut* scale2 = EaseOut::create(ScaleTo::create(0.4f,0.001f * 1),0.3f);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::removeSelfCallFunc, this));
        m_pRootNode->runAction(Sequence::create(scale1,scale2, callFunc,nullptr));
    }
}

void PopupUILayer::inActionDownCenter()
{
    
    if (m_pRootNode)
    {
        m_pRootNode->setPosition(Vec2(m_pRootNode->getPositionX(), -SCREEN_HEIGHT));
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(m_pRootNode->getPositionX(), SCREEN_HEIGHT*0.5));
        cocos2d::EaseBackOut* inout = cocos2d::EaseBackOut::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::inActionCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));
    }
}
void PopupUILayer::outActionDownCenter()
{
    if (m_pRootNode)
    {
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(m_pRootNode->getPositionX(),-SCREEN_HEIGHT));
        cocos2d::EaseBackIn* inout = cocos2d::EaseBackIn::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::removeSelfCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));
    }
}

void PopupUILayer::inActionTopCenter()
{
    if (m_pRootNode)
    {
        m_pRootNode->setPosition(Vec2(m_pRootNode->getPositionX(), SCREEN_HEIGHT));
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(m_pRootNode->getPositionX(), SCREEN_HEIGHT*0.5));
        cocos2d::EaseBackOut* inout = cocos2d::EaseBackOut::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::inActionCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));
    }
}
void PopupUILayer::outActionTopCenter()
{
    if (m_pRootNode)
    {
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(m_pRootNode->getPositionX(),SCREEN_HEIGHT));
        cocos2d::EaseBackIn* inout = cocos2d::EaseBackIn::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::removeSelfCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));

    }
}

void PopupUILayer::inActionLeftCenter()
{
     if (m_pRootNode)
    {
        m_pRootNode->setPosition(Vec2(-SCREEN_WIDTH, m_pRootNode->getPositionY()));
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(SCREEN_WIDTH*0.5, m_pRootNode->getPositionY()));
        cocos2d::EaseBackOut* inout = cocos2d::EaseBackOut::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::inActionCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));

    }
}
void PopupUILayer::outActionLeftCenter()
{

    if (m_pRootNode)
    {
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(-SCREEN_WIDTH,m_pRootNode->getPositionY()));
        cocos2d::EaseBackIn* inout = cocos2d::EaseBackIn::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::removeSelfCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));
    }
}

void PopupUILayer::inActionRightCenter()
{
    if (m_pRootNode)
    {
        m_pRootNode->setPosition(Vec2(SCREEN_WIDTH, m_pRootNode->getPositionY()));
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(SCREEN_WIDTH*0.5, m_pRootNode->getPositionY()));
        cocos2d::EaseBackOut* inout = cocos2d::EaseBackOut::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::inActionCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));
    }
}
void PopupUILayer::outActionRightCenter()
{

    if (m_pRootNode)
    {
        cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(0.5,Vec2(SCREEN_WIDTH,SCREEN_HEIGHT*0.5));
        cocos2d::EaseBackIn* inout = cocos2d::EaseBackIn::create(moveTo);
        CallFunc* callFunc =  CallFunc::create(CC_CALLBACK_0(PopupUILayer::removeSelfCallFunc, this));
        m_pRootNode->runAction(Sequence::create(inout,callFunc, nullptr));

    }
}

void PopupUILayer::inActionCallFunc()
{
    CCLOG("inActionCallFunc");
}

void PopupUILayer::removeSelfCallFunc()
{
    CCLOG("removeSelfCallFunc");
     PopupUILayerManager::getInstance()->resetPopupType(m_pPopupType);
    
    executeCloseBack();
    executeCloseBackO();
    executeCloseCallbackD();
    
    //暂停事件恢复 只有关闭的窗口是最后一个窗口时才恢复
    if (m_nIsPause && PopupUILayerManager::getInstance()->getCurrentPopUpType() == ePopupInvalid && PopupUILayerManager::getInstance()->getSceneType() == PopupUILayerManager::eSceneType::ST_GAME) {
        schedulerResume();
    }
    if(PopupUILayerManager::getInstance()->getCurrentPopUpType() == ePopupInvalid &&
       PopupUILayerManager::getInstance()->getSceneType() == PopupUILayerManager::eSceneType::ST_GAME)
        StatisticsManager::getInstance()->resumeSchedu();
    
    this->removeFromParent();
}
bool PopupUILayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}
void PopupUILayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return;
}
void PopupUILayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (m_pRootNode &&  !m_pRootNode->getBoundingBox().containsPoint(touch->getLocation()) && m_nIsBlankClose) {
//        CCLOG("onTouchEnded");
        closePopup();
    }
    return;
}
void PopupUILayer::setDarkLayerVisble(bool visble)
{
    if (m_pDarkLayer) {
        m_pDarkLayer->setVisible(visble);
    }
}
void PopupUILayer::onKeyBackClick()
{
    closePopup();
}

