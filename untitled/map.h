#pragma once
class map {
private:
    int  u, d, l, r;
public:
    int number = 75;

    template <typename T>
void DrawWall(T x, T y, T width, T height, Color color = BLUE) {
        DrawRectangle(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height), color);
    }
    int u_() const { return u; }

    int d_() const { return d; }

    int l_() const { return l; }

    int r_() const { return r; }



    void cross() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=525;
        l=350;
        r=350;
        d=525;
    }

    void left_t() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=525;
        l=350;
        r=900;
        d=525;
    }

    void right_t() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=525;
        l=900;
        r=350;
        d=525;
    }

    void up_t() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 1200, 100);
        u=525;
        l=350;
        r=350;
        d=1200;
    }

    void down_t() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=1200;
        l=350;
        r=350;
        d=525;
    }

    void ul_l() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 1200, 100);
        u=525;
        l=350;
        r=900;
        d=1200;
    }

    void ur_l() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 1200, 100);
        u=525;
        l=900;
        r=350;
        d=1200;
    }

    void dl_l() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=1200;
        l=350;
        r=900;
        d=525;
    }

    void dr_l() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=1200;
        l=900;
        r=350;
        d=525;
    }

    void hor_line() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 350);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 1200, 100);
        u=1200;
        l=350;
        r=350;
        d=1200;
    }

    void ver_line() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=525;
        l=900;
        r=900;
        d=525;
    }

    void up() {
        DrawWall(0, 0, 525, 100);
        DrawWall(675, 0, 525, 100);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 1200, 100);
        u=525;
        l=900;
        r=900;
        d=1200;
    }

    void down() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 525, 100);
        DrawWall(675, 800, 525, 100);
        u=1200;
        l=900;
        r=900;
        d=525;
    }

    void left() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(0, 0, 100, 350);
        DrawWall(0, 550, 100, 350);
        DrawWall(1100, 0, 100, 900);
        DrawWall(0, 800, 1200, 100);
        u=1200;
        l=350;
        r=900;
        d=1200;
    }

    void right() {
        DrawWall(0, 0, 1200, 100);
        DrawWall(1100, 0, 100, 350);
        DrawWall(0, 0, 100, 900);
        DrawWall(1100, 550, 100, 350);
        DrawWall(0, 800, 1200, 100);
        u=1200;
        l=900;
        r=350;
        d=1200;
    }
};