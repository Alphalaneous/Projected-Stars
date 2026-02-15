#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(LevelInfoLayer) {
	
	bool init(GJGameLevel* level, bool a1) {
		if (!LevelInfoLayer::init(level, a1)) return false;
		if (level->m_stars == 0) return true;

		CCSprite* progressInside;

		if (!level->isPlatformer()) {
			auto progressBar = typeinfo_cast<CCSprite*>(getChildByID("normal-mode-bar"));
			if (!progressBar) return true;

			progressInside = progressBar->getChildByType<CCSprite*>(0);
			if (!progressInside) return true;
		}

		int countToAdd = 0;

		auto winSize = CCDirector::get()->getWinSize();

		auto statDict = GameStatsManager::get()->m_playerStats;

		if (level->m_normalPercent != 100) {
			countToAdd = level->m_stars;
		}

		auto projectedStarsLabel = CCLabelBMFont::create("-", "bigFont.fnt");
		projectedStarsLabel->setColor({255, 255, 255});
		projectedStarsLabel->setAnchorPoint({0, 0.5});
		projectedStarsLabel->setPosition({ (winSize.width / 2) + 133.5f, 136 });
		projectedStarsLabel->setZOrder(10);
		projectedStarsLabel->setScale(0.4f);
		projectedStarsLabel->setID("stars-label"_spr);

		std::string value = level->isPlatformer() ? statDict->valueForKey("28")->getCString() : statDict->valueForKey("6")->getCString();
		int count = numFromString<int>(value).unwrapOr(0);
		float units = level->isPlatformer() ? 232 : level->m_normalPercent * (234.0f/100.0f);

		bool showStarSprite = false;
		bool showOutline = false;

		if (!level->isPlatformer()) {
			showStarSprite = true;

			if (level->m_normalPercent == 100) {
				projectedStarsLabel->setColor(ccYELLOW);
				progressInside->setColor(ccYELLOW);
			}
			if (level->m_normalPercent <= 90) {
				showOutline = true;
			}
		}
		else {
			if (level->m_normalPercent == 100) {
				showStarSprite = true;
			}
			else {
				showOutline = true;
			}
		}

		std::string type = level->isPlatformer() ? "Moon" : "Star";
		std::string spriteName = fmt::format("GJ_big{}_001.png", type);
		std::string spriteOutlineName = fmt::format("{}Outline.png"_spr, utils::string::toLower(type));

		auto starSpriteOutline = CCSprite::create(spriteOutlineName.c_str());
		starSpriteOutline->setPosition({ (winSize.width / 2) + 118, 136});
		starSpriteOutline->setAnchorPoint({ 0.5f, 0.5f });
		starSpriteOutline->setScale(0.5f);
		starSpriteOutline->setZOrder(3);
		starSpriteOutline->setID("stars-sprite-outline"_spr);

		projectedStarsLabel->setString(numToString(count + countToAdd).c_str());

		float particleAmount = level->m_normalPercent;

		if (level->m_normalPercent < 5 && !level->isPlatformer()) particleAmount = 5;
		if (level->isPlatformer()) particleAmount = 50;

		auto starSprite = CCSprite::createWithSpriteFrameName(spriteName.c_str());
		starSprite->setPosition({ (winSize.width / 2) - 118 + units, 136});
		starSprite->setAnchorPoint({ 0.5f, 0.5f });
		starSprite->setScale(0.5f);
		starSprite->setZOrder(3);
		starSprite->setID("stars-sprite"_spr);

		int particleID = level->isPlatformer() ? 101 : 4;

		auto particles = GameToolbox::particleFromString(fmt::format("100a-1a1a1a33a0a360a20a5a10a10a0a0a0a0a0a0a15a5a0a360a1a1a1a1a1a1a1a0a5a0a0a360a1a1a1a1a1a1a0a0a0a0a0a0a100a0a0a0a0a0a0a2a1a0a0a0a{}a0a0a0a0a0a0a0a0a0a0a5a0a5a0", particleID).c_str(), CCParticleSystemQuad::create(), false);
		particles->setTotalParticles(particleAmount / 2);

		auto offset = level->isPlatformer() ? CCPoint{-3 , -1} : CCPoint{0, 0};

		particles->setPosition(CCPoint{(winSize.width / 2) - 118 + units , 136} + offset);
		particles->setAnchorPoint({ 0.5f, 0.5f });
		particles->setScale(0.5f);
		particles->setZOrder(3);
		particles->setID("stars-particles"_spr);
		addChild(particles);

		starSprite->setVisible(showStarSprite);
		starSpriteOutline->setVisible(showOutline);

		addChild(starSprite);
		addChild(starSpriteOutline);
		addChild(projectedStarsLabel);
		
		return true;
	}
};