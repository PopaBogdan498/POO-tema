#pragma once
#include "raylib.h"
#include <math.h>

void GameOver(){
    //animation
    DrawText("Game Over!", 50, 50, 20, WHITE);
}

class player{
private:
    int invincibility=10;
    const int max_hp=40;
    int hp=max_hp;
    const int width=100;
    const int height=150;
    void decreaseHP(int damage){
        hp-=damage;
    }
public:
    const int radius=50;
    int damage=5;
    int x=600;
    int y=450;
    int opacity=1;
    void check_collision(int enemy_x, int enemy_y, int enemy_w, int enemy_h, int damage, int direction) {
        if (invincibility<=1) {
            if (CheckCollisionCircleLine(Vector2{x, y}, 50, Vector2{enemy_x, enemy_y + enemy_h - 1}, Vector2{enemy_x + enemy_w - 1, enemy_y + enemy_h - 1})
                && direction !=1){
                y= std::min(y+100, 749);
                decreaseHP(damage);
                invincibility=2;
            }//down
            if (CheckCollisionCircleLine(Vector2{x, y}, 50, Vector2{enemy_x, enemy_y}, Vector2{enemy_x, enemy_y + enemy_h - 1})
                && direction !=3){
                x= std::max(x-100, 151);
                decreaseHP(damage);
                invincibility=2;
                }//left
            if (CheckCollisionCircleLine(Vector2{x, y}, 50, Vector2{enemy_x + enemy_w - 1, enemy_y}, Vector2{enemy_x + enemy_w - 1, enemy_y + enemy_h - 1})
                && direction !=2){
                x= std::min(x+100, 1049);
                decreaseHP(damage);
                invincibility=2;
                }//right
            if (CheckCollisionCircleLine(Vector2{x, y}, 50, Vector2{enemy_x, enemy_y}, Vector2{enemy_x + enemy_w - 1, enemy_y})
                && direction != 0){
                y= std::max(y-100, 151);
                decreaseHP(damage);
                invincibility=2;
            }//up
        }
        else if (invincibility>=240){
            invincibility=1;
            opacity=1;
        }
        else {
            invincibility++;
            opacity = 0.5f + sinf((invincibility/240.0f) * 4 * PI);
        }
        DrawText(TextFormat("%i",hp),0,0,50,GREEN);
        if (hp<=0) {
            GameOver();
            hp=0;
        }
    }
};


