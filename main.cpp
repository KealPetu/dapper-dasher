#include "raylib.h"
#include "raymath.h"

constexpr int WINDOW_WIDTH  {800};
constexpr int WINDOW_HEIGHT {450};
constexpr float GRAVITY_ACC {981.f};

struct Player {
    Vector2 velocity    {0.f,0.f };
    Rectangle rectangle {0.f, 0.f, 0.f, 0.f };
    Texture2D texture   {0, 0, 0, 0, 0 };
    float jumpVelocity  { 0.f };
    bool canJump        {false};
};

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "dapper dasher game");

    float deltaTime {0.f};
    Player player {
        Vector2 {0.f,0.f},
        Rectangle {0, 0, 0, 0},
        Texture2D { LoadTexture("resources/sprites/scarfy/scarfy.png") },
        GRAVITY_ACC * 2 / 3,
        false
    };

    player.rectangle.width = player.texture.width/6;
    player.rectangle.height = player.texture.height;
    player.rectangle.x = WINDOW_WIDTH/2.f - player.rectangle.width / 2.f;

    while (!WindowShouldClose()) {

        deltaTime = GetFrameTime();
        player.velocity.y += GRAVITY_ACC * deltaTime;
        player.rectangle.y += player.velocity.y * deltaTime;
        player.rectangle.y = Clamp(player.rectangle.y, 0.f, WINDOW_HEIGHT - player.rectangle.height);

        if (player.rectangle.y == WINDOW_HEIGHT - player.rectangle.height) {
            player.canJump = true;
            player.velocity.y = 0.f;
        }

        if (IsKeyPressed(KEY_SPACE) && player.canJump) {
            player.velocity.y = -player.jumpVelocity;
            player.canJump = false;
        }

        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangle(static_cast<int>(player.rectangle.x), static_cast<int>(player.rectangle.y),
                          static_cast<int>(player.rectangle.width), static_cast<int>(player.rectangle.height), BLUE);
            DrawText(TextFormat("player.velocity.y: %.2f", player.velocity.y), 10, 10, 10, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}