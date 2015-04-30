//
//  StageManager.h
//  TinyFlare
//
//  Created by wang haibo on 15/4/24.
//
//

#ifndef __TinyFlare__StageManager__
#define __TinyFlare__StageManager__

#include "cocos2d.h"
#include "ChaosNumber.h"

class StageManager : public cocos2d::Ref
{
    StageManager();
    virtual ~StageManager();
public:
    typedef enum{
        ST_DEFAULT = 0,                     ///击毁积分
        ST_UNKNOWN
    } StageType;
    typedef enum{
        EUT_TIME = 0,                     ///时间刷新
        EUT_NUM,                          ///个数刷新
        EUT_PERCENT_BY_TIME,              ///进度百分比时间刷新
        EUT_PERCENT_BY_NUM,               ///进度百分比个数刷新
        EUT_UNKNOWN
    } EnemyUpdateType;
    static StageManager* getInstance();
    bool init();
    void reset();
    void nextStage(int stage);
    
    void addScore(int score);
    
    float getCurrentPercent() { return m_fCurrentPercent.GetFloatValue(); }
    
private:
    ChaosNumber              m_nCurrentIndex;
    ChaosNumber              m_nCurrentScore;
    ChaosNumber              m_fCurrentPercent;
    ChaosNumber              m_fLastPercent;
    ChaosNumber              m_nTargetScore;
    
    cocos2d::ValueVector   m_StageList;
};

#endif /* defined(__TinyFlare__StageManager__) */
