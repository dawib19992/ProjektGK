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
    Vector2 windowSize = { 1280, 960 };
    InitWindow(windowSize.x, windowSize.y, "Solar System 3D");
    SetTargetFPS(60);

    camera.position = { 0.0f, 50.0f, 100.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    previousMousePosition = { 0, 0 };
    firstMouseMove = true;
    cameraYaw = PI;
    cameraPitch = 0.3f;

    center = { 0.0f, 0.0f, 0.0f };

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

    sunTexture = LoadTexture("resources/sun.jpg");
    moonTexture = LoadTexture("resources/moon.jpg");
    planetTextures.resize(8);
    for (int i = 0; i < 8; i++) {
        std::string filename = "resources/planet" + std::to_string(i + 1) + ".jpg";
        planetTextures[i] = LoadTexture(filename.c_str());
        TraceLog(LOG_INFO, "Sun texture loaded: %d", sunTexture.id);
        TraceLog(LOG_INFO, "Moon texture loaded: %d", moonTexture.id);
    }

    Mesh sunMesh = GenMeshSphere(sun_size, 32, 32);
    sunModel = LoadModelFromMesh(sunMesh);
    sunModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = sunTexture;
    sunModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    planetModels.resize(8);
    for (size_t i = 0; i < 8; i++) {
        Mesh planetMesh = GenMeshSphere(planet_sizes[i], 32, 32);
        planetModels[i] = LoadModelFromMesh(planetMesh);
        planetModels[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = planetTextures[i];
        planetModels[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    }

    Mesh moonMesh = GenMeshSphere(moon_size, 32, 32);
    moonModel = LoadModelFromMesh(moonMesh);
    moonModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = moonTexture;
    moonModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    backgroundTexture = LoadTexture("resources/background.jpg");
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load background texture");
    }

    planet_rotation_angles.assign(8, 0.0f);
    planet_rotation_speeds = { 0.12f, 0.21f, 0.24f, 0.4f, 0.5f, 0.17f, 0.35f, 0.45f };
}

SolarSystem::~SolarSystem() {
    UnloadModel(sunModel);
    UnloadModel(moonModel);
    for (auto& model : planetModels) {
        UnloadModel(model);
    }
    UnloadTexture(sunTexture);
    UnloadTexture(moonTexture);
    for (auto& texture : planetTextures) {
        UnloadTexture(texture);
    }
    UnloadTexture(backgroundTexture);
    CloseWindow();
}

void SolarSystem::UpdateCamera() {
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();

        if (firstMouseMove) {
            previousMousePosition = mousePosition;
            firstMouseMove = false;
        }

        float deltaX = mousePosition.x - previousMousePosition.x;
        float deltaY = mousePosition.y - previousMousePosition.y;
        previousMousePosition = mousePosition;

        const float sensitivity = 0.005f;

        cameraYaw += deltaX * sensitivity;
        cameraPitch += deltaY * sensitivity;

        cameraPitch = Clamp(cameraPitch, -PI / 2 + 0.1f, PI / 2 - 0.1f);

        float distance = Vector3Distance(camera.position, center);
        camera.position.x = center.x + distance * cos(cameraPitch) * sin(cameraYaw);
        camera.position.y = center.y + distance * sin(cameraPitch);
        camera.position.z = center.z + distance * cos(cameraPitch) * cos(cameraYaw);

        camera.target = center;
    }
    else {
        firstMouseMove = true;
    }
    float moveSpeed = 0.5f;

    if (IsKeyDown(KEY_RIGHT)) {
        Vector3 right = Vector3CrossProduct(Vector3Subtract(camera.target, camera.position), camera.up);
        right = Vector3Normalize(right);
        camera.position = Vector3Add(camera.position, Vector3Scale(right, -moveSpeed));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, -moveSpeed));
    }
    if (IsKeyDown(KEY_LEFT)) {
        Vector3 right = Vector3CrossProduct(Vector3Subtract(camera.target, camera.position), camera.up);
        right = Vector3Normalize(right);
        camera.position = Vector3Add(camera.position, Vector3Scale(right, moveSpeed));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_UP)) {
        Vector3 viewDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3CrossProduct(viewDir, camera.up);
        Vector3 up = Vector3CrossProduct(right, viewDir);
        up = Vector3Normalize(up);
        camera.position = Vector3Add(camera.position, Vector3Scale(up, moveSpeed));
        camera.target = Vector3Add(camera.target, Vector3Scale(up, moveSpeed));
    }
    if (IsKeyDown(KEY_DOWN)) {
        Vector3 viewDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3CrossProduct(viewDir, camera.up);
        Vector3 up = Vector3CrossProduct(right, viewDir);
        up = Vector3Normalize(up);
        camera.position = Vector3Add(camera.position, Vector3Scale(up, -moveSpeed));
        camera.target = Vector3Add(camera.target, Vector3Scale(up, -moveSpeed));
    }

    if (IsKeyDown(KEY_MINUS)) {
        Vector3 viewDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        camera.position = Vector3Add(camera.position, Vector3Scale(viewDir, moveSpeed * 2));
    }
    if (IsKeyDown(KEY_EQUAL)) {
        Vector3 viewDir = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        camera.position = Vector3Add(camera.position, Vector3Scale(viewDir, -moveSpeed * 2));
    }
}

void SolarSystem::run() {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            fullscreen = !fullscreen;
            if (fullscreen) {
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                ToggleFullscreen();
            }
            else {
                ToggleFullscreen();
                SetWindowSize(1280, 960);
            }
        }

        UpdateCamera();

        for (size_t i = 0; i < planet_radius.size(); ++i) {
            planet_angle[i] += planet_velocities[i] * 0.02f;
        }
        for (size_t i = 0; i < planet_rotation_angles.size(); ++i) {
            planet_rotation_angles[i] += planet_rotation_speeds[i] * 0.5f;
        }

        moon_angle += moon_velocity * 0.009f;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(backgroundTexture,
            { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 }, 0.0f, WHITE);

        BeginMode3D(camera);

        DrawModel(sunModel, center, 1.0f, WHITE);

        for (size_t i = 0; i < planet_radius.size(); ++i) {
            DrawOrbitRing(planet_radius[i], GRAY);

            Vector3 planet_pos = {
                center.x + cos(planet_angle[i]) * planet_radius[i],
                center.y,
                center.z + sin(planet_angle[i]) * planet_radius[i]
            };
            DrawModelEx(planetModels[i], planet_pos, { 0, 1, 0 }, planet_rotation_angles[i] * RAD2DEG,
                { 1, 1, 1 }, WHITE);

            if ((int)i == earth_pos) {
                Vector3 moon_pos = {
                    planet_pos.x + cos(moon_angle) * moon_radius,
                    planet_pos.y,
                    planet_pos.z + sin(moon_angle) * moon_radius
                };
                DrawModel(moonModel, moon_pos, 1.0f, WHITE);
            }
        }

        EndMode3D();

        DrawText("Solar System 3D", 10, 10, 20, WHITE);
        DrawText("Arrow keys: Move camera", 10, 40, 20, WHITE);
        DrawText("+/-: Zoom in/out", 10, 70, 20, WHITE);
        DrawText("F11 - Toggle fullscreen", 10, 100, 20, WHITE);

        EndDrawing();
    }
}