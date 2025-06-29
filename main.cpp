#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <iomanip> 
#include "Projectile.h"
#include "Vector.h"
#include "Functions.h"
using namespace Functions;

int main()
{
    // Window settings
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "PROJECTILE SIMULATION");
    window.setFramerateLimit(120);

    // Required Vector and Projectile objects
    Vector velocity(velocityX, velocityY);
    Vector position(positionX, positionY);
    Vector acceleration(0, gravity);
    Vector maxHeightPosition;
    Vector finalRangePosition;
    Vector firstRangePosition;
    Vector wind(0, 0);  // wind effect(optional)

    Projectile projectile(position, velocity, acceleration);

    // SFML objects
    sf::CircleShape projectileShape = createCircle(10.f, sf::Color::Green);
    sf::CircleShape projectileMaxHeight = createCircle(5.f, sf::Color::Black);
    sf::Clock clock; // time object

    // Font and text settings
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.loadFromFile("arial.ttf")) {
            if (!font.loadFromFile("fonts/arial.ttf")) {
                std::cerr << "Font could not be loaded! Using default font." << std::endl;
                // Continue with default font
            }
        }
    }

    sf::Text infoText = createText(font, 16, sf::Color::Black, 10.f, 10.f);
    sf::Text maxHeightText = createText(font, 16, sf::Color::Black, -10.f, -10.f);
    sf::Text finalRangeText = createText(font, 16, sf::Color::Black, -10.f, -10.f);
    sf::Text firstRangeText = createText(font, 16, sf::Color::Black, -10.f, -10.f);

    sf::VertexArray maxHeightLine(sf::Lines, 2);
    sf::VertexArray finalRangeLine(sf::Lines, 2);
    sf::VertexArray firstRangeLine(sf::Lines, 2);

    std::vector<sf::Vector2f> previousPositions;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Calculate deltaTime
        double deltaTime = clock.restart().asSeconds();
        static double inputTimer = 0;
        inputTimer += deltaTime;

        if (isSetting) {
            // Input throttling 
            if (inputTimer > 0.15) {
                bool inputDetected = false;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    positionY += 5;
                    positionY = std::clamp(positionY, 100.0, 300.0);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    positionY -= 5;
                    positionY = std::clamp(positionY, 100.0, 300.0);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    userSpeed += 5;
                    userSpeed = std::clamp(userSpeed, 1.0, 130.0);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    userSpeed -= 5;
                    userSpeed = std::clamp(userSpeed, 1.0, 130.0);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    userAngle += 1;
                    userAngle = std::clamp(userAngle, 1.0, 90.0);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    userAngle -= 1;
                    userAngle = std::clamp(userAngle, 1.0, 90.0);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
                    
                    if (std::abs(gravity - (-9.807)) < 0.001) {
                        // toggle air resistance
                        resistanceCoefficient = (resistanceCoefficient > 0.001) ? 0.0 : 0.02;
                    }
                    else if (std::abs(gravity - (-1.625)) < 0.001) {
                        resistanceCoefficient = 0.0;
                    }
                    else if (std::abs(gravity - (-3.728)) < 0.001) {
                        // toggle air resistance
                        resistanceCoefficient = (resistanceCoefficient > 0.001) ? 0.0 : 0.006;
                    }
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                    // switch the gravity(Earth-Moon-Mars)
                    if (std::abs(gravity - (-9.807)) < 0.001) {
                        gravity = -1.625;  // Earth -> Moon
                        resistanceCoefficient = 0.0;  // no atmosphere in Moon
                    }
                    else if (std::abs(gravity - (-1.625)) < 0.001) {
                        gravity = -3.728;  // Moon -> Mars
                        resistanceCoefficient = 0.006; // weak atmosphere in Mars
                    }
                    else if (std::abs(gravity - (-3.728)) < 0.001) {
                        gravity = -9.807;  // Mars -> Earth
                        resistanceCoefficient = 0.02;  // Earth
                    }
                    else {
                        gravity = -9.807;  // Earth by default
                        resistanceCoefficient = 0.02;
                    }

                    projectile.setAcceleration(gravity);
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    isSetting = false;
                    isSimulationRunning = true;
                    float radianAngle = userAngle * M_PI / 180.0f;
                    velocityX = userSpeed * cos(radianAngle);
                    velocityY = userSpeed * sin(radianAngle);

                    // set the initial values
                    Vector newPosition(positionX, positionY);
                    Vector newVelocity(velocityX, velocityY);
                    projectile.setPosition(newPosition);
                    projectile.setVelocity(newVelocity);
                    inputDetected = true;
                }

                if (inputDetected) {
                    inputTimer = 0; // set timer

                    // Pozisyonu güncelle
                    Vector newPosition(positionX, positionY);
                    projectile.setPosition(newPosition);
                }
            }
            // show menu
        }
        else if (isSimulationRunning) {
            timeAccumulator += deltaTime * timeScale;

            updateTextInfo(projectile, maxHeightPosition, finalRangePosition, firstRangePosition, infoText, maxHeightText, finalRangeText, positionX, positionY, totalTime);

            updateSimulation(isFinalRange, isMaxHeight, isFirstRange, projectile, wind, projectileShape, projectileMaxHeight,
                previousPositions, timeAccumulator, totalTime, deltaTime, timeScale, resistanceCoefficient,
                windowHeight, positionX, positionY, maxHeightPosition, finalRangePosition, velocity,
                maxHeightLine, maxHeightText, finalRangeLine, finalRangeText, infoText, firstRangePosition, firstRangeText, firstRangeLine, sf::Color::Black);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                // When press R return to the menu
                resetSimulation(projectile, maxHeightPosition, finalRangePosition, firstRangePosition, previousPositions);
                isSetting = true;
                isSimulationRunning = false;
            }
        }

        window.clear(sf::Color::White);

        if (isSetting) {
            // different render in setting mode
            renderSettingMode(window, projectile, projectileShape, font, userSpeed, userAngle, positionY);
        }
        else {
            // Normal render
            renderScene(window, projectile, projectileShape, infoText, isMaxHeight, isFinalRange, maxHeightLine, maxHeightText,
                projectileMaxHeight, finalRangeLine, finalRangeText, previousPositions, font, firstRangeLine, firstRangeText, sf::Color::Black);
        }

        window.display();
    }

    return 0;
}

