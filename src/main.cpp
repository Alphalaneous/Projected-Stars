#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ParticleHelper.hpp"

using namespace geode::prelude;

template<typename Base, typename T>
bool instanceof(const T ptr) {
    return dynamic_cast<const Base>(ptr) != nullptr;
}

template<typename O, typename T>
T getFromObjectPos(CCObject* obj, CCPoint point) {
    CCObject* pObj = nullptr;

    CCARRAY_FOREACH(dynamic_cast<O>(obj)->getChildren(), pObj) {
        CCNode* currentNode = (CCNode*)pObj;
        CCPoint xy = currentNode->getPosition();

        T obj = dynamic_cast<T>(pObj);

        if (instanceof<T>(obj)) {
            if (point.x == xy.x && point.y == xy.y) {
                return obj;
            }
        }
    }
    return nullptr;
}

class $modify(LevelInfoLayer) {
	
	bool init(GJGameLevel* level) {

		if (!LevelInfoLayer::init(level)) return false;

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		auto projectedStarsLabel = CCLabelBMFont::create("-", "bigFont.fnt");
		cocos2d::CCDictionary* statDict = GameStatsManager::sharedState()->m_playerStats;
		int starCount = statDict->valueForKey("6")->intValue();

		int levelStarCount = level->m_stars;
		int countToAdd = 0;

		if(level->m_normalPercent != 100){
			countToAdd = levelStarCount;
		}

		std::string starCountStr = std::to_string(starCount + countToAdd);
		projectedStarsLabel->setColor(ccWHITE);
		projectedStarsLabel->setString(starCountStr.c_str());
		projectedStarsLabel->setAnchorPoint({0, 0.5});
		projectedStarsLabel->setPosition({ (winSize.width / 2) + 133.5f, (winSize.height / 2) - 23.0f });
		projectedStarsLabel->setZOrder(10);
		projectedStarsLabel->setScale(0.4f);
		projectedStarsLabel->setTag(84739);

		CCSprite* progressBar = getFromObjectPos<CCLayer*, CCSprite*>(this, {(winSize.width / 2), 136});

		CCSprite* progressInside = reinterpret_cast<CCSprite*>(progressBar->getChildren()->objectAtIndex(0));

		
		//progress bar center x, 136 y
		
		//168.5 start
		//400.5 end
		//232 units total

		//116 sub

		//284.5 to 430, 145.5
		//160 to 137, 23


		float particleAmount = level->m_normalPercent;

		if(level->m_normalPercent < 5){
			particleAmount = 5;
		}

		CCParticleSystem* particles = ParticleHelper::createStarParticles(particleAmount);
		CCSprite* starSprite = CCSprite::createWithSpriteFrameName("GJ_bigStar_001.png");

		float units = level->m_normalPercent * (234.0f/100.0f);

		starSprite->setPosition({ ((winSize.width / 2) - 118) + units, 136});
		starSprite->setAnchorPoint({ 0.5f, 0.5f });
		starSprite->setScale(0.5f);
		starSprite->setZOrder(3);

		particles->setPosition({ ((winSize.width / 2) - 118) + units, 136});
		particles->setAnchorPoint({ 0.5f, 0.5f });
		particles->setScale(0.5f);
		particles->setZOrder(3);

		CCSprite* starSpriteOutline = CCSprite::create("starOutline.png"_spr);
		starSpriteOutline->setPosition({ (winSize.width / 2) + 118, 136});
		starSpriteOutline->setAnchorPoint({ 0.5f, 0.5f });
		starSpriteOutline->setScale(0.5f);
		starSpriteOutline->setZOrder(3);

		if(levelStarCount != 0){

			if(level->m_normalPercent == 100){
				projectedStarsLabel->setColor(ccYELLOW);
				progressInside->setColor(ccYELLOW);
			}

			if(level->m_normalPercent <= 90){
				this->addChild(starSpriteOutline);
			}

			this->addChild(particles);
			this->addChild(starSprite);
			this->addChild(projectedStarsLabel);
		}

		return true;
	}
};


