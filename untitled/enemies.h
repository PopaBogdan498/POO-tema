#pragma once
#include "raylib.h"
#include <stack>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string>

class EnemyObserver;

template <typename T>
class Enemy {
protected:
    T hp;
    int value_ = 1;
    int x_, y_, animation_, frame_;
    const int height_ = 100, width_ = 100;
    bool isDead_ = false;
    static EnemyObserver* observer;
public:
    static EnemyObserver* getObserver() { return observer; }

    Enemy(T health, int initial_x = 0, int initial_y = 0)
        : hp(health), x_(initial_x == 0 ? GetRandomValue(100, 1000) : initial_x),
          y_(initial_y == 0 ? GetRandomValue(100, 700) : initial_y)
    {


    }


    virtual ~Enemy() = default;

    int& x() { return x_; }
    int& y() { return y_; }
    int x() const { return x_; }
    int y() const { return y_; }
    int width() const { return width_; }
    int height() const { return height_; }
    int& animation(){return animation_;}
    int& value(){return value_;}
    int& frame(){return frame_;}
    bool& isDead(){return isDead_;}
    T& getHp(){return hp;}

    virtual int damage_() const { return 10; }

    virtual void walk_left() { x_ -= 5; }
    virtual void walk_right() { x_ += 5; }
    virtual void walk_up() { y_ -= 5; }
    virtual void walk_down() { y_ += 5; }


    virtual void update() {
        if (hp <= 0 && !isDead_) {
            isDead_ = true;
        }
        if (animation_ % 10 == 0) {
            frame_++;
            frame_ = frame_ % 4;
        }
    }

    virtual void draw() { }

    virtual void check_collision(int player_x, int player_y, T damage) {
        if (CheckCollisionRecs(Rectangle{static_cast<float>(player_x - 50), static_cast<float>(player_y + 50), 100, 50}, Rectangle{static_cast<float>(x()), static_cast<float>(y()), static_cast<float>(width()), static_cast<float>(height())})) {
            y_ = std::min(y_ + 100, 700);
            decreaseHP(damage);
        }
        if (CheckCollisionRecs(Rectangle{static_cast<float>(player_x - 100), static_cast<float>(player_y - 50), 100, 50}, Rectangle{static_cast<float>(x()), static_cast<float>(y()), static_cast<float>(width()), static_cast<float>(height())})) {
            x_ = std::max(x_ - 100, 101);
            decreaseHP(damage);
        }
        if (CheckCollisionRecs(Rectangle{static_cast<float>(player_x + 50), static_cast<float>(player_y - 50), 100, 50}, Rectangle{static_cast<float>(x()), static_cast<float>(y()), static_cast<float>(width()), static_cast<float>(height())})) {
            x_ = std::min(x_ + 100, 999);
            decreaseHP(damage);
        }
        if (CheckCollisionRecs(Rectangle{static_cast<float>(player_x - 50), static_cast<float>(player_y - 100), 100, 50}, Rectangle{static_cast<float>(x()), static_cast<float>(y()), static_cast<float>(width()), static_cast<float>(height())})) {
            y_ = std::max(y_ - 100, 101);
            decreaseHP(damage);
        }
    }


    virtual std::string getTypeString() const = 0;

private:
    void decreaseHP(T damage) {
        hp -= damage;
        if (hp <= 0) {
            isDead_ = true;
        }
    }
};

class EnemyObserver {
public:
    std::stack<std::unique_ptr<Enemy<int>>> deadEnemies;

    void registerDeath(std::unique_ptr<Enemy<int>> enemy) {
        if (enemy) {
            deadEnemies.push(std::move(enemy));
        }
    }

    std::unique_ptr<Enemy<int>> popDeadEnemy() {
        if (deadEnemies.empty()) return nullptr;
        std::unique_ptr<Enemy<int>> top = std::move(deadEnemies.top());
        deadEnemies.pop();
        return top;
    }


    void clearDeadEnemies() {
        while (!deadEnemies.empty()) {
            deadEnemies.pop();
        }
    }
};

template <>
EnemyObserver* Enemy<int>::observer = new EnemyObserver();

template <typename U>
void DrawEnemy(const Enemy<U>& e) {
    DrawRectangle(e.x(), e.y(), e.width(), e.height(), RED);
}

class Crab : public Enemy<int> {
public:
    Crab() : Enemy<int>(2000) { animation_ = 0; frame_ = 0; }
    int damage_() const override { return 15; }
    void walk_left() override { x_ -= 3; }
    void walk_right() override { x_ += 3; }
    void walk_up() override { y_ -= 3; }
    void walk_down() override { y_ += 3; }
    std::string getTypeString() const override { return "Crab"; }
};

class Scorpion : public Enemy<int> {
public:
    Scorpion() : Enemy<int>(2000) { animation_ = 0; frame_ = 0; }
    int damage_() const override { return 20; }
    void walk_left() override { x_ -= 8; }
    void walk_right() override { x_ += 8; }
    void walk_up() override { y_ -= 8; }
    void walk_down() override { y_ += 8; }
    std::string getTypeString() const override { return "Scorpion"; }
};


class Teleporter : virtual public Enemy<int> {
public:
    int teleportTimer;
    const int TELEPORT_INTERVAL = 180;



    Teleporter(int health = 2000) : Enemy<int>(health), teleportTimer(0) { animation_ = 0; frame_ = 0; }
    int damage_() const override { return 10; }

    void update() override {


        teleportTimer++;
        if (teleportTimer >= TELEPORT_INTERVAL) {
            x_ = GetRandomValue(100, 1200 - width_ - 100);
            y_ = GetRandomValue(100, 900 - height_ - 100);
            teleportTimer = 0;
        }
    }
    std::string getTypeString() const override { return "Teleporter"; }
};


class Necromancer : virtual public Enemy<int> {
public:
    int maxRevives;
    int timer = 0;
    const int REVIVE_INTERVAL = 1200;



    Necromancer(int health = 2000, int revives = 5) : Enemy<int>(health), maxRevives(revives) { animation_ = 0; frame_ = 0; }
    int damage_() const override { return 10; }

    void update() override {


        timer++;
    }

    bool isReadyToRevive() const {
        return timer >= REVIVE_INTERVAL;
    }

    void resetReviveTimer() {
        timer = 0;
    }
    std::string getTypeString() const override { return "Necromancer"; }
};


class Boss : public Teleporter, public Necromancer {
public:



    Boss(int health = 2000, int maxRevives = 5)
        : Enemy<int>(health),
          Teleporter(health),
          Necromancer(health, maxRevives)
    {
        animation_ = 0;
        frame_ = 0;
    }

    int damage_() const override { return 25; }

    void update() override {


        Enemy<int>::update();



        Teleporter::update();
        Necromancer::update();
    }
    std::string getTypeString() const override { return "Boss"; }


};

class EnemyFactory {
public:
    static std::unique_ptr<Crab> CreateCrab() {
        return std::make_unique<Crab>();
    }
    static std::unique_ptr<Scorpion> CreateScorpion() {
        return std::make_unique<Scorpion>();
    }
    static std::unique_ptr<Necromancer> CreateNecromancer(int maxRevives) {
        return std::make_unique<Necromancer>(2000, maxRevives);
    }
    static std::unique_ptr<Teleporter> CreateTeleporter() {
        return std::make_unique<Teleporter>(2000);
    }

    static std::unique_ptr<Boss> CreateBoss(int health = 2000, int maxRevives = 5) {
        return std::make_unique<Boss>(health, maxRevives);
    }
};