#include "solarsystem.hpp"

void SolarSystem::DrawOrbitRing(float radius, Color color) {
    const int segments = 100;
    for (int i = 0; i < segments; i++) {
        float angle1 = 2.0f * PI * i / segments;
        float angle2 = 2.0f * PI * (i + 1) / segments;

        Vector3 start = {
            center.x + cos(angle1) * radius,
            center.y,
            center.z + sin(angle1) * radius
        };

        Vector3 end = {
            center.x + cos(angle2) * radius,
            center.y,
            center.z + sin(angle2) * radius
        };

        DrawLine3D(start, end, color);
    }
}

SolarSystem::SolarSystem() {
    Vector2 windowSize = { 1920, 1080 };
    InitWindow(windowSize.x, windowSize.y, "Solar System 3D");
    SetTargetFPS(60);

    // Ustawienia kamery
    camera.position = { 50.0f, 50.0f, 50.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    center = { 0.0f, 0.0f, 0.0f };

    // Parametry planet
    planet_radius = { 8.0f, 11.0f, 16.5f, 22.5f, 31.0f, 43.0f, 51.5f, 56.5f };
    planet_velocities = { 1.607f, 1.174f, 1.0f, 0.802f, 0.434f, 0.323f, 0.228f, 0.182f };
    planet_sizes = { 1.0f, 1.5f, 2.0f, 1.8f, 6.0f, 5.5f, 2.5f, 2.2f };
    colors = {
        {115, 147, 179, 255},
        {255, 87, 51, 255},
        {30, 144, 255, 255},
        {178, 34, 34, 255},
        {210, 105, 30, 255},
        {220, 20, 60, 255},
        {72, 209, 204, 255},
        {65, 105, 225, 255}
    };
    planet_angle.assign(8, 0);

    moon_angle = 0.0f;
    for (size_t i = 0; i < planet_sizes.size(); ++i) {
        if (planet_sizes[i] == 2.0f) {
            earth_pos = i;
        }
    }

    fullscreen = false;
    cameraControlActive = false;
}

void SolarSystem::run() {
    while (!WindowShouldClose()) {
        // Obsługa pełnego ekranu
        if (IsKeyPressed(KEY_F11)) {
            fullscreen = !fullscreen;
            if (fullscreen) {
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                ToggleFullscreen();
            }
            else {
                ToggleFullscreen();
                SetWindowSize(1920, 1080);
            }
        }

        // Sterowanie kamerą tylko przy przytrzymanym 'C'
        if (IsKeyDown(KEY_C)) {
            if (!cameraControlActive) {
                cameraControlActive = true;
                DisableCursor();
            }

            // Aktualizacja kamery w trybie FREE
            float rotationSpeed = 0.03f;
            float moveSpeed = 0.3f;

            // Poruszanie kamerą w globalnych osiach
            if (IsKeyDown(KEY_W)) camera.position.y += moveSpeed;
            if (IsKeyDown(KEY_S)) camera.position.y -= moveSpeed;
            if (IsKeyDown(KEY_A)) camera.position.x -= moveSpeed;
            if (IsKeyDown(KEY_D)) camera.position.x += moveSpeed;

            Vector2 mouseDelta = GetMouseDelta();
            camera.target.x += mouseDelta.x * rotationSpeed;
            camera.target.y -= mouseDelta.y * rotationSpeed;
        }
        else if (cameraControlActive) {
            cameraControlActive = false;
            EnableCursor();
        }

        // Aktualizacja pozycji planet
        for (size_t i = 0; i < planet_radius.size(); ++i) {
            planet_angle[i] += planet_velocities[i] * 0.02f;
        }
        moon_angle += moon_velocity * 0.009f;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        // Rysowanie Słońca
        DrawSphere(center, sun_size, YELLOW);

        // Rysowanie planet i orbit
        for (size_t i = 0; i < planet_radius.size(); ++i) {
            DrawOrbitRing(planet_radius[i], GRAY);

            Vector3 planet_pos = {
                center.x + cos(planet_angle[i]) * planet_radius[i],
                center.y,
                center.z + sin(planet_angle[i]) * planet_radius[i]
            };
            DrawSphere(planet_pos, planet_sizes[i], colors[i]);

            // Księżyc dla Ziemi
            if ((int)i == earth_pos) {
                Vector3 moon_pos = {
                    planet_pos.x + cos(moon_angle) * moon_radius,
                    planet_pos.y,
                    planet_pos.z + sin(moon_angle) * moon_radius
                };
                DrawSphere(moon_pos, moon_size, LIGHTGRAY);
            }
        }

        EndMode3D();

        // Informacje sterowania
        DrawText("Solar System 3D", 10, 10, 20, WHITE);
        DrawText("Hold 'C' to move camera (WASD + Mouse)", 10, 40, 20, WHITE);
        DrawText("W: Up, S: Down, A: Left, D: Right", 10, 70, 20, WHITE);
        DrawText("Mouse: Rotate view", 10, 100, 20, WHITE);
        DrawText("Press F11 to toggle fullscreen", 10, 130, 20, WHITE);
        DrawFPS(10, 160);

        EndDrawing();
    }
    CloseWindow();
}