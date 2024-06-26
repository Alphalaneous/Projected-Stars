#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ParticleHelper.hpp"

using namespace geode::prelude;

class $modify(LevelInfoLayer) {
	
	bool init(GJGameLevel* level, bool a1) {

		if (!LevelInfoLayer::init(level, a1)) return false;

		int levelStarCount = level->m_stars;

		if(levelStarCount == 0) return true;

		bool isPlatformer = level->isPlatformer();
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		std::string spriteName;
		std::string spriteOutlineName;

		auto projectedStarsLabel = CCLabelBMFont::create("-", "bigFont.fnt");
		cocos2d::CCDictionary* statDict = GameStatsManager::sharedState()->m_playerStats;
		int count = 0;

		if(!isPlatformer){
			count = statDict->valueForKey("6")->intValue();
			spriteName = "GJ_bigStar_001.png";
			spriteOutlineName = "starOutline.png"_spr;
		}
		else {
			count = statDict->valueForKey("28")->intValue();
			spriteName = "GJ_bigMoon_001.png";
			spriteOutlineName = "moonOutline.png"_spr;
		}

		int countToAdd = 0;

		if(level->m_normalPercent != 100){
			countToAdd = levelStarCount;
		}

		std::string starCountStr = std::to_string(count + countToAdd);

		projectedStarsLabel->setColor(ccWHITE);
		projectedStarsLabel->setString(starCountStr.c_str());
		projectedStarsLabel->setAnchorPoint({0, 0.5});
		projectedStarsLabel->setPosition({ (winSize.width / 2) + 133.5f, (winSize.height / 2) - 23.0f });
		projectedStarsLabel->setZOrder(10);
		projectedStarsLabel->setScale(0.4f);
		projectedStarsLabel->setID("stars-label"_spr);

		float units = 0;
		bool showStarSprite = true;
		bool showOutline = false;
		CCSprite* starSpriteOutline = CCSprite::create(spriteOutlineName.c_str());
		starSpriteOutline->setPosition({ (winSize.width / 2) + 118, 136});
		starSpriteOutline->setAnchorPoint({ 0.5f, 0.5f });
		starSpriteOutline->setScale(0.5f);
		starSpriteOutline->setZOrder(3);
		starSpriteOutline->setID("stars-sprite-outline"_spr);

		if(!isPlatformer){
			units = level->m_normalPercent * (234.0f/100.0f);

			CCSprite* progressBar = typeinfo_cast<CCSprite*>(getChildByID("normal-mode-bar"));
			CCSprite* progressInside = reinterpret_cast<CCSprite*>(progressBar->getChildren()->objectAtIndex(0));

			if(level->m_normalPercent == 100){
				projectedStarsLabel->setColor(ccYELLOW);
				progressInside->setColor(ccYELLOW);
			}
			if(level->m_normalPercent <= 90) showOutline = true;
		}
		else {
			units = 232;

			if(level->m_normalPercent == 100) showStarSprite = true;
			else showOutline = true;
		}

		float particleAmount = level->m_normalPercent;

		if(level->m_normalPercent < 5 && !isPlatformer) particleAmount = 5;

		CCSprite* starSprite = CCSprite::createWithSpriteFrameName(spriteName.c_str());
		starSprite->setPosition({ (winSize.width / 2) - 118 + units, 136});
		starSprite->setAnchorPoint({ 0.5f, 0.5f });
		starSprite->setScale(0.5f);
		starSprite->setZOrder(3);
		starSprite->setID("stars-sprite"_spr);

		CCParticleSystem* particles = ParticleHelper::createStarParticles(particleAmount);
		particles->setPosition({ (winSize.width / 2) - 118 + units, 136});
		particles->setAnchorPoint({ 0.5f, 0.5f });
		particles->setScale(0.5f);
		particles->setZOrder(3);
		particles->setID("stars-particles"_spr);

		starSprite->setVisible(showStarSprite);
		starSpriteOutline->setVisible(showOutline);

		this->addChild(particles);
		this->addChild(starSprite);
		this->addChild(starSpriteOutline);
		this->addChild(projectedStarsLabel);
		
		return true;
	}
};