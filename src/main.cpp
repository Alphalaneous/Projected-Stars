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
	
	bool init(GJGameLevel* level, bool a1) {

		if (!LevelInfoLayer::init(level, a1)) return false;

		bool isPlatformer = level->isPlatformer();
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		
		if(!isPlatformer){

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
		}
		else{

			auto projectedMoonsLabel = CCLabelBMFont::create("-", "bigFont.fnt");
			cocos2d::CCDictionary* statDict = GameStatsManager::sharedState()->m_playerStats;
			int moonCount = statDict->valueForKey("28")->intValue();

			int levelMoonCount = level->m_stars;
			int countToAdd = 0;

			if(level->m_normalPercent != 100){
				countToAdd = levelMoonCount;
			}

			std::string starCountStr = std::to_string(moonCount + countToAdd);
			projectedMoonsLabel->setColor(ccWHITE);
			projectedMoonsLabel->setString(starCountStr.c_str());
			projectedMoonsLabel->setAnchorPoint({0, 0.5});
			projectedMoonsLabel->setPosition({ (winSize.width / 2) + 133.5f, (winSize.height / 2) - 23.0f });
			projectedMoonsLabel->setZOrder(10);
			projectedMoonsLabel->setScale(0.4f);
			projectedMoonsLabel->setTag(84739);

			CCSprite* moonSprite = CCSprite::createWithSpriteFrameName("GJ_bigMoon_001.png");

			moonSprite->setPosition({ (winSize.width / 2) + 118, 136});
			moonSprite->setAnchorPoint({ 0.5f, 0.5f });
			moonSprite->setScale(0.5f);
			moonSprite->setZOrder(3);

			CCSprite* moonSpriteOutline = CCSprite::create("moonOutline.png"_spr);
			moonSpriteOutline->setPosition({ (winSize.width / 2) + 118, 136});
			moonSpriteOutline->setAnchorPoint({ 0.5f, 0.5f });
			moonSpriteOutline->setScale(0.5f);
			moonSpriteOutline->setZOrder(3);

			
			if(levelMoonCount != 0){

				if(level->m_normalPercent == 100){
					projectedMoonsLabel->setColor({100, 255, 255});
					CCParticleSystem* particles = ParticleHelper::createStarParticles(100);
					particles->setPosition({ ((winSize.width / 2) + 116), 136});
					particles->setAnchorPoint({ 0.5f, 0.5f });
					particles->setScale(0.4f);
					particles->setZOrder(3);
					this->addChild(particles);
					this->addChild(moonSprite);
				}
				else{
					this->addChild(moonSpriteOutline);
				}
				
				this->addChild(projectedMoonsLabel);
			}
		}
		return true;
	}
};


