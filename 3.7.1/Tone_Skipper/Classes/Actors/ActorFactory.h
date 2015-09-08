//
//  ActorFactory.h
//  Tone_Skipper
//
//  Created by wang haibo on 15/9/7.
//
//

#ifndef __Tone_Skipper__ActorFactory__
#define __Tone_Skipper__ActorFactory__

#include "Player.h"
class ActorFactory : public cocos2d::Ref
{
    ActorFactory();
    virtual ~ActorFactory();
public:
    static ActorFactory* getInstance();
    
    Player* createPlayer(Player::PlayerType type);
};

#endif /* defined(__Tone_Skipper__ActorFactory__) */
