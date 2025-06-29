#include "Functions.h"

namespace Functions {
    void setMaxHeightLine(sf::VertexArray& line, const Vector& vector, sf::Color color, double windowHeight, double positionY) {
        line[0].position = sf::Vector2f(vector.getX() + 5, windowHeight - positionY);
        line[1].position = sf::Vector2f(vector.getX() + 5, windowHeight - vector.getY());
        line[0].color = color;
        line[1].color = color;
    }

    void setRangeLine(sf::VertexArray& line, const Vector& vector, sf::Color color, double windowHeight, double positionY, double positionX) {
        line[0].position = sf::Vector2f(positionX, windowHeight - positionY);
        line[1].position = sf::Vector2f(vector.getX(), windowHeight - positionY);
        line[0].color = color;
        line[1].color = color;
    }

    sf::Text createText(sf::Font& font, short int characterSize, sf::Color color, double x, double y) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(characterSize);
        text.setFillColor(color);
        text.setPosition(x, y);
        return text;
    }

    sf::CircleShape createCircle(double radius, sf::Color color) {
        sf::CircleShape circle(radius);
        circle.setFillColor(color);
        return circle;
    }

    void drawMaxHeight(sf::RenderWindow& window, bool isMaxHeight, sf::VertexArray& maxHeightLine, sf::Text& maxHeightText, sf::CircleShape& projectileMaxHeight) {
        if (isMaxHeight) {
            window.draw(maxHeightText);
            window.draw(projectileMaxHeight);
            window.draw(maxHeightLine);
        }
    }

    void drawFinalRange(sf::RenderWindow& window, bool isMaxRange, sf::VertexArray& maxRangeLine, sf::Text& maxRangeText) {
        if (isMaxRange) {
            window.draw(maxRangeLine);
            window.draw(maxRangeText);
        }
    }

    void drawFirstRange(sf::RenderWindow& window, bool isFirstRange, sf::VertexArray& firstRangeLine, sf::Text& firstRangeText) {
        if (isFirstRange) {
            window.draw(firstRangeLine);
            window.draw(firstRangeText);
        }
    }

    void checkMaxHeight(bool& isMaxHeight, Projectile& projectile, sf::CircleShape& projectileMaxHeight, Vector& maxHeightPosition, sf::VertexArray& maxHeightLine, sf::Text& maxHeightText, sf::Color color, double windowHeight, double positionY) {
        if (!isMaxHeight && projectile.getVelocity().getY() <= 0) {
            maxHeightPosition = projectile.getPosition();
            maxHeightText.setPosition(maxHeightPosition.getX() - 50, windowHeight - 20 - maxHeightPosition.getY());
            isMaxHeight = true;
        }
        if (isMaxHeight) {
            projectileMaxHeight.setPosition(maxHeightPosition.getX(), windowHeight - maxHeightPosition.getY());
            setMaxHeightLine(maxHeightLine, maxHeightPosition, color, windowHeight, positionY);
        }
    }

    void checkFirstRange(bool& isFirstRange, Projectile& projectile, Vector& firstRangePosition, sf::Text& firstRangeText,
        sf::VertexArray& firstRangeLine, sf::Color color, double windowHeight, double positionX, double positionY, double totalTime)
    {
        if (!isFirstRange && projectile.getPosition().getY() <= positionY) {
            isFirstRange = true;
            firstTouchTime = totalTime;  // first touch
            firstRangePosition = projectile.getPosition();
            firstRangePosition.setY(positionY);
        }
        if (isFirstRange) {
            double range = firstRangePosition.getX() - positionX;
            firstRangeText.setString("First Range: " + std::to_string(range) + " m");
            firstRangeText.setPosition((firstRangePosition.getX() / 2) - 50, windowHeight - positionY - 50);
            setRangeLine(firstRangeLine, firstRangePosition, color, windowHeight, positionY, positionX);
        }

    }

    void checkFinalRange(bool& isFinalRange, Projectile& projectile, Vector& finalRangePosition, Vector& velocity,
        sf::Text& finalRangeText, sf::Text& infoText, sf::VertexArray& finalRangeLine, sf::CircleShape& projectileShape,
        sf::Color color, double windowHeight, double positionX, double positionY, double totalTime)
    {
        if (!isFinalRange && projectile.getPosition().getY() <= positionY + 5 &&
            projectile.getVelocity().getMagnitude() < 1.0) {

            isFinalRange = true;
            finalRangePosition = projectile.getPosition();

            // Set final position to ground level
            finalRangePosition.setY(positionY);
            projectile.setPosition(finalRangePosition);
            projectile.setVelocity(Vector(0, 0));
        }

        // Always update display elements when final range is reached
        if (isFinalRange) {
            double range = finalRangePosition.getX() - positionX;
            finalRangeText.setString("Final Range: " + std::to_string(range) + " m");
            finalRangeText.setPosition((finalRangePosition.getX() / 2) - 50, windowHeight - positionY - 30);
            setRangeLine(finalRangeLine, finalRangePosition, color, windowHeight, positionY, positionX);

            // Keep projectile at final position
            projectileShape.setPosition(finalRangePosition.getX() - projectileShape.getRadius(),
                windowHeight - positionY - projectileShape.getRadius());
        }
    }

    // Simplified bouncing logic
    void setProjectile(bool isFinalRange, Projectile& projectile, Vector& wind, sf::CircleShape& projectileShape,
        std::vector<sf::Vector2f>& previousPositions, double& timeAccumulator, double& totalTime,
        double deltaTime, double timeScale, double resistanceCoefficient, double windowHeight, double positionY) {

        if (isFinalRange) {
            // Don't update if projectile has reached final range
            return;
        }

        // Update projectile physics
        projectile.updateProjectileWithAirResistance(timeScale * deltaTime, resistanceCoefficient, wind);

        // Add to trajectory trace
        if (timeAccumulator >= 0.2) {
            previousPositions.push_back(projectileShape.getPosition());
            timeAccumulator = 0;
        }
        totalTime += deltaTime * timeScale;

        // Handle ground collision with bouncing
        if (projectile.getPosition().getY() <= positionY && projectile.getVelocity().getY() < 0) {
            Vector velocity = projectile.getVelocity();

            // Bounce with energy loss
            if (std::abs(gravity + 9.807) < 0.001) {
                velocity.setY(-velocity.getY() * 0.6);
                velocity.setX(velocity.getX() * 0.8);
            }
            if (std::abs(gravity + 1.625) < 0.001) {
                velocity.setY(-velocity.getY() * 0.8);
                velocity.setX(velocity.getX() * 0.9);
            }
            if (std::abs(gravity + 3.728) < 0.001) {
                velocity.setY(-velocity.getY() * 0.7);
                velocity.setX(velocity.getX() * 0.85);
            }
            projectile.setVelocity(velocity);

            // Correct position to ground level
            Vector position = projectile.getPosition();
            position.setY(positionY);
            projectile.setPosition(position);
        }

        // Update visual position
        projectileShape.setPosition(projectile.getPosition().getX(), windowHeight - projectile.getPosition().getY());
    }

    void updateTextInfo(Projectile& projectile, Vector& maxHeightPosition, Vector& finalRangePosition, Vector& firstRangePosition, sf::Text& infoText, sf::Text& maxHeightText, sf::Text& finalRangeText, double positionX, double positionY, double totalTime) {
        std::string info = "X: " + std::to_string(projectile.getPosition().getX() - positionX) +
            "\t\t\tFirst Touch Range: " + std::to_string(firstRangePosition.getX() - positionX) +
            "\nY: " + std::to_string(projectile.getPosition().getY() - positionY) +
            "\t\t\t Max Height: " + std::to_string(maxHeightPosition.getY() - positionY) +
            "\nSpeed: " + std::to_string(projectile.getVelocity().getMagnitude()) + " m/s" +
            "\nGravity: " + std::to_string(std::abs(projectile.getAcceleration().getY())) + " m/s^2" +
            "\nTime: " + std::to_string(totalTime) + "\t\tFirst Touch Time: " + std::to_string(firstTouchTime) + " s";
        infoText.setString(info);

        std::string maxHeightInfo = "Max Height: " + std::to_string(maxHeightPosition.getY() - positionY) + " m";
        maxHeightText.setString(maxHeightInfo);

        std::string finalRangeInfo = "Final Range: " + std::to_string(finalRangePosition.getX() - positionX);
        finalRangeText.setString(finalRangeInfo);
    }

    void drawTrajectoryTrace(sf::RenderWindow& window, std::vector<sf::Vector2f>& previousPositions, sf::Color color) {
        sf::VertexArray trajectoryTrace(sf::LineStrip);
        for (auto& position : previousPositions)
            trajectoryTrace.append(sf::Vertex(sf::Vector2f(position.x, position.y), color));
        window.draw(trajectoryTrace);
        trajectoryTrace.clear();
    }

    void updateSimulation(bool& isFinalRange, bool& isMaxHeight, bool& isFirstRange, Projectile& projectile, Vector& wind, sf::CircleShape& projectileShape, sf::CircleShape& projectileMaxHeight,
        std::vector<sf::Vector2f>& previousPositions, double& timeAccumulator, double& totalTime, double deltaTime, double timeScale, double resistanceCoefficient,
        double windowHeight, double positionX, double positionY, Vector& maxHeightPosition, Vector& finalRangePosition, Vector& velocity, sf::VertexArray& maxHeightLine, sf::Text& maxHeightText,
        sf::VertexArray& finalRangeLine, sf::Text& finalRangeText, sf::Text& infoText, Vector& firstRangePosition, sf::Text& firstRangeText, sf::VertexArray& firstRangeLine, sf::Color color) {

        // Update projectile first
        setProjectile(isFinalRange, projectile, wind, projectileShape, previousPositions, timeAccumulator, totalTime, deltaTime, timeScale, resistanceCoefficient, windowHeight, positionY);

        // Check for max height
        checkMaxHeight(isMaxHeight, projectile, projectileMaxHeight, maxHeightPosition, maxHeightLine, maxHeightText, color, windowHeight, positionY);

        //Check for first touch to the ground - totalTime parametresi eklendi
        checkFirstRange(isFirstRange, projectile, firstRangePosition, firstRangeText, firstRangeLine, color, windowHeight, positionX, positionY, totalTime);

        // Check for final range
        checkFinalRange(isFinalRange, projectile, finalRangePosition, velocity, finalRangeText, infoText, finalRangeLine, projectileShape, color, windowHeight, positionX, positionY, totalTime);
    }

    void renderScene(sf::RenderWindow& window, Projectile& projectile, sf::CircleShape& projectileShape, sf::Text& infoText, bool isMaxHeight, bool isFinalRange,
        sf::VertexArray& maxHeightLine, sf::Text& maxHeightText, sf::CircleShape& projectileMaxHeight,
        sf::VertexArray& finalRangeLine, sf::Text& finalRangeText, std::vector<sf::Vector2f>& previousPositions, sf::Font& font,
        sf::VertexArray& firstRangeLine, sf::Text& firstRangeText, sf::Color color) {

        sf::Text menuInfo = createText(font, 20, sf::Color::Black, 1575, 10);
        std::string menu = "Press R for setup mode";
        menuInfo.setString(menu);

        window.draw(menuInfo);
        window.draw(projectileShape);
        drawVelocityArrows(projectile, window, false, projectile.getVelocity().getMagnitude());
        window.draw(infoText);

        drawMaxHeight(window, isMaxHeight, maxHeightLine, maxHeightText, projectileMaxHeight);
        drawFinalRange(window, isFinalRange, finalRangeLine, finalRangeText);
        drawFirstRange(window, isFirstRange, firstRangeLine, firstRangeText);
        drawTrajectoryTrace(window, previousPositions, color);
    }

    void renderSettingMode(sf::RenderWindow& window, Projectile& projectile, sf::CircleShape& projectileShape, sf::Font& font, double speed, double angle, double positionY) {

        sf::Text setupModeInfo = createText(font, 64, sf::Color::Black, 350, 200);
        std::string setupMode = "WELCOME TO THE SETUP MODE";
        setupModeInfo.setString(setupMode);

        sf::Text settingInfo = createText(font, 32, sf::Color::Black, 470, 300);
        std::string info = " The Speed of the Projectile is(1-130): " + std::to_string(speed) + "\n"
            + " The Angle of the Projectile is(1-90): " + std::to_string(angle) + "\n"
            + " The Height of the Projectile is(100-300): " + std::to_string(positionY);
        settingInfo.setString(info);

        sf::Text instructionInfo = createText(font, 32, sf::Color::Black, 470, 450);
        std::string instructiontext = "-----Press enter to start the simulation-----\n"
            "-----Use up and down arrow for the height-----\n"
            "-----Use left and right arrow for the speed-----\n"
            "-----Press A and D for the angle of the projectile-----\n"
            "-----Press K to activate and deactivate the air resistance-----\n"
            "-----Press G to change the gravity of the projectile-----";
        instructionInfo.setString(instructiontext);

        sf::Text airResistanceInfo = createText(font, 32, sf::Color::Black, 1320, 600);
        std::string airResistancetext;
        if (resistanceCoefficient > 0.001) { // if it is bigger than 0.001, it is active
            airResistancetext = "(Air resistance is active: " +
                std::to_string(resistanceCoefficient) + ")";
        }
        else {
            airResistancetext = "(Air resistance is not active)";
        }
        airResistanceInfo.setString(airResistancetext);

        sf::Text gravityInfo = createText(font, 32, sf::Color::Black, 1240, 640);
        const double EPSILON = 0.001;
        std::string gravitytext;

        if (std::abs(gravity + 9.807) < EPSILON)
            gravitytext = "(Gravity mode = Earth)";
        else if (std::abs(gravity + 1.625) < EPSILON)
            gravitytext = "(Gravity mode = Moon)";
        else
            gravitytext = "(Gravity mode = Mars)";
        gravityInfo.setString(gravitytext);

        projectileShape.setPosition(positionX, windowHeight - positionY);
        window.draw(projectileShape);
        drawVelocityArrows(projectile, window, true, speed, angle);

        window.draw(setupModeInfo);
        window.draw(settingInfo);
        window.draw(instructionInfo);
        window.draw(airResistanceInfo);
        window.draw(gravityInfo);
    }

    void resetSimulation(Projectile& projectile, Vector& maxHeightPosition, Vector& finalRangePosition, Vector& firstRangePosition, std::vector<sf::Vector2f>& previousPositions) {
        totalTime = 0;
        timeAccumulator = 0;
        firstTouchTime = 0;  

        isMaxHeight = false;
        isFinalRange = false;
        isFirstRange = false;

        Vector position(positionX, positionY);
        Vector velocity(velocityX, velocityY);
        projectile.setPosition(position);
        projectile.setVelocity(velocity);

        maxHeightPosition.setX(0);
        maxHeightPosition.setY(0);

        finalRangePosition.setX(0);
        finalRangePosition.setY(0);

        firstRangePosition.setX(0);
        firstRangePosition.setY(0);

        previousPositions.clear();

        isSetting = true;
        isSimulationRunning = false;
    }

    void drawVelocityArrows(Projectile& proj, sf::RenderWindow& window, bool isSettingMode, double settingSpeed, double settingAngle) {
        Vector pos = proj.getPosition();
        float arrowLength, angle;

        if (isSettingMode) {
            arrowLength = settingSpeed * 1.5f; // scale the velocity visually
            angle = settingAngle; // degree
        }
        else {
            // use real velocity in normal mode
            Vector vel = proj.getVelocity();
            arrowLength = sqrt(vel.getX() * vel.getX() + vel.getY() * vel.getY()) * 1.5f;
            angle = atan2(vel.getY(), vel.getX()) * 180 / M_PI;
        }

        float posX = pos.getX() + 10;
        float posY = windowHeight - pos.getY() + 10;
        sf::Color arrowColor = sf::Color::Black;

        // body of arrow
        sf::RectangleShape arrow(sf::Vector2f(arrowLength - 10, 3));
        arrow.setPosition(posX, posY);
        arrow.setRotation(-angle);
        arrow.setFillColor(arrowColor);
        if (!isFinalRange)
            window.draw(arrow);

        // Head of arrow
        sf::ConvexShape arrowHead;
        arrowHead.setPointCount(3);
        arrowHead.setPoint(0, sf::Vector2f(15, 0));
        arrowHead.setPoint(1, sf::Vector2f(0, -10));
        arrowHead.setPoint(2, sf::Vector2f(0, 10));

        float radianAngle = -angle * M_PI / 180.0f;
        float headX = posX + (arrowLength - 10) * cos(radianAngle);
        float headY = posY + (arrowLength - 10) * sin(radianAngle);

        arrowHead.setPosition(headX, headY);
        arrowHead.setRotation(-angle);
        arrowHead.setFillColor(arrowColor);
        if (!isFinalRange)
            window.draw(arrowHead);
    }
}
