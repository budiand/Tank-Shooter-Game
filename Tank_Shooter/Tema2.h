#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/Tema2/tank.h"
#include "lab_m1/Tema2/turret.h"
#include "lab_m1/Tema2/projectile.h"
#include "lab_m1/Tema2/enemy.h"
#include "lab_m1/Tema2/building.h"

#include <random>

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderTankMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float hp);
        void RenderTank(glm::vec3 coord, float tankAngle, float cannon_angle, float turret_angle, const glm::vec3& color1, const glm::vec3& color2, float hp);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 orthoMatrix;
        
        // TODO(student): If you need any other class variables, define them here.
        bool isOrtho = false;
        float orthoRight = 8.0f, orthoUp = 4.5f, orthoDown = -4.5f, orthoLeft = -8.0f;
        
        //mini view port
        ViewportArea miniViewportArea;
        glm::ivec2 resolution = window->GetResolution();


        // tank
        Tank tank;
        GLboolean right_click;

        // turret
        Turret turret;
        float  cannon_angle;
        GLboolean stay, right, left, up, down;

        // projectiles
        std::vector<Projectile> projectiles;
        float projectile_time; // time between each render for tank projectiles

        // enemies
        std::vector<Enemy> enemies;
        int enemies_number;

        std::random_device random_coord;
        std::mt19937 gen{ random_coord() };
        std::uniform_real_distribution<float> dist_coord{ -10.0f, 30.0f }; // coordinates(x,z)
        std::uniform_real_distribution<float> dist_enemy_angle{ 0.0f, 360.0f }; // angle

        std::uniform_int_distribution<int> dist_states{ 1, 4 };
        float random_state;
        float state_timer;
        int curr_state, next_state;

        // buildings
        std::vector<Building> buildings;
        std::uniform_real_distribution<float> dist_build_coord{ -10.0f, 40.0f }; // coordinates(x,z)
        std::uniform_real_distribution<float> dist_build_scale{ 2.0f, 5.0f }; // scaling(sx,sy,sz)
        std::uniform_real_distribution<float> dist_build_angle{ 0.0f, 360.0f }; // angle
        int buildings_number;

        // shaders
        GLboolean is_hit;

        // end game
        int score;
        float end_timer;
        GLboolean game_over;
    };
}   // namespace m1
