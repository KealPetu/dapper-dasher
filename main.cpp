#include "raylib.h"
#include "raymath.h"

constexpr int WINDOW_WIDTH  {800};
constexpr int WINDOW_HEIGHT {450};
constexpr float GRAVITY_ACC {981.f};

struct Player
{
    Vector2 position {};
    Vector2 velocity {};
    Texture2D texture {};
    Rectangle textureRect {};
    Rectangle hitboxRect {};
    float jumpVelocity {};
    const int FRAMES_PER_SEC { 8 };
    const float TIME_PER_FRAME { 1.f / FRAMES_PER_SEC };
    float currentTime {};
    int currentFrame {};
    bool canJump {};

    void Update(const float deltaTime, const float windowHeight) {
        velocity.y += GRAVITY_ACC * deltaTime;
        position.y += velocity.y * deltaTime;
        position.y = Clamp(position.y, 0.f,windowHeight - textureRect.height);
        if (position.y == windowHeight - textureRect.height) {
            canJump = true;
            velocity.y = 0.f;
        }
        if (IsKeyPressed(KEY_SPACE) && canJump) {
            velocity.y = -jumpVelocity;
            canJump = false;
        }
        currentTime += deltaTime;
        if (currentTime >= TIME_PER_FRAME && canJump) {
            currentFrame++;
            currentTime = 0.f;
            if (currentFrame > 5) currentFrame = 0;
        }
        textureRect.x = texture.width/6 * currentFrame;
        hitboxRect = {position.x + 45, position.y, textureRect.width - 60, textureRect.height};
    }

    void Draw() {
        DrawTextureRec(texture, textureRect, position, WHITE);
        //DEBUG-
        DrawRectangleLines(hitboxRect.x, hitboxRect.y, hitboxRect.width, hitboxRect.height, RED);
        //------
    }
};

struct Nebula
{
    Vector2 position {};
    Vector2 velocity {};
    Texture2D texture {};
    Rectangle textureRect {};
    Rectangle hitboxRect {};
    const int FRAMES_PER_SEC { 8 };
    const float TIME_PER_FRAME { 1.f / FRAMES_PER_SEC };
    float currentTime {};
    int currentFrame {};

    void Update(const float deltaTime, const float windowWidth) {
        position.x += velocity.x * deltaTime;
        if (position.x < -textureRect.width) {
            position.x = windowWidth;
        }
        currentTime += deltaTime;
        if (currentTime >= TIME_PER_FRAME) {
            currentFrame = (currentFrame + 1) % 61;
            currentTime = 0.f;
            int row = currentFrame / 8;
            int col = currentFrame % 8;
            textureRect.x = col * (texture.width / 8);
            textureRect.y = row * (texture.height / 8);
        }
        hitboxRect = {position.x + 20, position.y + 20, textureRect.width - 30, textureRect.height - 25};
    }
    void Draw() {
        DrawTextureRec(texture, textureRect, position, WHITE);
        //Debug-
        DrawRectangleLines(hitboxRect.x, hitboxRect.y, hitboxRect.width, hitboxRect.height, RED);
        //------
    }
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
        player.Update(deltaTime, WINDOW_HEIGHT);
        nebula.Update(deltaTime, WINDOW_WIDTH);
        const bool isColliding = CheckCollisionRecs(player.hitboxRect, nebula.hitboxRect);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            player.Draw();
            nebula.Draw();
            DrawText(TextFormat("player.velocity.y: %.2f", player.velocity.y), 10, 10, 10, RED);
            if (isColliding) {
                 DrawText("Collision!", 10, 30, 20, RED);
            }
        EndDrawing();
    }

    UnloadTexture(player.texture);
    UnloadTexture(nebula.texture);
    CloseWindow();
    return 0;
}