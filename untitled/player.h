#pragma once
#include "raylib.h"
#include <iostream>
#include <cmath>

template <typename T> class Enemy;

class player {
private:
    int hp_ = 1000;
    int attackDamage_ = 100;




    int getOppositeDirection(int enemy_x, int enemy_y) {
        float deltaX = (float)enemy_x - x;
        float deltaY = (float)enemy_y - y;

        if (std::abs(deltaX) > std::abs(deltaY)) {
            if (deltaX < 0) {
                return 2;
            } else {
                return 3;
            }
        } else {
            if (deltaY < 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    const int radius_ = 50;
    float opacity_ = 1.0f;
    int invincibilityTimer_ = 0;
    const int INVINCIBILITY_DURATION = 60;
    int frame_ = 0;
public:
    int x = 600, y = 450;
    int getHp() const { return hp_; }
    void setHp(int hp) { hp_ = hp; }
    int getAttackDamage() const { return attackDamage_; }
    void setAttackDamage(int damage) { attackDamage_ = damage; }
    int getX() const { return x; }
    int getY() const { return y; }

    int getRadius() const { return radius_; }
    float& getOpacity() { return opacity_; }

    int getFrame() const { return frame_; }
    void setFrame(int frame) { frame_ = frame; }


    int getInvincibilityTimer() const { return invincibilityTimer_; }

    void update() {
        if (invincibilityTimer_ > 0) {
            invincibilityTimer_--;
            if (invincibilityTimer_ % 10 < 5) {
                opacity_ = 0.5f;
            } else {
                opacity_ = 1.0f;
            }
            if (invincibilityTimer_ == 0) {
                opacity_ = 1.0f;
            }
        }
    }



    void applyEnemyKnockback(Enemy<int>& enemy, int knockback_amount, int player_shield_direction) {
        switch (player_shield_direction) {
            case 0:
                enemy.y() += knockback_amount;
                break;
            case 1:
                enemy.y() -= knockback_amount;
                break;
            case 2:
                enemy.x() -= knockback_amount;
                break;
            case 3:
                enemy.x() += knockback_amount;
                break;
            default:
                break;
        }

        enemy.x() = std::max(100, std::min(enemy.x(), 1200 - enemy.width() - 100));
        enemy.y() = std::max(100, std::min(enemy.y(), 900 - enemy.height() - 100));
    }



    void check_collision(Enemy<int>& enemy, int inv_frame_direction) {
        int enemy_x = enemy.x();
        int enemy_y = enemy.y();
        int enemy_width = enemy.width();
        int enemy_height = enemy.height();
        int enemy_damage = enemy.damage_();


        if (CheckCollisionCircles({(float)x, (float)y}, getRadius(), {(float)enemy_x + enemy_width / 2, (float)enemy_y + enemy_height / 2}, enemy_width / 2)) {


            int player_knockback_amount = 70;
            int enemy_shield_knockback_amount = 100;


            int calculated_enemy_direction = getOppositeDirection(enemy_x, enemy_y);


            if (inv_frame_direction != -1 && inv_frame_direction == calculated_enemy_direction) {



                applyEnemyKnockback(enemy, enemy_shield_knockback_amount, inv_frame_direction);

            } else if (invincibilityTimer_ <= 0) {

                hp_ -= enemy_damage;


                float deltaX = (float)x - enemy_x;
                float deltaY = (float)y - enemy_y;

                if (std::abs(deltaX) > std::abs(deltaY)) {
                    if (deltaX > 0) { x += player_knockback_amount; }
                    else { x -= player_knockback_amount; }
                } else {
                    if (deltaY > 0) { y += player_knockback_amount; }
                    else { y -= player_knockback_amount; }
                }


                x = std::max(100, std::min(x, 1200 - 100 - 100));
                y = std::max(100, std::min(y, 900 - 100 - 100));

                invincibilityTimer_ = INVINCIBILITY_DURATION;
            } else {

            }
        }
    }
};