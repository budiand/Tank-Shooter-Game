#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/tank.h"
#include "lab_m1/Tema2/turret.h"
#include "lab_m1/lab4/transform3D.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tankcentrat");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tankcentrat.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("senila1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("senila2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senila2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("ground");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // end game after 1 min
    end_timer = 60.0f;
    score = 0;    game_over = false;

    // tank obj
    tank.init(-15, 0.3f, 0, 0, 1, 0.5f, 1, false);
    tank.set_turret(turret);
    tank.turret.init(0);

    // turret obj
    cannon_angle = 0;
    stay = false;    right = false;   left = false;

    // mouse move
    right_click = false;

    // enemy obj
    enemies_number = 10;

    // states
    state_timer = 0.5f;     next_state = 0;
    
    // buildings obj
    buildings_number = 14;
    
    // camera
    camera = new implemented::Camera();
    camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
        glm::vec3(tank.x, tank.y, tank.z),
        glm::vec3(0, 1, 0));

    // small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    orthoMatrix = glm::ortho(
        static_cast<float>(miniViewportArea.x), static_cast<float>(miniViewportArea.x + miniViewportArea.width),
        static_cast<float>(miniViewportArea.y), static_cast<float>(miniViewportArea.y + miniViewportArea.height),
        -1.0f, 1.0f  // Near and far planes
    );
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::RenderTank(glm::vec3 coord, float tankAngle, float turret_angle, float cannon_angle, const glm::vec3& color1, const glm::vec3& color2, float hp) {
    // Render senila1
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(coord.x, 0, coord.z);
        modelMatrix *= transform3D::RotateOY(tankAngle);
        RenderTankMesh(meshes["senila1"], shaders["TemaShader"], modelMatrix, glm::vec3(108 / 255.f, 117 / 255.f, 125 / 255.f), hp);
    }

    // Render senila2
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(coord.x, 0, coord.z);
        modelMatrix *= transform3D::RotateOY(tankAngle);
        RenderTankMesh(meshes["senila2"], shaders["TemaShader"], modelMatrix, glm::vec3(108 / 255.f, 117 / 255.f, 125 / 255.f), hp);
    }

    // Render tankcentrat_bottom
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(coord.x, coord.y, coord.z);
        modelMatrix *= transform3D::RotateOY(tankAngle);
        RenderTankMesh(meshes["tankcentrat"], shaders["TemaShader"], modelMatrix, color1, hp);
    }

    // Render tankcentrat_top
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(coord.x, coord.y + 0.5f, coord.z);
        modelMatrix *= transform3D::RotateOY(turret_angle);    
        modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
        RenderTankMesh(meshes["tankcentrat"], shaders["TemaShader"], modelMatrix, color2, hp);
    }

    // Render tun
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(coord.x, coord.y + 0.625f, coord.z);
        modelMatrix *= transform3D::RotateOY(turret_angle);
        modelMatrix *= transform3D::RotateOZ(cannon_angle);
        RenderTankMesh(meshes["tun"], shaders["TemaShader"], modelMatrix, glm::vec3(108 / 255.f, 117 / 255.f, 125 / 255.f), hp);
    }
}


void Tema2::Update(float deltaTimeSeconds)
{  
    // ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(100, 0, 100));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(6));
        RenderTankMesh(meshes["ground"], shaders["TemaShader"], modelMatrix, glm::vec3(167 / 255.f, 201 / 255.f, 87 / 255.f), 1);    
    }
    

    // rendering tank 
// turret movement 
    if (right && stay == false) {
        tank.turret.angleX -=  deltaTimeSeconds;
    }
    if (left && stay == false) {
        tank.turret.angleX +=  deltaTimeSeconds;
    }

    /*if (up && stay == false) {
        cannon_angle -= 0.7 * deltaTimeSeconds;
    }
    if (down && stay == false) {
        cannon_angle += 0.7 * deltaTimeSeconds;
    }*/
   
    RenderTank(glm::vec3(tank.x, tank.y, tank.z), tank.angle, tank.turret.angleX, cannon_angle, glm::vec3(52 / 255.f, 78 / 255.f, 65 / 255.f), glm::vec3(88 / 255.f, 129 / 255.f, 87 / 255.f), tank.hp);
    

    // spawning random buildings
    if (buildings_number > 0) {
        Building building(dist_build_coord(gen), 0, dist_build_coord(gen), // (x,y,z)
            0, // angle
            dist_build_scale(gen), dist_build_scale(gen), dist_build_scale(gen)); // (sx,sy,sz)

        buildings_number--;

        buildings.push_back(building);
    }
    for (auto& building : buildings) {

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(building.x, building.y, building.z));
        modelMatrix *= transform3D::RotateOY(building.angle);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(building.sx, building.sy, building.sz));
        RenderTankMesh(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(252/255.f, 163/255.f, 17/255.f), 1);
    }


    // spawning some enemies at random coordinates
    if (enemies_number > 0) {

        Enemy enemy(dist_coord(gen), 0.3f, dist_coord(gen), // (x,y,z)
                    0, // angle
                    true, false); // exists, broken

        enemy.init(0.5f, // top_r
                    1, // bot_r
                    2.0f, // state_timer
                    1); // hp

        Turret turret;
        enemy.set_turret(turret);
        enemy.turret.init(0);
        enemy.projectile_time = -1; // used for spawning projectiles at equal intervals

        enemies_number--;

        enemies.push_back(enemy);
    }


    // enemy movement based on random states

        state_timer -= deltaTimeSeconds; // time in a state
        if (state_timer <= 0) {
            for (auto& enemy : enemies) {
                if (enemy.exists && !enemy.broken && !game_over) {
                    enemy.state = next_state;
                    next_state = dist_states(gen);
                }
            }
            state_timer = 3.0f;  
        }

        for (auto& enemy : enemies) {
            if (enemy.exists) {

                // forward
                if (enemy.state == 1 && enemy.broken == false && !game_over) {
                    enemy.x += deltaTimeSeconds * 2 * cos(-enemy.angle);
                    enemy.z += deltaTimeSeconds * 2 * sin(-enemy.angle);
                }

                // back
                if (enemy.state == 2 && enemy.broken == false && !game_over) {
                    enemy.x -= deltaTimeSeconds * 2 * cos(-enemy.angle);
                    enemy.z -= deltaTimeSeconds * 2 * sin(-enemy.angle);
                }

                // left
                if (enemy.state == 3 && enemy.broken == false && !game_over) {
                    enemy.angle += deltaTimeSeconds;
                    enemy.turret.angleX += deltaTimeSeconds;
                }

                // right
                if (enemy.state == 4 && enemy.broken == false && !game_over) {
                    enemy.angle -= deltaTimeSeconds;
                    enemy.turret.angleX -= deltaTimeSeconds;
                }
                RenderTank(glm::vec3(enemy.x, enemy.y, enemy.z), enemy.angle, enemy.turret.angleX, 0, glm::vec3(120 / 255.f, 0 / 255.f, 0 / 255.f), glm::vec3(193 / 255.f, 18 / 255.f, 31 / 255.f), enemy.hp);
            }
        }
    

    // projectiles rendering and dissapearing after a period of time
    for (auto& projectile : projectiles) {
        if (projectile.exists) {

            projectile.x += deltaTimeSeconds * 10 * cos(-projectile.angle);
            projectile.z += deltaTimeSeconds * 10 * sin(-projectile.angle);

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(projectile.x, projectile.y, projectile.z);
            modelMatrix *= transform3D::Scale(projectile.radius, projectile.radius, projectile.radius);
            RenderTankMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0, 0), 1);

            projectile.ttl -= deltaTimeSeconds;
            if (projectile.ttl <= 0)
                projectile.exists = false;
        }
    }
    projectile_time -= deltaTimeSeconds;


// COLLISIONS

    // tank-buidling && projectile-building collisions 
    for (auto& building : buildings) {
        tank.checkCollisionWithBuilding(building);
    }

    for (auto& enemy : enemies) {
        for (auto& building : buildings) {
            for (auto& projectile : enemy.projectiles) {
                projectile.checkCollisionWithBuilding(building);
            }
            enemy.checkCollisionWithBuilding(building);
        }
    }
   
    for (auto& projectile : projectiles) {
        for (auto& building : buildings) {
            projectile.checkCollisionWithBuilding(building);
        }
    }
    
    
    // projectile-enemy collision
    for (auto& enemy : enemies) {
        for (auto& projectile : projectiles) {
            if (projectile.exists) {
                float distance = sqrt(pow(projectile.x - enemy.x, 2) + pow(projectile.y - (enemy.y + 0.625f), 2) + pow(projectile.z - enemy.z, 2));

                if (projectile.radius + enemy.top_radius > distance) {

                    projectile.exists = false;

                    if (!enemy.broken) 
                        enemy.hp++;

                    if (enemy.hp == 4) {
                        enemy.broken = true;
                        score++;
                    }
                }
            }
        }
    }


    // tank-tank(enemy) collision
    for (auto& enemy : enemies) {
        if (enemy.exists) {
            float distance = sqrt(pow(tank.x - enemy.x, 2) + pow(tank.y - enemy.y, 2) + pow(tank.z - enemy.z, 2));

            if (tank.radius + enemy.bot_radius > distance) {

                float OD = tank.radius + enemy.bot_radius - distance;
                glm::vec3 Dif = glm::vec3(enemy.x - tank.x, enemy.y - tank.y, enemy.z - tank.z);
                glm::vec3 P = OD * glm::normalize(Dif);
                tank.x += P.x * -0.5f; tank.y += P.y * -0.5f; tank.z += P.z * -0.5f;
                enemy.x += P.x * 0.5f; enemy.y += P.y * 0.5f; enemy.z += P.z * 0.5f;

                // setting the camera right
                if (!right_click) {
                    camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
                        glm::vec3(tank.x, tank.y, tank.z),
                        glm::vec3(0, 1, 0));
                }
            }
        }
    }


    // enemy-enemy collision
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            auto& enemy1 = enemies[i];
            auto& enemy2 = enemies[j];
            if (enemy1.exists && enemy2.exists) {
                float distance = sqrt(pow(enemy1.x - enemy2.x, 2) + pow(enemy1.y - enemy2.y, 2) + pow(enemy1.z - enemy2.z, 2));

                if (enemy1.bot_radius + enemy2.bot_radius > distance) {

                    float OD = enemy1.bot_radius + enemy2.bot_radius - distance;
                    glm::vec3 Dif = glm::vec3(enemy2.x - enemy1.x, enemy2.y - enemy1.y, enemy2.z - enemy1.z);
                    glm::vec3 P = OD * glm::normalize(Dif);
                    enemy1.x += P.x * -0.5f; enemy1.y += P.y * -0.5f; enemy1.z += P.z * -0.5f;
                    enemy2.x += P.x * 0.5f; enemy2.y += P.y * 0.5f; enemy2.z += P.z * 0.5f;
                }
            }
        }
    }

    // projectile-tank collision
    for (auto& enemy : enemies) {
        for (auto& projectile : enemy.projectiles) {
            if (projectile.exists && enemy.exists && !tank.broken) {
                float distance = sqrt(pow(projectile.x - tank.x, 2) + pow(projectile.y - (tank.y + 0.625f), 2) + pow(projectile.z - tank.z, 2));

                if (projectile.radius + tank.top_radius > distance) {

                    projectile.exists = false;
                    
                    if(!tank.broken)
                        tank.hp++;

                    if (tank.hp == 4) 
                        tank.broken = true;
                }
            }
        }
    }
    

    // enemy attack on tank
    for (auto& enemy : enemies) {
        if (!enemy.broken && !tank.broken) {
            float distance = sqrt(pow(tank.x - enemy.x, 2) + pow(tank.y - enemy.y, 2) + pow(tank.z - enemy.z, 2));

            if (distance <= 7) {
                // Attack mode
                // vector, get the angle
                float dx = tank.x - enemy.x;
                float dz = tank.z - enemy.z;
                float angle = atan2(dz, dx);

                enemy.turret.angleX = -angle;

                if (enemy.projectile_time <= 0) {
                    Projectile enemy_projectile(enemy.x + 1.4f * cos(-enemy.turret.angleX), enemy.y + 0.625f, enemy.z + 1.4f * sin(-enemy.turret.angleX), enemy.turret.angleX, 0.1f, 3.0f, true);
                    enemy.projectiles.push_back(enemy_projectile);
                    enemy.projectile_time = 4.5f;
                }
            }
        }
        for (auto& enemy_projectile : enemy.projectiles) {
            if (enemy_projectile.exists) {

                enemy_projectile.x += deltaTimeSeconds * 5 * cos(-enemy_projectile.angle);
                enemy_projectile.z += deltaTimeSeconds * 5 * sin(-enemy_projectile.angle);

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(enemy_projectile.x, enemy_projectile.y, enemy_projectile.z);
                modelMatrix *= transform3D::Scale(enemy_projectile.radius, enemy_projectile.radius, enemy_projectile.radius);
                RenderTankMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0, 0), 1);

                enemy_projectile.ttl -= deltaTimeSeconds;
                if (enemy_projectile.ttl <= 0)
                    enemy_projectile.exists = false;
            }
        }
        enemy.projectile_time -= deltaTimeSeconds;
    }


    // exit the game afte 1 min
    end_timer -= deltaTimeSeconds;
    if (end_timer <= 0 && !game_over) {
        game_over = true;
        std::cout << "Good job! Your score is: " << score << "! <3<3<3" << std::endl;
    }

    if (tank.broken && !game_over) {
        game_over = true;
        std::cout << "Game is over :( Try better next time! " << std::endl;
    }


    // mini view port
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
  
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(tank.x, tank.y, tank.z);
    RenderTankMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(52 / 255.f, 78 / 255.f, 65 / 255.f), tank.hp);

    for (auto& enemy : enemies) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(enemy.x, enemy.y, enemy.z);
        RenderTankMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(120 / 255.f, 0 / 255.f, 0 / 255.f), enemy.hp);
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderTankMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float hp)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();

    // Set the view matrix uniform
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Set the projection matrix uniform
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Set the model matrix uniform
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Set uniform color
    GLint loc_uniform_color = glGetUniformLocation(shader->program, "uniform_color");
    if (loc_uniform_color != -1) {
        glUniform3fv(loc_uniform_color, 1, glm::value_ptr(color));
    }

    // Shader hp
    GLuint enemyHPLoc = glGetUniformLocation(shader->program, "hp");
    glUniform1f(enemyHPLoc, hp);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    // Render the mesh
    mesh->Render();
}




void Tema2::OnInputUpdate(float deltaTime, int mods)
{
   // if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) /// sterg asta daca vreau sa pot sa misc si tancul in acelasi timp
    //{
    if (!game_over) {
        float cameraSpeed = 5.0f;
        float rotationSpeed = 0.5f;

        if (window->KeyHold(GLFW_KEY_W)) {

            camera->MoveForward(deltaTime * cameraSpeed);
            tank.x += deltaTime * cameraSpeed * cos(-tank.angle);
            tank.z += deltaTime * cameraSpeed * sin(-tank.angle);
            if (!right_click) {
                camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
                    glm::vec3(tank.x, tank.y, tank.z),
                    glm::vec3(0, 1, 0));
            }

        }

        if (window->KeyHold(GLFW_KEY_A)) {

            tank.angle += deltaTime;
            tank.turret.angleX += deltaTime;

            if (!right_click) {
                camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
                    glm::vec3(tank.x, tank.y, tank.z),
                    glm::vec3(0, 1, 0));


            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {

            tank.angle -= deltaTime;
            tank.turret.angleX -= deltaTime; 

            if (!right_click) {
                camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
                    glm::vec3(tank.x, tank.y, tank.z),
                    glm::vec3(0, 1, 0));


            }
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-deltaTime * cameraSpeed);
            tank.x -= deltaTime * cameraSpeed * cos(-tank.angle);
            tank.z -= deltaTime * cameraSpeed * sin(-tank.angle);

            if (!right_click) {
                camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
                    glm::vec3(tank.x, tank.y, tank.z),
                    glm::vec3(0, 1, 0));


            }
        }

        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
            right_click = true;
        }
    }
    //}
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) 
        stay = true; // turret  
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_SPACE) {
        stay = false;  // turret
    }
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;
    if (!game_over) {
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {
            right_click = true;
                   
            camera->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
        }
    }

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !right_click) {

        if (!stay) {
            tank.turret.angleX -= deltaX * 0.01f * 0.5f;
            // cannon_angle -= deltaY * 0.001f;
            left = false;
            right = false;
        }
        if (!stay && mouseX >= resolution.x - 10) {
            right = true;
            left = false;
        }
        if (!stay && mouseX <= 10) {
            left = true;
            right = false;
        }

        right_click = false;

    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (!game_over) {
        if (button == GLFW_MOUSE_BUTTON_2 && projectile_time <= 0) {
            Projectile projectile(tank.x + 1.4f * cos(-tank.turret.angleX), tank.y + 0.625f, tank.z + 1.4f * sin(-tank.turret.angleX), tank.turret.angleX, 0.1f, 3.0f, true);
            projectiles.push_back(projectile);

            projectile_time = 1;
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (button == GLFW_MOUSE_BUTTON_3) {
        camera->Set(glm::vec3(tank.x - 3 * cos(-tank.angle), 1.5f, tank.z - 3 * sin(-tank.angle)),
            glm::vec3(tank.x, tank.y, tank.z),
            glm::vec3(0, 1, 0));
        right_click = false;
    }
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
