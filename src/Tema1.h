#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        // colors
        glm::vec3 head_color = glm::vec3(1, 0.5f, 0.5f);
        glm::vec3 eye_color = glm::vec3(0.5f, 0.7f, 1);
        glm::vec3 color1 = glm::vec3(0, 0.3f, 1);
        glm::vec3 color2 = glm::vec3(1, 1, 1);
        glm::vec3 color3 = glm::vec3(0.8f, 0, 0.2f);
        glm::vec3 color4 = glm::vec3(1, 0, 0);
        glm::vec3 color5 = glm::vec3(0, 1, 1);
        glm::vec3 color6 = glm::vec3(0, 0, 0);
        // for window & camera & map
        glm::ivec2 resolution;
        float xcamera = 160, ycamera = 340;
        float mapscalex = 80, mapscaley = 70;
        // for scaling
        float cx, cy;
        float scaleX, scaleY;
        // sizes and lengths
        float radius = 40;
        float radius_eye = 10;
        float length = 20;
        float enemysize = 80;
        // for character
        float x_head = length * mapscalex / 2, y_head = length * mapscaley / 2;
        float x_eye1 = x_head - 20, y_eye1 = y_head - 40;
        float x_eye2 = x_head + 20, y_eye2 = y_head - 40;
        float angle;
        float pi = 3.14159;
        float a, b;
        float speed = 200;
        // for projectile
        float firerate = 1;
        bool weapon;
        float transxproj, transyproj;
        float angle1;
        float x_tinta, y_tinta;
        float from_x = 0, from_y = 0;
        float step = 0;
        bool shootagain = true;
        float time = 0;
        float currenttime = 0;
        float maxdistance = 15000;
        float currdistance = 0;
        // for obstacles
        float ox1 = 20, oy1 = 30, ox2 = 60, oy2 = 10, ox3 = 50, oy3 = 30, ox4 = 10, oy4 = 10;
        float sx1 = 10, sy1 = 20, sx2 = 15, sy2 = 15, sx3 = 15, sy3 = 25, sx4 = 15, sy4 = 10;
        // for enemy
        struct coord {
            float xenemy;
            float yenemy;
            float transxenemy;
            float transyenemy;
            float enemystep;
            float speed;
        };
        std::vector<coord> enemies;
        bool bec = true;
        // coliziune inamic - proiectil
        float distance = 0;
        float scor = 0;
        // coliziune inamic - personaj
        float dist = 0;
        // health bar
        float xbar = length * mapscalex - 340;
        float ybar = length * mapscaley - 400;
        float sxbar = 2, sybar = 0.5f;
        float sxbar_default = 2, sybar_default = 0.5f;
        float sxbar_nofill = 2, sybar_nofill = 0.5f;
        // misc
        float cnt = 0;
        glm::mat3 modelMatrix;
        float x = 0, y = 0;
        bool ok = 0;
        bool play = 1;
        // hp
        bool drawhp = true;
        float xhp, yhp;
        bool bec1 = true;
        float sxhp = 0.5f, syhp = 0.5f;
        float distancehp = 0;
        // power-up
        bool drawpower = true;
        float rad = 10;
        float xpower, ypower;
        bool bec2 = true;
        float sxpow = 2, sypow = 2;
        float distancepower = 0;
    };
}
