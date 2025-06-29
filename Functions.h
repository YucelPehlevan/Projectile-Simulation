#pragma once
#include <SFML/Graphics.hpp>
#include "Vector.h"
#include "Projectile.h"
#include "Variables.h"
using namespace Variables;

namespace Functions {

	void setMaxHeightLine(sf::VertexArray& line, const Vector& vector,sf::Color color = sf::Color::Black,double windowHeight=1000.0, double positionY=100.0);

	void setRangeLine(sf::VertexArray& line, const Vector& vector, sf::Color color = sf::Color::Black, double windowHeight=1000.0,double positionY=100.0,double positionX=0.0);sf::Text createText(sf::Font& font, short int characterSize, sf::Color color = sf::Color::Black, double x = -10.0, double y = -10.0);sf::CircleShape createCircle(double radius, sf::Color color);

	void drawMaxHeight(sf::RenderWindow& window, bool isMaxHeight, sf::VertexArray& maxHeightLine, sf::Text& maxHeightText, sf::CircleShape& projectileMaxHeight);

	void drawFinalRange(sf::RenderWindow& window, bool isMaxRange, sf::VertexArray& maxRangeLine, sf::Text& maxRangeText);

	void drawFirstRange(sf::RenderWindow& window, bool isFirstRange, sf::VertexArray& firstRangeLine, sf::Text& firstRangeText);

	void checkMaxHeight(bool& isMaxHeight, Projectile& projectile, sf::CircleShape& projectileMaxHeight, Vector& maxHeightPosition, sf::VertexArray& maxHeightLine, sf::Text& maxHeightText, sf::Color color = sf::Color::Black, double windowHeight=1000.0, double positionY=100.0);

	void checkFirstRange(bool& isFirstRange, Projectile& projectile, Vector& firstRangePosition, sf::Text& firstRangeText,sf::VertexArray& finalRangeLine, sf::Color color = sf::Color::Black, double windowHeight=1000.0, double positionX=0.0, double positionY=100.0);

	void checkFinalRange(bool& isFinalRange, Projectile& projectile, Vector& finalRangePosition, Vector& velocity, sf::Text& finalRangeText, sf::Text& infoText, sf::VertexArray& finalRangeLine, sf::CircleShape& projectileShape,sf::Color color = sf::Color::Black, double windowHeight=1000.0, double positionX=0.0, double positionY=100.0, double totalTime=0.0);

	void setProjectile(bool isFinalRange, Projectile& projectile, Vector& wind, sf::CircleShape& projectileShape,std::vector<sf::Vector2f>& previousPositions, double& timeAccumulator, double& totalTime,double deltaTime, double timeScale=3.0, double resistanceCoefficient=0.02, double windowHeight=1000.0,double positionY=100.0);

	void updateTextInfo(Projectile& projectile, Vector& maxHeightPosition, Vector& finalRangePosition, Vector& firstRangePosition, sf::Text& infoText, sf::Text& maxHeightText, sf::Text& finalRangeText, double positionX=0.0, double positionY=100.0, double totalTime=0);

	void drawTrajectoryTrace(sf::RenderWindow& window, std::vector<sf::Vector2f>& previousPositions,sf::Color color = sf::Color::Black);

	void updateSimulation(bool& isFinalRange, bool& isMaxHeight, bool& isFirstRange, Projectile& projectile, Vector& wind, sf::CircleShape& projectileShape, sf::CircleShape& projectileMaxHeight,
		std::vector<sf::Vector2f>& previousPositions, double& timeAccumulator, double& totalTime, double deltaTime,double timeScale, double resistanceCoefficient,
		double windowHeight, double positionX, double positionY, Vector& maxHeightPosition, Vector& finalRangePosition, Vector& velocity, sf::VertexArray& maxHeightLine, sf::Text& maxHeightText,
		sf::VertexArray& finalRangeLine, sf::Text& finalRangeText,  sf::Text& infoText, Vector& firstRangePosition, sf::Text& firstRangeText,sf::VertexArray& firstRangeLine,sf::Color color = sf::Color::Black);

	void renderScene(sf::RenderWindow& window,Projectile& projectile, sf::CircleShape& projectileShape, sf::Text& infoText, bool isMaxHeight, bool isFinalRange,
		sf::VertexArray& maxHeightLine, sf::Text& maxHeightText, sf::CircleShape& projectileMaxHeight,
		sf::VertexArray& finalRangeLine, sf::Text& finalRangeText, std::vector<sf::Vector2f>& previousPositions,sf::Font& font,
		sf::VertexArray& firstRangeLine, sf::Text& firstRangeText, sf::Color color = sf::Color::Black);

	void renderSettingMode(sf::RenderWindow& window, Projectile& projectile, sf::CircleShape& projectileShape,sf::Font& font, double speed=50, double angle=45, double positionY=100.0);

	void resetSimulation(Projectile& projectile, Vector& maxHeightPosition, Vector& finalRangePosition,Vector& firstRangePosition, std::vector<sf::Vector2f>& previousPositions);
	
	void drawVelocityArrows(Projectile& proj, sf::RenderWindow& window, bool isSettingMode = false, double settingSpeed = 0, double settingAngle = 0);

	float getScaleFactor(float zoomLevel);

	sf::CircleShape createScaledCircle(double radius, sf::Color color, float scaleFactor);

	sf::Text createScaledText(sf::Font& font, short int characterSize, sf::Color color, double x, double y, float scaleFactor);

	void setScaledLine(sf::VertexArray& line, sf::Vector2f start, sf::Vector2f end, sf::Color color, float scaleFactor);
}
