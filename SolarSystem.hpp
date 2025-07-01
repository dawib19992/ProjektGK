#pragma once

#include <raylib.h>
#include <vector>
#include <cmath>


class SolarSystem {
    Vector2 windowSize;
    const float sun_size = 3.0f;
    Vector3 center;
    Camera3D camera;

    std::vector<float> planet_radius, planet_sizes,
        planet_velocities, planet_angle;
    std::vector<Color> colors;

    const float moon_size = 0.5f, moon_radius = 4.0f,
        moon_velocity = 10.0f;
    float moon_angle;
    int earth_pos;

    bool fullscreen;
    bool cameraControlActive;

    void DrawOrbitRing(float radius, Color color);

public:
    SolarSystem();
    void run();
};