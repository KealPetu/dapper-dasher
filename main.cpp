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
    int frameCols {6};
    const int TOTAL_FRAMES { 6 };
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
            if (currentFrame == TOTAL_FRAMES) currentFrame = 0;
        }
        textureRect.x = texture.width/frameCols * currentFrame;
        constexpr Vector2 hitboxOffset {45, 0};
        constexpr Vector2 hitboxSizeReduction {60, 0};
        hitboxRect = {position.x + hitboxOffset.x, position.y + hitboxOffset.y,
            textureRect.width - hitboxSizeReduction.x, textureRect.height - hitboxSizeReduction.y};
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
    const int frameCols {8};
    const int frameRows {8};
    const int TOTAL_FRAMES { 61 };     // The last 3 cells are empty / don't have any sprites
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
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
            currentTime = 0.f;
            const int row = currentFrame / frameCols;
            const int col = currentFrame % frameRows;
            textureRect.x = col * (texture.width / frameCols);
            textureRect.y = row * (texture.height / frameRows);
        }
        constexpr Rectangle padding {20, 20, 30, 25};
        constexpr Vector2 hitboxOffset {20, 20};
        constexpr Vector2 hitboxSizeReduction {30, 25};
        hitboxRect = {position.x + hitboxOffset.x, position.y + hitboxOffset.y,
            textureRect.width - hitboxSizeReduction.x, textureRect.height - hitboxSizeReduction.y};
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
    player.textureRect.width = player.texture.width / player.frameCols;
    player.textureRect.height = player.texture.height;
    player.position.x = WINDOW_WIDTH/2.f - player.textureRect.width / 2.f;
    player.jumpVelocity = GRAVITY_ACC * 2/3;
    player.canJump = false;

    Nebula nebula;
    nebula.texture = LoadTexture("./resources/sprites/nebula/nebula-spritesheet.png");
    nebula.textureRect.width = nebula.texture.width / nebula.frameCols;
    nebula.textureRect.height = nebula.texture.height / nebula.frameRows;
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