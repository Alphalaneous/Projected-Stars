#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "ParticleHelper.hpp"

using namespace geode::prelude;

class $modify(LevelInfoLayer) {
	
	bool init(GJGameLevel* level, bool a1) {
		if (!LevelInfoLayer::init(level, a1)) return false;

		CCSprite* progressBar;
		CCSprite* progressInside;
		if (!level->isPlatformer()) {
			progressBar = typeinfo_cast<CCSprite*>(getChildByID("normal-mode-bar"));
			if (!progressBar) return true;
			progressInside = progressBar->getChildByType<CCSprite*>(0);
			if (!progressInside) return true;
		}

		if (level->m_stars == 0) return true;

		float units = 0;
		bool showStarSprite = false;
		bool showOutline = false;
		int count = 0;
		int countToAdd = 0;

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		std::string spriteName;
		std::string spriteOutlineName;

		auto projectedStarsLabel = CCLabelBMFont::create("-", "bigFont.fnt");
		cocos2d::CCDictionary* statDict = GameStatsManager::sharedState()->m_playerStats;

		if (level->m_normalPercent != 100) {
			countToAdd = level->m_stars;
		}

		projectedStarsLabel->setColor(ccWHITE);
		projectedStarsLabel->setAnchorPoint({0, 0.5});
		projectedStarsLabel->setPosition({ (winSize.width / 2) + 133.5f, 136 });
		projectedStarsLabel->setZOrder(10);
		projectedStarsLabel->setScale(0.4f);
		projectedStarsLabel->setID("stars-label"_spr);

		if (!level->isPlatformer()) {
			std::string value = statDict->valueForKey("6")->m_sString;
			if(value.empty()) count = 0;
			else count = std::stoi(value);
			spriteName = "GJ_bigStar_001.png";
			spriteOutlineName = "starOutline.png"_spr;
			units = level->m_normalPercent * (234.0f/100.0f);
			showStarSprite = true;

			if (level->m_normalPercent == 100) {
				projectedStarsLabel->setColor(ccYELLOW);
				progressInside->setColor(ccYELLOW);
			}
			if (level->m_normalPercent <= 90) showOutline = true;
		}
		else {
			std::string value = statDict->valueForKey("28")->m_sString;
			if(value.empty()) count = 0;
			else count = std::stoi(value);
			spriteName = "GJ_bigMoon_001.png";
			spriteOutlineName = "moonOutline.png"_spr;
			units = 232;

			if (level->m_normalPercent == 100) showStarSprite = true;
			else showOutline = true;
		}
		std::string starCountStr = std::to_string(count + countToAdd);

		CCSprite* starSpriteOutline = CCSprite::create(spriteOutlineName.c_str());
		starSpriteOutline->setPosition({ (winSize.width / 2) + 118, 136});
		starSpriteOutline->setAnchorPoint({ 0.5f, 0.5f });
		starSpriteOutline->setScale(0.5f);
		starSpriteOutline->setZOrder(3);
		starSpriteOutline->setID("stars-sprite-outline"_spr);

		projectedStarsLabel->setString(starCountStr.c_str());

		float particleAmount = level->m_normalPercent;

		if (level->m_normalPercent < 5 && !level->isPlatformer()) particleAmount = 5;

		CCSprite* starSprite = CCSprite::createWithSpriteFrameName(spriteName.c_str());
		starSprite->setPosition({ (winSize.width / 2) - 118 + units, 136});
		starSprite->setAnchorPoint({ 0.5f, 0.5f });
		starSprite->setScale(0.5f);
		starSprite->setZOrder(3);
		starSprite->setID("stars-sprite"_spr);

		if (CCParticleSystem* particles = ParticleHelper::createStarParticles(particleAmount)) {
			particles->setPosition({ (winSize.width / 2) - 118 + units, 136});
			particles->setAnchorPoint({ 0.5f, 0.5f });
			particles->setScale(0.5f);
			particles->setZOrder(3);
			particles->setID("stars-particles"_spr);
			addChild(particles);
		}
		
		starSprite->setVisible(showStarSprite);
		starSpriteOutline->setVisible(showOutline);

		addChild(starSprite);
		addChild(starSpriteOutline);
		addChild(projectedStarsLabel);
		
		return true;
	}
};