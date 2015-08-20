//
//  Decorator.cpp
//  MazeJump
//
//  Created by wang haibo on 15/7/29.
//
//

#include "Decorator.h"
#include "OutlineEffect3D.h"
#include "RunController.h"
#include "AudioEngine.h"
#include "SdkBoxManager.h"
USING_NS_CC;
using namespace experimental;
Decorator* Decorator::create(DecoratorType type)
{
    TerrainLayer* layer = RunController::getInstance()->getTerrainLayer();
    if(!layer)
        return nullptr;
    auto decorator = new (std::nothrow) Decorator();
    if (decorator)
    {
        decorator->setType(type);
        switch (type) {
            case DT_HEART:
                if(decorator->initWithFile("heart.c3b"))
                {
                    decorator->setScale(0.5);
                    decorator->setPositionY(5);
                }
                break;
            case DT_GOLD:
                if(decorator->initWithFile("goldbig.c3b"))
                {
                    decorator->setScale(0.4);
                    decorator->setPositionY(5);
                }
                break;
            case DT_GOLD_BIG:
                if(decorator->initWithFile("goldbig.c3b"))
                {
                    decorator->setScale(0.7);
                    decorator->setPositionY(5);
                }
                break;
            case DT_BIRD:
                if(decorator->initWithFile("bird.c3b"))
                {
                    decorator->setScale(0.5);
                    decorator->setPositionY(layer->getCellBaseRadius()*2);
                    decorator->setRotation3D(Vec3(0, 90, 0));
                    decorator->setOpacity(0);
                }
                break;
            case DT_PLANE:
                if(decorator->initWithFile("plane.c3b"))
                {
                    decorator->setScale(0.8);
                    decorator->setPositionY(10);
                    decorator->setOpacity(0);
                    decorator->setNeedToUpdate(true);
                }
                break;
            case DT_PORTAL:
                if(decorator->initWithFile("8x8x8.c3b"))
                {
                    decorator->setPositionY(10);
                    decorator->setRotation3D(Vec3(0,45,55));
                }
                break;
            default:
                break;
        }
        decorator->_contentSize = decorator->getBoundingBox().size;
        decorator->m_fRadius = decorator->_contentSize.width*0.5f;
        
        OutlineEffect3D* outline = OutlineEffect3D::create();
        outline->setOutlineColor(Vec3(0.3f, 0.3f, 0.3f));
        outline->setOutlineWidth(0.03f);
        decorator->addEffect(outline, 1);
        
        decorator->setLightMask((unsigned int)LightFlag::LIGHT0);
        
        decorator->autorelease();
        return decorator;
    }
    CC_SAFE_DELETE(decorator);
    return nullptr;
}
Decorator::Decorator()
{
    m_bNeedToCollision = true;
    m_bNeedToUpdate = false;
    m_fTime = 0;
    m_pFakeShadow   = nullptr;
}
Decorator::~Decorator()
{
    if(m_pFakeShadow)
    {
        m_pFakeShadow->removeFromParentAndCleanup(true);
        m_pFakeShadow = nullptr;
    }
}
void Decorator::update(float delta)
{
    TerrainLayer* layer = RunController::getInstance()->getTerrainLayer();
    if(!layer)
        return;
    if(m_bNeedToUpdate)
    {
        if (m_Type == DT_BIRD) {
            float z = getPositionZ();
            float y = getPositionY();
            z += layer->getCellBaseRadius()*4*delta;
            m_fTime += delta;
            y = layer->getCellBaseRadius()*2 + sinf(M_PI*m_fTime)*layer->getCellBaseRadius()*1.5f;
            setPositionY(y);
            setPositionZ(z);
            
            if(m_pFakeShadow)
            {
                Mat4 trans = getNodeToWorldTransform();
                Vec3 pos;
                trans.getTranslation(&pos);
                m_pFakeShadow->setPositionX(pos.x);
                m_pFakeShadow->setPositionZ(pos.z+1);
            }
            
            Runner* runner = RunController::getInstance()->getMainPlayer();
            if(runner && m_bNeedToCollision)
            {
                bool collision = runner->getModifyAABB().intersects(getAABB());
                if(collision)
                {
                    setNeedToCollision(false);
                    if(runner->isSpeedUp())
                    {
                        AudioEngine::play2d("hit.wav", false, 0.5);
                        Vec3 pos;
                        getNodeToWorldTransform().getTranslation(&pos);
                        RunController::getInstance()->addDecoratorExplosion(pos);
                        TerrainPatternLayer* layer = static_cast<TerrainPatternLayer*>(getParent()->getParent());
                        if(layer)
                            layer->eraseDecorator(this);
                    }
                    else
                    {
                        runner->setState(Runner::RS_DEATH);
                        RunController::getInstance()->setGameState(RunController::RGS_GAMEOVER);
                    #if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
                        SdkBoxManager::getInstance()->logEvent("Run Game", "Death", "Hit Bird", 1);
                    #endif
                    }
                }
            }
        }
        else if(m_Type == DT_PLANE)
        {
            Runner* runner = RunController::getInstance()->getMainPlayer();
            if(runner && m_bNeedToCollision)
            {
                bool collision = runner->getModifyAABB().intersects(getAABB());
                if(collision)
                {
                    setNeedToCollision(false);
                    if(runner->isSpeedUp())
                    {
                        AudioEngine::play2d("hit.wav", false, 0.5);
                        Vec3 pos;
                        getNodeToWorldTransform().getTranslation(&pos);
                        RunController::getInstance()->addDecoratorExplosion(pos);
                        TerrainPatternLayer* layer = static_cast<TerrainPatternLayer*>(getParent()->getParent());
                        if(layer)
                            layer->eraseDecorator(this);
                    }
                    else
                    {
                        runner->setState(Runner::RS_DEATH);
                        RunController::getInstance()->setGameState(RunController::RGS_GAMEOVER);
                    #if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
                        SdkBoxManager::getInstance()->logEvent("Run Game", "Death", "Hit Plane", 1);
                    #endif
                    }
                }
            }
        }
    }
}
void Decorator::setFakeShadow(cocos2d::Layer* ownerLayer)
{
    if(!ownerLayer)
        return;
    if(m_pFakeShadow)
    {
        m_pFakeShadow->removeFromParentAndCleanup(true);
        m_pFakeShadow = nullptr;
    }
    m_pFakeShadow = Sprite3D::create("fakeshadow.c3b");
    if(m_pFakeShadow)
    {
        m_pFakeShadow->setCameraMask((unsigned short)CameraFlag::USER1);
        ownerLayer->addChild(m_pFakeShadow, 1);
        Mat4 trans = getNodeToWorldTransform();
        Vec3 pos;
        trans.getTranslation(&pos);
        m_pFakeShadow->setPositionX(pos.x);
        m_pFakeShadow->setPositionZ(pos.z+1);

        m_pFakeShadow->setPositionY(1.5);
        
        m_pFakeShadow->setScale(0.7f);

        m_pFakeShadow->setForceDepthWrite(true);
        m_pFakeShadow->setOpacity(0);
        DelayTime* delay = DelayTime::create(3.0f);
        EaseSineOut* fadeIn = EaseSineOut::create(FadeIn::create(0.5f));
        Sequence* sequence = Sequence::create(delay, fadeIn, NULL);
        m_pFakeShadow->runAction(sequence);
    }
    else
        CCLOG("create fake shadow failed!");
}