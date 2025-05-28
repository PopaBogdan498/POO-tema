#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include <algorithm>

class EnemyObserver;

template <typename T>
class Enemy {
public:
    T hp;
    int value = 1;
    int x, y, animation, frame;
    const int height = 100, width = 100;
    bool isDead = false;
    static EnemyObserver* observer;

    Enemy(T health) : hp(health), x(GetRandomValue(100, 1000)), y(GetRandomValue(100, 700)) {}

    virtual void walk_left() { x -= 5; }
    virtual void walk_right() { x += 5; }
    virtual void walk_up() { y -= 5; }
    virtual void walk_down() { y += 5; }

    virtual void update() {
        if (hp <= 0 && !isDead) {
            isDead = true;
            if (observer) observer->registerDeath(this);
        }
    }

    virtual void draw() {
        if (!isDead) {
            if (animation % 10 == 0) {
                frame++;
                frame = frame % 4;
            }
        }
    }

    virtual void check_collision(int player_x, int player_y, T damage) {
        if (CheckCollisionRecs(Rectangle{player_x - 50, player_y + 50, 100, 50}, Rectangle{x, y, width, height})) {
            y = std::min(y + 100, 700);
            decreaseHP(damage);
        }
        if (CheckCollisionRecs(Rectangle{player_x - 100, player_y - 50, 100, 50}, Rectangle{x, y, width, height})) {
            x = std::max(x - 100, 101);
            decreaseHP(damage);
        }
        if (CheckCollisionRecs(Rectangle{player_x + 50, player_y - 50, 100, 50}, Rectangle{x, y, width, height})) {
            x = std::min(x + 100, 999);
            decreaseHP(damage);
        }
        if (CheckCollisionRecs(Rectangle{player_x - 50, player_y - 100, 100, 50}, Rectangle{x, y, width, height})) {
            y = std::max(y - 100, 101);
            decreaseHP(damage);
        }
    }

private:
    void decreaseHP(T damage) {
        hp -= damage;
        if (hp <= 0) {
            isDead = true;
            if (observer) observer->registerDeath(this);
        }
    }
};

class EnemyObserver {
public:
    std::vector<std::unique_ptr<Enemy<int>>> deadEnemies;

    void registerDeath(Enemy<int>* enemy) {
        deadEnemies.push_back(std::unique_ptr<Enemy<int>>(enemy));
    }

    void removeEnemy(Enemy<int>* enemy) {
        auto it = std::find_if(deadEnemies.begin(), deadEnemies.end(),
            [enemy](const std::unique_ptr<Enemy<int>>& e) { return e.get() == enemy; });

        if (it != deadEnemies.end()) {
            deadEnemies.erase(it);
        }
    }
};

template <>
EnemyObserver* Enemy<int>::observer = new EnemyObserver();

template <typename U>
void DrawEnemy(const Enemy<U>& e) {
    DrawRectangle(e.x, e.y, e.width, e.height, RED);
}

class Crab : public Enemy<int> {
public:
    Crab() : Enemy<int>(50) {}
    void walk_left() override { x -= 3; }
    void walk_right() override { x += 3; }
    void walk_up() override { y -= 3; }
    void walk_down() override { y += 3; }
};

class Scorpion : public Enemy<int> {
public:
    Scorpion() : Enemy<int>(30) {}
    void walk_left() override { x -= 8; }
    void walk_right() override { x += 8; }
    void walk_up() override { y -= 8; }
    void walk_down() override { y += 8; }
};

class Necromancer : virtual public Enemy<int> {
public:
    std::vector<std::unique_ptr<Enemy<int>>> deadEnemies;
    int maxRevives;
    int timer = 0;

    Necromancer() : Enemy<int>(20), maxRevives(5) {}
    Necromancer(int maxRevives) : Enemy<int>(20), maxRevives(maxRevives) {}

    void update() override {
        timer++;
        if (timer >= 1200) {
            timer = 0;
            if (!deadEnemies.empty() && deadEnemies.size() < maxRevives) {
                deadEnemies.pop_back();
            }
        }
    }

    void walk_left() override { x -= 2; }
    void walk_right() override { x += 2; }
    void walk_up() override { y -= 2; }
    void walk_down() override { y += 2; }
};

class Teleporter : virtual public Enemy<int> {
public:
    bool isVisible = true;
    int timer = 0;

    Teleporter() : Enemy<int>(100) {}

    void walk_left() override {}
    void walk_right() override {}
    void walk_up() override {}
    void walk_down() override {}

    void update() override {
        timer++;
        if (timer >= 420) {
            isVisible = !isVisible;
            timer = 0;
            x = GetRandomValue(100, 1000);
            y = GetRandomValue(100, 700);
            std::cout << "Teleporter moved to: " << x << ", " << y << std::endl;
        }
    }

    void draw() override {
        if (isVisible) {
            DrawEnemy(*this);
        }
    }
};

class Boss : public Necromancer, public Teleporter {
public:
    Boss() : Enemy<int>(120), Necromancer(5), Teleporter() {}
    Boss(int maxRevives) : Enemy<int>(120), Necromancer(maxRevives), Teleporter() {}

    void update() override {
        Necromancer::update();
        Teleporter::update();
    }

    void walk_left() override { x -= 2; }
    void walk_right() override { x += 2; }
    void walk_up() override { y -= 2; }
    void walk_down() override { y += 2; }
};

class EnemyFactory {
public:
    static Crab CreateCrab() { return Crab(); }
    static Scorpion CreateScorpion() { return Scorpion(); }
    static Necromancer CreateNecromancer(int maxRevives) { return Necromancer(maxRevives); }
    static Teleporter CreateTeleporter() { return Teleporter(); }
    static Boss CreateBoss(int maxRevives) { return Boss(maxRevives); }
};
