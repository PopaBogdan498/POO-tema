#include <iostream>
#include <format>
#include <memory>
#include <vector>
#include <algorithm>

#include "raylib.h"
#include "map.h"
#include "enemies.h"
#include "player.h"

using namespace std;


int count_ = 1, inv_frame = -1, direction = 0, atk_frame, def_frame;
int x_draw_offset = 0, y_draw_offset = 0;
int playerAttackDamage = 0;

Texture2D CharacterSprite;
Texture2D CrabSprite;

int previous_map_number = -1;
int enemy_spawn_timer = 0;
const int ENEMY_SPAWN_DELAY_FRAMES = 60;

void spawnNewEnemies(std::vector<std::unique_ptr<Enemy<int>>>& enemies) {

    enemies.clear();


    int num_enemies_to_spawn = GetRandomValue(1, 5);

    for (int i = 0; i < num_enemies_to_spawn; ++i) {
        int enemy_type = GetRandomValue(0, 4);

        switch (enemy_type) {
            case 0: enemies.push_back(EnemyFactory::CreateCrab()); break;
            case 1: enemies.push_back(EnemyFactory::CreateScorpion()); break;
            case 2: enemies.push_back(EnemyFactory::CreateNecromancer(GetRandomValue(1, 3))); break;
            case 3: enemies.push_back(EnemyFactory::CreateTeleporter()); break;
            case 4: enemies.push_back(EnemyFactory::CreateBoss(2000, GetRandomValue(1, 2))); break;
        }
    }



}


int main() {

    map map;
    std::vector<std::unique_ptr<Enemy<int>>> enemies;

    player player;


    InitWindow(1200, 900, "GAME");
    SetTargetFPS(60);
    InitAudioDevice();
    Sound sound = LoadSound("resources/beach.wav");


    CharacterSprite = LoadTexture("resources/PlayerSprite.png");
    CrabSprite = LoadTexture("resources/crab.png");

    SetMasterVolume(100);
    PlaySound(sound);


    previous_map_number = map.number;
    spawnNewEnemies(enemies);



    while (!WindowShouldClose()) {

        if (!IsSoundPlaying(sound)) {
            sound = LoadSound("resources/beach.wav");
            PlaySound(sound);
        }


        if (map.number != previous_map_number) {

            enemies.clear();
            Enemy<int>::getObserver()->clearDeadEnemies();
            enemy_spawn_timer = ENEMY_SPAWN_DELAY_FRAMES;
            previous_map_number = map.number;
        }


        if (enemy_spawn_timer > 0) {
            enemy_spawn_timer--;
            if (enemy_spawn_timer == 0) {

                spawnNewEnemies(enemies);
            }
        }


        BeginDrawing();
        ClearBackground(PINK);


        switch (map.number) {
            case 15: map.down(); break; case 24: map.dr_l(); break;
            case 25: map.up_t(); break; case 26: map.dl_l(); break;
            case 32: map.down(); break; case 34: map.right_t(); break;
            case 35: map.hor_line(); break; case 36: map.ul_l(); break;
            case 42: map.right_t(); break; case 43: map.hor_line(); break;
            case 44: map.up_t(); break; case 45: map.dl_l(); break;
            case 47: map.down(); break; case 48: map.dr_l(); break;
            case 49: map.left(); break; case 51: map.right(); break;
            case 52: map.ul_l(); break; case 53: map.down(); break;
            case 54: map.dr_l(); break; case 55: map.cross(); break;
            case 56: map.down_t(); break; case 57: map.up_t(); break;
            case 58: map.left_t(); break; case 63: map.right_t(); break;
            case 64: map.cross(); break; case 65: map.left_t(); break;
            case 66: map.ur_l(); break; case 67: map.dl_l(); break;
            case 68: map.ver_line(); break; case 69: map.down(); break;
            case 71: map.right(); break; case 72: map.hor_line(); break;
            case 73: map.left_t(); break; case 74: map.right_t(); break;
            case 75: map.up_t(); break; case 76: map.down_t(); break;
            case 77: map.up_t(); break; case 78: map.up_t(); break;
            case 79: map.left_t(); break; case 83: map.ur_l(); break;
            case 84: map.left_t(); break; case 86: map.ver_line(); break;
            case 89: map.ver_line(); break; case 94: map.up(); break;
            case 95: map.right(); break; case 96: map.ul_l(); break;
            case 97: map.right(); break; case 98: map.hor_line(); break;
            case 99: map.ul_l(); break;
        }


        player.update();

        DrawText(TextFormat("HP: %d", player.getHp()), 10, 10, 30, WHITE);
        if (player.getHp() <= 0) {
            DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 60) / 2, GetScreenHeight() / 2 - 30, 60, RED);
        }


        for (auto& enemy : enemies) {
            if (enemy->isDead()) continue;


            if (GetRandomValue(1, 10000) % GetRandomValue(20, 35) == 0) {
                enemy->value() = GetRandomValue(1, 4);
            }


            switch (enemy->value()) {
                case 1: enemy->walk_right(); break;
                case 2: enemy->walk_left(); break;
                case 3: enemy->walk_up(); break;
                case 4: enemy->walk_down(); break;
            }


            enemy->x() = std::max(100, std::min(enemy->x(), 1200 - enemy->width() - 100));
            enemy->y() = std::max(100, std::min(enemy->y(), 900 - enemy->height() - 100));

            enemy->update();
            enemy->draw();


            Rectangle enemy_sprite_frame = { (float)enemy->frame() * 100, 0, 100, 100 };
            DrawTextureRec(CrabSprite, enemy_sprite_frame, { (float)enemy->x(), (float)enemy->y() }, WHITE);


            DrawText(enemy->getTypeString().c_str(), enemy->x(), enemy->y() - 20, 15, RED);
        }


        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if ((*it)->isDead()) {
                if (Enemy<int>::getObserver()) {

                    auto enemy_int_ptr = dynamic_cast<Enemy<int>*>(it->get());
                    if (enemy_int_ptr) {
                        Enemy<int>::getObserver()->registerDeath(std::move(*it));
                    }
                }
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }


        for (auto& enemy_ptr : enemies) {
            Necromancer* necromancer_ptr = dynamic_cast<Necromancer*>(enemy_ptr.get());
            if (necromancer_ptr && necromancer_ptr->isReadyToRevive()) {
                if (necromancer_ptr->maxRevives > 0) {
                    std::unique_ptr<Enemy<int>> revived_enemy = Enemy<int>::getObserver()->popDeadEnemy();
                    if (revived_enemy) {
                        revived_enemy->isDead() = false;
                        revived_enemy->getHp() = 50;

                        revived_enemy->x() = GetRandomValue(100, 1200 - revived_enemy->width() - 100);
                        revived_enemy->y() = GetRandomValue(100, 900 - revived_enemy->height() - 100);
                        enemies.push_back(std::move(revived_enemy));
                        necromancer_ptr->resetReviveTimer();
                        necromancer_ptr->maxRevives--;
                        break;
                    }
                }
            }
        }




        Rectangle Sprite1 = { (float)player.getFrame() * 100, (float)direction * 150, 100, 150 };
        Color tint = { 255, 255, 255, static_cast<unsigned char>(255 * player.getOpacity()) };

        if (atk_frame == 0 && def_frame == 0) {
            DrawTextureRec(CharacterSprite, Sprite1, { (float)player.x - 50, (float)player.y - 100 }, tint);
            inv_frame = -1;
        } else if (def_frame == 0) {
            Rectangle Sprite2 = { (float)player.getFrame() * 100, (float)direction * 150, 150, 150 };
            playerAttackDamage = player.getAttackDamage();
            if (++atk_frame == 30) {
                atk_frame = 0;
                player.setFrame(0);
            }

            if (direction == 1 || direction == 3) {
                x_draw_offset = player.x - 50;
                y_draw_offset = player.y - 100;
            } else if (direction == 0) {
                x_draw_offset = player.x - 50;
                y_draw_offset = player.y - 50;
            } else if (direction == 2) {
                x_draw_offset = player.x - 100;
                y_draw_offset = player.y - 100;
            }
            {

            DrawTextureRec(CharacterSprite, Sprite2, { (float)x_draw_offset, (float)y_draw_offset }, tint);
            }


            for (auto& enemy : enemies) {
                if (!enemy->isDead()) {

                    enemy->check_collision(player.x, player.y, playerAttackDamage);
                }
            }
            inv_frame = -1;
        } else if (atk_frame == 0) {
            playerAttackDamage = 0;
            if (++def_frame == 60) {
                def_frame = 0;
                player.setFrame(0);
                inv_frame = -1;
            } else {
                inv_frame = direction;
            }
            DrawTextureRec(CharacterSprite, Sprite1, { (float)player.x - 50, (float)player.y - 100 }, tint);
        }


        for (auto& enemy : enemies) {
            if (!enemy->isDead()) {

                player.check_collision(*enemy, inv_frame);
            }
        }

        EndDrawing();


        if (IsKeyDown(KEY_UP) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 0, 0, (float)map.u_(), 100 }) &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 675, 0, 525, 100 })) {
            player.y -= 5;
            direction = 1;
            if (count_ % 60 == 0) player.setFrame((player.getFrame() + 1) % 4);
        }
        if (IsKeyDown(KEY_DOWN) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 0, 800, (float)map.d_(), 100 }) &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 675, 800, 525, 100 })) {
            player.y += 5;
            direction = 0;
            if (count_ % 60 == 0) player.setFrame((player.getFrame() + 1) % 4);
        }
        if (IsKeyDown(KEY_LEFT) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 0, 0, 100, (float)map.l_() }) &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 0, 550, 100, 350 })) {
            player.x -= 5;
            direction = 2;
            if (count_ % 60 == 0) player.setFrame((player.getFrame() + 1) % 4);
        }
        if (IsKeyDown(KEY_RIGHT) && atk_frame == 0 && def_frame == 0 &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 1100, 0, 100, (float)map.r_() }) &&
            !CheckCollisionCircleRec({ (float)player.x, (float)player.y }, player.getRadius(), { 1100, 550, 100, 350 })) {
            player.x += 5;
            direction = 3;
            if (count_ % 60 == 0) player.setFrame((player.getFrame() + 1) % 4);
        }
        count_ = (count_ + 1) % 60;


        if (IsKeyPressed(KEY_Z) && atk_frame == 0 && def_frame == 0) {
            atk_frame = 1;
            player.setFrame(5);
        }


        if (IsKeyDown(KEY_X) && atk_frame == 0 && def_frame == 0) {
            def_frame = 1;
            player.setFrame(4);
        } else if (IsKeyUp(KEY_X) && def_frame > 0) {

            def_frame = 0;
            player.setFrame(0);
            inv_frame = -1;
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
    UnloadTexture(CharacterSprite);
    UnloadTexture(CrabSprite);
    CloseWindow();
    return 0;
}