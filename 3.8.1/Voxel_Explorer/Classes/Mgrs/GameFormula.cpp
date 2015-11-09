//
//  GameFormula.cpp
//  Voxel_Explorer
//
//  Created by wang haibo on 15/10/20.
//
//

#include "GameFormula.hpp"
#include "AlisaMethod.h"
USING_NS_CC;

static const CChaosNumber gsMaxBlockRate = 0.2f;
static const CChaosNumber gsMaxCriticalStrikeRate = 0.3f;
static const CChaosNumber gsMaxDodgeRate = 0.2f;
static const CChaosNumber gsMaxMagicItemFindRate = 0.65f;
bool GameFormula::generateMagicItem(float magicFindRate)
{
    float percentNoMagic = 1.0 - magicFindRate;
    AlisaMethod* am = AlisaMethod::create(magicFindRate, percentNoMagic, -1.0, NULL);
    if(!am)
        return false;
    return am->getRandomIndex() == 0;
}
bool GameFormula::generatePickItem(bool isElite, bool isBoss)
{
    if(isElite)
        return true;
    else if(isBoss)
        return true;
    else
    {
        float percentOne = 0.3f;
        float percentZero = 1.0 - percentOne;
        AlisaMethod* am = AlisaMethod::create(percentOne, percentZero, -1.0, NULL);
        if(!am)
            return false;
        return am->getRandomIndex() == 0;
    }
}
int GameFormula::getNextLevelExp(int currentLevel)
{
    int exp = floor((pow((currentLevel-1), 3)+60)/5*((currentLevel-1)*2+60)+60);
    int mod = exp%50;
    return exp - mod;
}
int GameFormula::getKillNormalMonsterExp(int currentLevel, int monsterLevel)
{
    int monsterExp = (monsterLevel-1)*2+60;
    int diff = monsterLevel - currentLevel;
    if(diff>=0)
    {
        if(diff>=5)
            monsterExp = monsterExp*2;
        else
            monsterExp = monsterExp*pow(1.15f, diff);
    }
    else
    {
        if(diff<=-5)
            monsterExp = 0;
        else
            monsterExp = monsterExp*pow(0.8f, diff);
    }
    return monsterExp;
}
int GameFormula::getKillEliteMonsterExp(int currentLevel, int monsterLevel)
{
    int monsterExp = (monsterLevel-1)*2+60;
    int diff =  monsterLevel - currentLevel;
    if(diff>=0)
    {
        if(diff>=5)
            monsterExp = monsterExp*6;
        else
            monsterExp = monsterExp*pow(1.5f, diff);
    }
    else
    {
        if(diff<=-8)
            monsterExp = 0;
        else
            monsterExp = monsterExp*pow(0.85f, diff);
    }
    return monsterExp;
}
int GameFormula::getKillBossExp(int currentLevel, int bossLevel)
{
    return (bossLevel-1)*20+100;
}

void GameFormula::exchangeMoney(CChaosNumber srcCopper, CChaosNumber& gold, CChaosNumber& silver, CChaosNumber& copper)
{
    gold = srcCopper/10000;
    silver = (srcCopper.GetLongValue()%10000)/100;
    copper = srcCopper.GetLongValue()%100;
}
CChaosNumber GameFormula::exchangeMoney(CChaosNumber srcGold, CChaosNumber srcSilver, CChaosNumber srcCopper)
{
    return srcGold*100*100 + srcSilver*100 + srcCopper*1;
}