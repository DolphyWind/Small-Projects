#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct {
    int x;
    int y;
    int width;
    int height;
    Color color;
} Box;

bool aabb_collision(Box* a, Box* b) {
    return a->x < b->x + b->width && a->x + a->width > b->x &&
           a->y < b->y + b->height && a->y + a->height > b->y;
}


int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Yaoi");

    Box box1 = {
        .x = 20,
        .y = 300,
        .width = 128,
        .height = 128,
        .color = BLUE,
    };

    Box box2 = {
        .x = 1100,
        .y = 300,
        .width = 128,
        .height = 128,
        .color = RED
    };

    int spamton_frames = 0;
    int tenna_frames = 0;
    Image spamton = LoadImageAnim("spamton.png", &spamton_frames);
    ImageResize(&spamton, box1.width, box1.height);
    Image tenna = LoadImageAnim("tenna.png", &tenna_frames);
    ImageResize(&tenna, box2.width, box2.height);
    Texture2D spamton_texture = LoadTextureFromImage(spamton);
    Texture2D tenna_texture = LoadTextureFromImage(tenna);

    SetTargetFPS(60);
    const int SPD = 10;

    while (!WindowShouldClose()) {
        int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

        if (IsKeyDown(KEY_LEFT)) {
            dx1 = -SPD;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            dx1 = SPD;
        }
        if (IsKeyDown(KEY_UP)) {
            dy1 = -SPD;
        }
        if (IsKeyDown(KEY_DOWN)) {
            dy1 = SPD;
        }
        if (IsKeyDown(KEY_A)) {
            dx2 = -SPD;
        }
        if (IsKeyDown(KEY_D)) {
            dx2 = SPD;
        }
        if (IsKeyDown(KEY_W)) {
            dy2 = -SPD;
        }
        if (IsKeyDown(KEY_S)) {
            dy2 = SPD;
        }

        box1.x += dx1;
        box1.y += dy1;
        box2.x += dx2;
        box2.y += dy2;

        if(aabb_collision(&box1, &box2)) {
            int overlap_x = MIN(box1.x + box1.width, box2.x + box2.width) - MAX(box1.x, box2.x);
            int overlap_y = MIN(box1.y + box1.height, box2.y + box2.height) - MAX(box1.y, box2.y);
            
            Vector2 mtv = {.x = 0, .y = 0};
            if (overlap_x < overlap_y) {
                mtv.x = overlap_x * (box1.x < box2.x ? -1 : 1);
            }
            else {
                mtv.y = overlap_y * (box1.y < box2.y ? -1 : 1);
            }
            if (dx1 != 0 || dy1 != 0) {
                box1.x += mtv.x;
                box1.y += mtv.y;
            }
            if(dx2 != 0 || dy2 != 0) {
                box2.x -= mtv.x;
                box2.y -= mtv.y;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexture(spamton_texture, box1.x, box1.y, WHITE);
            DrawTexture(tenna_texture, box2.x, box2.y, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
