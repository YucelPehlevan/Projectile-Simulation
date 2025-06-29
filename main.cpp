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
    // Try multiple font paths for better compatibility
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

        // Delta time hesapla (input throttling için)
        double deltaTime = clock.restart().asSeconds();
        static double inputTimer = 0;
        inputTimer += deltaTime;

        if (isSetting) {
            // Input throttling (0.1 saniyede bir)
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
                    // Mevcut gezegen türüne göre hava direncini toggle et
                    if (std::abs(gravity - (-9.807)) < 0.001) {
                        // Earth - 0.02 ile 0.0 arasýnda toggle
                        resistanceCoefficient = (resistanceCoefficient > 0.001) ? 0.0 : 0.02;
                    }
                    else if (std::abs(gravity - (-1.625)) < 0.001) {
                        // Moon - Normalde hava direnci yok ama test için eklenebilir
                        resistanceCoefficient = 0.0;
                    }
                    else if (std::abs(gravity - (-3.728)) < 0.001) {
                        // Mars - 0.006 ile 0.0 arasýnda toggle
                        resistanceCoefficient = (resistanceCoefficient > 0.001) ? 0.0 : 0.006;
                    }
                    inputDetected = true;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                    // Mevcut gravity deðerine göre bir sonrakine geç
                    if (std::abs(gravity - (-9.807)) < 0.001) {
                        gravity = -1.625;  // Earth -> Moon
                        resistanceCoefficient = 0.0;  // Ay'da atmosfer yok
                    }
                    else if (std::abs(gravity - (-1.625)) < 0.001) {
                        gravity = -3.728;  // Moon -> Mars
                        resistanceCoefficient = 0.006;  // Mars'ta ince atmosfer
                    }
                    else if (std::abs(gravity - (-3.728)) < 0.001) {
                        gravity = -9.807;  // Mars -> Earth
                        resistanceCoefficient = 0.02;  // Dünya'da yoðun atmosfer
                    }
                    else {
                        gravity = -9.807;  // Default olarak Earth'e ayarla
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

                    // Baþlangýç deðerlerini ayarla
                    Vector newPosition(positionX, positionY);
                    Vector newVelocity(velocityX, velocityY);
                    projectile.setPosition(newPosition);
                    projectile.setVelocity(newVelocity);
                    inputDetected = true;
                }

                if (inputDetected) {
                    inputTimer = 0; // Timer'ý sýfýrla

                    // Pozisyonu güncelle
                    Vector newPosition(positionX, positionY);
                    projectile.setPosition(newPosition);
                }
            }

            // Setting modunda bilgi göster
            // (Bu kýsmý sonra ekleyeceðiz - text rendering)
        }
        else if (isSimulationRunning) {
            timeAccumulator += deltaTime * timeScale;

            updateTextInfo(projectile, maxHeightPosition, finalRangePosition, firstRangePosition, infoText, maxHeightText, finalRangeText, positionX, positionY, totalTime);

            updateSimulation(isFinalRange, isMaxHeight, isFirstRange, projectile, wind, projectileShape, projectileMaxHeight,
                previousPositions, timeAccumulator, totalTime, deltaTime, timeScale, resistanceCoefficient,
                windowHeight, positionX, positionY, maxHeightPosition, finalRangePosition, velocity,
                maxHeightLine, maxHeightText, finalRangeLine, finalRangeText, infoText, firstRangePosition, firstRangeText, firstRangeLine, sf::Color::Black);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                // Reset ve setting moduna dön
                resetSimulation(projectile, maxHeightPosition, finalRangePosition, firstRangePosition, previousPositions);
                isSetting = true;
                isSimulationRunning = false;
            }
        }

        window.clear(sf::Color::White);

        if (isSetting) {
            // Setting modunda farklý render
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

