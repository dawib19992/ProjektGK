#include <raylib.h>
#include <string>
#include <raymath.h>
#include <vector>

class SolarSystem {
private:
    Camera3D camera;
    Vector3 center;
    std::vector<float> planet_radius;
    std::vector<float> planet_velocities;
    std::vector<float> planet_sizes;
    std::vector<Color> colors;
    std::vector<float> planet_angle;
    float moon_angle, moon_velocity = 2.7f, moon_radius = 3.5f, moon_size = 0.8f, sun_size = 6.2f;
    int earth_pos;
    bool fullscreen;

    Vector2 previousMousePosition;
    bool firstMouseMove = true;
    float cameraYaw = 0.0f;
    float cameraPitch = 0.0f;

    std::vector<float> planet_rotation_angles;
    std::vector<float> planet_rotation_speeds;

    Texture2D backgroundTexture;

    Model sunModel;
    Model moonModel;
    std::vector<Model> planetModels;
    Texture2D sunTexture;
    Texture2D moonTexture;
    std::vector<Texture2D> planetTextures;

public:
    SolarSystem();
    ~SolarSystem();
    void DrawOrbitRing(float radius, Color color);
    void UpdateCamera();
    void run();
};