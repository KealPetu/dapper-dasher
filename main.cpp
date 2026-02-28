#include "raylib.h"
#include "raymath.h"

constexpr int WINDOW_WIDTH  {800};
constexpr int WINDOW_HEIGHT {450};
constexpr float GRAVITY_ACC {981.f};

struct Player
{
    Vector2 position    {0.f, 0.f };
    Vector2 velocity    {0.f,0.f };
    Texture2D texture   {0, 0, 0, 0, 0 };
    Rectangle textureRect {0.f, 0.f, 0.f, 0.f };
    float jumpVelocity  { 0.f };
    const int FRAME_PER_SEC { 8 };
    const float TIME_PER_FRAME { 1.f / FRAME_PER_SEC };
    float currentTime { 0.f };
    int currentFrame { 0 };
    bool canJump        {false};
};

struct Nebula
{
    Vector2 position    {0.f, 0.f };
    Vector2 velocity    {0.f,0.f };
    Texture2D texture   {0, 0, 0, 0, 0 };
    Rectangle textureRect {0.f, 0.f, 0.f, 0.f };
    const int FRAME_PER_SEC { 8 };
    const float TIME_PER_FRAME { 1.f / FRAME_PER_SEC };
    float currentTime { 0.f };
    int currentFrame { 0 };
};

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "dapper dasher game");

    float deltaTime {0.f};

    Player player;
    player.texture = LoadTexture("./resources/sprites/scarfy/scarfy.png");
    player.textureRect.width = player.texture.width / 6;
    player.textureRect.height = player.texture.height;
    player.position.x = WINDOW_WIDTH/2.f - player.textureRect.width / 2.f;
    player.jumpVelocity = GRAVITY_ACC * 2/3;
    player.canJump = false;

    Nebula nebula;
    nebula.texture = LoadTexture("./resources/sprites/nebula/nebula-spritesheet.png");
    nebula.textureRect.width = nebula.texture.width / 8;
    nebula.textureRect.height = nebula.texture.height / 8;
    nebula.position.x = WINDOW_WIDTH;
    nebula.position.y = WINDOW_HEIGHT - nebula.textureRect.height;
    nebula.velocity.x = -163.5f;

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

        nebula.position.x += nebula.velocity.x * deltaTime;

        player.currentTime += deltaTime;

        if (player.currentTime >= player.TIME_PER_FRAME && player.canJump) {
            player.currentFrame++;
            player.currentTime = 0.f;
        }

        if (player.currentFrame > 5) {
            player.currentFrame = 0;
        }

        player.textureRect.x = player.texture.width/6 * player.currentFrame;

        nebula.currentTime += deltaTime;

        if (nebula.currentTime >= nebula.TIME_PER_FRAME)
        {
            nebula.currentFrame++;
            nebula.currentTime = 0.f;
        }

        if (nebula.currentFrame != 0 && nebula.currentFrame % 8 == 0) {
            nebula.textureRect.y += 100;
            nebula.currentFrame = 0;
        }

        if (nebula.textureRect.x == 400 && nebula.textureRect.y == 700)
        {
            nebula.textureRect.x = 0;
            nebula.textureRect.y = 0;
            nebula.currentFrame = 0;
        }

        nebula.textureRect.x = nebula.texture.width/8 * nebula.currentFrame;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTextureRec(player.texture, player.textureRect, player.position, WHITE);
            DrawTextureRec(nebula.texture, nebula.textureRect, nebula.position, WHITE);
            DrawText(TextFormat("player.velocity.y: %.2f", player.velocity.y), 10, 10, 10, RED);
        EndDrawing();
    }

    UnloadTexture(player.texture);
    UnloadTexture(nebula.texture);
    CloseWindow();
    return 0;
}