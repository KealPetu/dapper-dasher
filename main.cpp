#include "raylib.h"
#include "raymath.h"

constexpr int WINDOW_WIDTH  {800};
constexpr int WINDOW_HEIGHT {450};
constexpr float GRAVITY_ACC {981.f};

struct Player {
    Vector2 position    {0.f, 0.f };
    Vector2 velocity    {0.f,0.f };
    Texture2D texture   {0, 0, 0, 0, 0 };
    Rectangle textureRect {0.f, 0.f, 0.f, 0.f };
    float jumpVelocity  { 0.f };
    bool canJump        {false};
};

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "dapper dasher game");

    float deltaTime {0.f};
    Player player {
        Vector2 {0.f,0.f},
        Vector2 {0.f,0.f},
        Texture2D { LoadTexture("./resources/sprites/scarfy/scarfy.png") },
        Rectangle {0, 0, 0, 0},
        GRAVITY_ACC * 2 / 3,
        false
    };

    player.textureRect.width = player.texture.width / 6;
    player.textureRect.height = player.texture.height;
    player.position.x = WINDOW_WIDTH/2.f - player.textureRect.width / 2.f;

    constexpr int FRAME_PER_SEC { 8 };
    constexpr float TIME_PER_FRAME { 1.f / FRAME_PER_SEC };
    float currentTime { 0.f };
    int currentFrame { 0 };
    while (!WindowShouldClose()) {
        deltaTime = GetFrameTime();
        player.velocity.y += GRAVITY_ACC * deltaTime;
        player.position.y += player.velocity.y * deltaTime;
        player.position.y = Clamp(player.position.y, 0.f, WINDOW_HEIGHT - player.textureRect.height);

        if (player.position.y == WINDOW_HEIGHT - player.textureRect.height) {
            player.canJump = true;
            player.velocity.y = 0.f;
        }

        if (IsKeyPressed(KEY_SPACE) && player.canJump) {
            player.velocity.y = -player.jumpVelocity;
            player.canJump = false;
        }

        currentTime += deltaTime;

        if (currentTime >= TIME_PER_FRAME) {
            currentFrame++;
            currentTime = 0.f;
        }

        if (currentFrame > 5) {
            currentFrame = 0;
        }

        player.textureRect.x = player.texture.width/6 * currentFrame;

        BeginDrawing();
            ClearBackground(WHITE);
            DrawTextureRec(player.texture, player.textureRect, player.position, WHITE);
            DrawText(TextFormat("player.velocity.y: %.2f", player.velocity.y), 10, 10, 10, RED);
        EndDrawing();
    }

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}