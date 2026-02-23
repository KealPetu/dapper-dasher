#include <raylib.h>

constexpr int WINDOW_WIDTH  { 1280 };
constexpr int WINDOW_HEIGHT { 720 };

constexpr float GRAVITY_ACC { 98.1 };

struct Player {
    Vector2 position {0.f,0.f};
    float velocity { 0.f };
};

int main() {
    Player player {Vector2{WINDOW_WIDTH/2, WINDOW_HEIGHT - 80}, 0.f};
    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "dapper dasher game");

    while (!WindowShouldClose()) {
        const float deltaTime {GetFrameTime()};
        player.position.y -= GRAVITY_ACC * deltaTime;
        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangle(player.position.x, player.position.y, 50, 80, BLUE);
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}