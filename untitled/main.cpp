#include <iostream>
#include <format>
#include <memory>

#include "raylib.h"
#include "gamestate.h"

using namespace std;

int count = 1, frame = 0, inv_frame = -1, direction = 0, animation, atk_frame, def_frame, x = 0, y = 0, dmg = 0;
Texture2D CharacterSprite;
Texture2D AttackSprite;
Texture2D CrabSprite;

int main() {
    map map;
    std::vector<std::unique_ptr<Enemy<int>>> enemies;
    enemies.push_back(std::make_unique<Necromancer>(EnemyFactory::CreateNecromancer(5)));
    enemies.push_back(std::make_unique<Crab>(EnemyFactory::CreateCrab()));
    enemies.push_back(std::make_unique<Scorpion>(EnemyFactory::CreateScorpion()));
    enemies.push_back(std::make_unique<Teleporter>(EnemyFactory::CreateTeleporter()));
    enemies.push_back(std::make_unique<Boss>(EnemyFactory::CreateBoss(5)));

    player player;

    InitWindow(1200, 900, "GAME");
    SetTargetFPS(60);
    InitAudioDevice();
    Sound sound = LoadSound("resources/beach.wav");

    CharacterSprite = LoadTexture("resources/PlayerSprite.png");
    CrabSprite = LoadTexture("resources/crab.png");

    SetMasterVolume(100);
    PlaySound(sound);

    while (!WindowShouldClose()) {
        if (!IsSoundPlaying(sound)) {
            sound = LoadSound("resources/beach.wav");
            PlaySound(sound);
        }

        BeginDrawing();
        ClearBackground(PINK);

        switch (map.number) {
            case 15: map.down(); break;
            case 24: map.dr_l(); break;
            case 25: map.up_t(); break;
            case 26: map.dl_l(); break;
            case 32: map.down(); break;
            case 34: map.right_t(); break;
            case 35: map.hor_line(); break;
            case 36: map.ul_l(); break;
            case 42: map.right_t(); break;
            case 43: map.hor_line(); break;
            case 44: map.up_t(); break;
            case 45: map.dl_l(); break;
            case 47: map.down(); break;
            case 48: map.dr_l(); break;
            case 49: map.left(); break;
            case 51: map.right(); break;
            case 52: map.ul_l(); break;
            case 53: map.down(); break;
            case 54: map.dr_l(); break;
            case 55: map.cross(); break;
            case 56: map.down_t(); break;
            case 57: map.up_t(); break;
            case 58: map.left_t(); break;
            case 63: map.right_t(); break;
            case 64: map.cross(); break;
            case 65: map.left_t(); break;
            case 66: map.ur_l(); break;
            case 67: map.dl_l(); break;
            case 68: map.ver_line(); break;
            case 69: map.down(); break;
            case 71: map.right(); break;
            case 72: map.hor_line(); break;
            case 73: map.left_t(); break;
            case 74: map.right_t(); break;
            case 75: map.up_t(); break;
            case 76: map.down_t(); break;
            case 77: map.up_t(); break;
            case 78: map.up_t(); break;
            case 79: map.left_t(); break;
            case 83: map.ur_l(); break;
            case 84: map.left_t(); break;
            case 86: map.ver_line(); break;
            case 89: map.ver_line(); break;
            case 94: map.up(); break;
            case 95: map.right(); break;
            case 96: map.ul_l(); break;
            case 97: map.right(); break;
            case 98: map.hor_line(); break;
            case 99: map.ul_l(); break;
        }

        for (auto& enemy : enemies) {
            animation = (animation + 1) % 15;
            enemy->animation = (enemy->animation + 1) % 10;

            if (GetRandomValue(1, 10000) % GetRandomValue(20, 35) == 0) {
                enemy->value = GetRandomValue(1, 5);
            }

            switch (enemy->value) {
                case 1: if (enemy->x < 1000) enemy->walk_right(); break;
                case 2: if (enemy->x > 100) enemy->walk_left(); break;
                case 3: if (enemy->y > 100) enemy->walk_up(); break;
                case 4: if (enemy->y < 700) enemy->walk_down(); break;
            }

            enemy->draw();

            Rectangle crab = { enemy->frame * 100, 0, 100, 100 };
            DrawTextureRec(CrabSprite, crab, { enemy->x, enemy->y }, WHITE);
        }

        auto it = enemies.begin();
        while (it != enemies.end()) {
            player.check_collision((*it)->x, (*it)->y, (*it)->width, (*it)->height, 2, inv_frame);
            if (atk_frame || def_frame) {
                (*it)->check_collision(player.x, player.y, dmg);
            }
            if ((*it)->isDead) {
                Enemy<int>::observer->removeEnemy(it->get());
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        Rectangle Sprite1 = { frame * 100, direction * 150, 100, 150 };
        Color tint = { 255, 255, 255, static_cast<unsigned char>(255 * player.opacity) };

        for (auto& enemy : enemies) {
            enemy->update();
        }

        if (atk_frame == 0 && def_frame == 0) {
            DrawTextureRec(CharacterSprite, Sprite1, { player.x - 50, player.y - 100 }, tint);
        } else if (def_frame == 0) {
            Rectangle Sprite2 = { frame * 100, direction * 150, 150, 150 };
            dmg = player.damage;
            if (++atk_frame == 30) {
                atk_frame = 0;
                frame = 0;
            }
            if (direction == 1 || direction == 3) {
                x = player.x - 50;
                y = player.y - 100;
            } else if (direction == 0) {
                x = player.x - 50;
                y = player.y - 50;
            } else if (direction == 2) {
                x = player.x - 100;
                y = player.y - 100;
            }
            DrawTextureRec(CharacterSprite, Sprite2, { x, y }, tint);
        } else if (atk_frame == 0) {
            dmg = 0;
            if (++def_frame == 60) {
                def_frame = 0;
                frame = 0;
                inv_frame = -1;
            } else {
                inv_frame = direction;
            }
            DrawTextureRec(CharacterSprite, Sprite1, { player.x - 50, player.y - 100 }, tint);
        }

        EndDrawing();

        if (IsKeyDown(KEY_UP) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 0, 0, map.u, 100 }) &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 675, 0, 525, 100 })) {
            player.y -= 5;
            direction = 1;
            if (animation % 15 == 0) frame = (frame + 1) % 4;
        }
        if (IsKeyDown(KEY_DOWN) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 0, 800, map.d, 100 }) &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 675, 800, 525, 100 })) {
            player.y += 5;
            direction = 0;
            if (animation % 15 == 0) frame = (frame + 1) % 4;
        }
        if (IsKeyDown(KEY_LEFT) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 0, 0, 100, map.l }) &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 0, 550, 100, 350 })) {
            player.x -= 5;
            direction = 2;
            if (animation % 15 == 0) frame = (frame + 1) % 4;
        }
        if (IsKeyDown(KEY_RIGHT) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 1100, 0, 100, map.r }) &&
            !CheckCollisionCircleRec({ player.x, player.y }, player.radius, { 1100, 550, 100, 350 })) {
            player.x += 5;
            direction = 3;
            if (animation % 15 == 0) frame = (frame + 1) % 4;
        }

        if (IsKeyPressed(KEY_Z) && atk_frame == 0 && def_frame == 0) {
            atk_frame = 1;
            frame = 5;
        }

        if (IsKeyDown(KEY_X) && atk_frame == 0 && def_frame == 0) {
            def_frame = 1;
            frame = 4;
        }

        if (player.x <= 140) {
            player.x = 1059;
            player.y = 450;
            map.number--;
        }
        if (player.x >= 1060) {
            player.x = 151;
            player.y = 450;
            map.number++;
        }
        if (player.y <= 140) {
            player.x = 600;
            player.y = 749;
            map.number -= 10;
        }
        if (player.y >= 760) {
            player.x = 600;
            player.y = 151;
            map.number += 10;
        }
    }

    UnloadSound(sound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
