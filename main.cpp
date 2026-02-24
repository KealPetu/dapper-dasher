#include <iostream>
#include <raylib.h>

#include "build/_deps/raylib-src/src/raymath.h"

constexpr int WINDOW_WIDTH  {800};
constexpr int WINDOW_HEIGHT {450};
constexpr float GRAVITY_ACC {981.f};

struct Player {
    Vector2 position    {0.f,0.f};
    Vector2 velocity    {0.f,0.f};
    Vector2 size        {0.f,0.f};
    float jumpVelocity  {0.f};
    bool canJump        {false};
};

int main() {

    float deltaTime {0.f};
    Player player {
        Vector2{0.f, 0.f},
        Vector2{0.f,0.f},
        Vector2{50.f,80.f},
        654.f,
        false,
    };

    player.position.x = WINDOW_WIDTH/2.f - player.size.x / 2.f;
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "dapper dasher game");

    while (!WindowShouldClose()) {

        deltaTime = GetFrameTime();
        player.velocity.y += GRAVITY_ACC * deltaTime;
        player.position.y += player.velocity.y * deltaTime;
        player.position.y = Clamp(player.position.y, 0.f, WINDOW_HEIGHT - player.size.y);

        if (player.position.y == WINDOW_HEIGHT - player.size.y) {
            player.canJump = true;
            player.velocity.y = 0.f;
        }

        if (IsKeyPressed(KEY_SPACE) && player.canJump) {
            player.velocity.y = -player.jumpVelocity;
            player.canJump = false;
        }

        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangle(static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                          static_cast<int>(player.size.x), static_cast<int>(player.size.y), BLUE);
            DrawText(TextFormat("player.velocity.y: %.2f", player.velocity.y), 10, 10, 10, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}