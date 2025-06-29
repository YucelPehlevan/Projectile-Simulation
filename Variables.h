#pragma once
namespace Variables {
    constexpr double windowWidth = 1800.0;
    constexpr double windowHeight = 1000.0;
    constexpr double positionX = 10.0;
    inline double timeScale = 3.0;  // affecting only visual speed

    // Setting Projectile
    inline double userSpeed = 50;
    inline double userAngle = 45;
    inline double velocityX = 120.0;
    inline double velocityY = 50.0;
    inline double positionY = 100.0;
    inline double resistanceCoefficient = 0.02;
    inline double gravity = -9.807;

    // inline for global variables (min C++17 required)
    inline double totalTime = 0.0;
    inline double timeAccumulator = 0.0;
    inline double firstTouchTime = 0.0;  // variable for first touch to the ground
    inline bool isMaxHeight = false;
    inline bool isFirstRange = false;
    inline bool isFinalRange = false;

    // Setting Projectile
    inline bool isSetting = true;
    inline bool isSimulationRunning = false;
}
