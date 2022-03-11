#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

struct coord {
    float xenemy;
    float yenemy;
    float transxenemy;
    float transyenemy;
    float enemystep;
};

std::vector<coord> enemies;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    weapon = false;
    vector<struct coord> enemies;

    Mesh* head = object2D::CreateCircle("head", glm::vec3(x, y, 0), radius, head_color, false);
    AddMeshToList(head);

    Mesh* eye = object2D::CreateCircle("eye", glm::vec3(x, y, 0), radius_eye, eye_color, false);
    AddMeshToList(eye);

    Mesh* square = object2D::CreateSquare("square", glm::vec3(x, y, 0), length, color1, true);
    AddMeshToList(square);

    Mesh* whitesq = object2D::CreateSquare("whitesq", glm::vec3(x, y, 0), length, color2, true);
    AddMeshToList(whitesq);

    Mesh* redsq = object2D::CreateSquare("redsq", glm::vec3(x, y, 0), enemysize, color3, true);
    AddMeshToList(redsq);

    Mesh* blacksq = object2D::CreateSquare("blacksq", glm::vec3(x, y, 0), enemysize / 4, color6, true);
    AddMeshToList(blacksq);

    Mesh* redsq_nofill = object2D::CreateSquare("redsq_nofill", glm::vec3(x, y, 0), enemysize, color3, false);
    AddMeshToList(redsq_nofill);

    Mesh* projectile = object2D::CreateSquare("projectile", glm::vec3(x, y, 0), length, color4, true);
    AddMeshToList(projectile);

    Mesh* hp = object2D::CreateCircle("hp", glm::vec3(x, y, 0), radius, color5, false);
    AddMeshToList(hp);

    Mesh* power = object2D::CreateCircle("power", glm::vec3(x, y, 0), rad, color6, false);
    AddMeshToList(power);
}


void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    if (scor == 30) {
        if (ok == 0) {
            std::cout << "You win! \n";
            std::cout << "Press space if you want to play again. \n";
            ok = 1;
            play = 0;
        }
    } 
    else if (sxbar < 0.4) {
        if (ok == 0) {
            std::cout << "You lose. Good luck next time! :) \n";
            std::cout << "Press space if you want to try again. \n";
            ok = 1;
            play = 0;
        }
    }
    if (play) {
        auto camera = GetSceneCamera();
        camera->SetPosition(glm::vec3(xcamera, ycamera, 50));
        camera->SetRotation(glm::vec3(0, 0, 0));
        camera->Update();
        GetCameraInput()->SetActive(false);

        // timpul curent
        cnt = glfwGetTime();

        // power-up din 5 in 5 secunde
        if ((int)cnt % 5 == 0 && bec2) {
            bec2 = false;
            drawpower = true;
            xpower = 0 + rad * 2 + rand() % (int)(length * mapscalex - rad * 2);
            ypower = 0 + rad * 2 + rand() % (int)(length * mapscaley - rad * 2);
        }
        if ((int)cnt % 5 == 1) {
            bec2 = true;
        }

        if (drawpower) {
            // desen si transformari power-up
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(xpower, ypower);
            modelMatrix *= transform2D::Scale(sxpow, sypow);
            modelMatrix *= transform2D::Translate(-xpower, -ypower);
            modelMatrix *= transform2D::Translate(xpower, ypower);
            RenderMesh2D(meshes["power"], shaders["VertexColor"], modelMatrix);
        }

        // coliziune personaj power-up
        if (drawpower) {
            distancepower = sqrt(pow(x_head - xpower, 2) + pow(y_head - ypower, 2));
            if (distancepower <= radius + rad * sxpow) {
                if (firerate < 0.5f) {
                    std::cout << "Maximum fire rate achieved!" << "\n";
                    drawpower = false;
                }
                else {
                    firerate -= 0.1f;
                    drawpower = false;
                }
            }
        }


        // heal din 7 in 7 secunde
        if ((int)cnt % 7 == 0 && bec1) {
            bec1 = false;
            drawhp = true;
            xhp = 0 + radius * 2 + rand() % (int)(length * mapscalex - radius * 2);
            yhp = 0 + radius * 2 + rand() % (int)(length * mapscaley - radius * 2);
        }
        if ((int)cnt % 7 == 1) {
            bec1 = true;
        }

        if (drawhp) {
            // desen si transformari heal
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(xhp - length / 2, yhp - length / 8);
            modelMatrix *= transform2D::Scale(1, 0.25f);
            modelMatrix *= transform2D::Translate(-(xhp - length / 2), -(yhp - length / 8));
            modelMatrix *= transform2D::Translate(xhp - length / 2, yhp - length / 8);
            RenderMesh2D(meshes["whitesq"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(xhp - length / 8, yhp - length / 2);
            modelMatrix *= transform2D::Scale(0.25f, 1);
            modelMatrix *= transform2D::Translate(-(xhp - length / 8), -(yhp - length / 2));
            modelMatrix *= transform2D::Translate(xhp - length / 8, yhp - length / 2);
            RenderMesh2D(meshes["whitesq"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(xhp, yhp);
            modelMatrix *= transform2D::Scale(sxhp, syhp);
            modelMatrix *= transform2D::Translate(-xhp, -yhp);
            modelMatrix *= transform2D::Translate(xhp, yhp);
            RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);
        } 

        // coliziune personaj heal
        if (drawhp) {
            distancehp = sqrt(pow(x_head - xhp, 2) + pow(y_head - yhp, 2));
            if (distancehp <= radius + radius * sxhp) {
                if (sxbar + 0.2f > sxbar_default) {
                    sxbar = sxbar_default;
                    drawhp = false;
                }
                else {
                    sxbar += 0.2f;
                    drawhp = false;
                }
            }
        }

        // desen si transformari hp bar
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(xbar, ybar);
        modelMatrix *= transform2D::Scale(sxbar, sybar);
        modelMatrix *= transform2D::Translate(-xbar, -ybar);
        modelMatrix *= transform2D::Translate(xbar, ybar);
        RenderMesh2D(meshes["redsq"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(xbar, ybar);
        modelMatrix *= transform2D::Scale(sxbar_nofill, sybar_nofill);
        modelMatrix *= transform2D::Translate(-xbar, -ybar);
        modelMatrix *= transform2D::Translate(xbar, ybar);
        RenderMesh2D(meshes["redsq_nofill"], shaders["VertexColor"], modelMatrix);

        // desen si transformari personaj
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x_head, y_head);
        RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x_head, y_head);
        modelMatrix *= transform2D::Rotate(angle);
        modelMatrix *= transform2D::Translate(-x_head, -y_head);
        modelMatrix *= transform2D::Translate(x_eye1, y_eye1);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x_head, y_head);
        modelMatrix *= transform2D::Rotate(angle);
        modelMatrix *= transform2D::Translate(-x_head, -y_head);
        modelMatrix *= transform2D::Translate(x_eye2, y_eye2);
        RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

        // pentru aparitia unui inamic la interval de 5 secunde
        if ((int)cnt % 2 == 0 && bec) {
            bec = false;
            coord entry;
            entry.xenemy = rand() % (int)(length * mapscalex - enemysize);
            entry.yenemy = rand() % (int)(length * mapscaley - enemysize);
            entry.enemystep = 0;
            entry.transxenemy = entry.xenemy;
            entry.transyenemy = entry.yenemy;
            entry.speed = ((float)rand() / (float)(RAND_MAX)) * 0.5f;
            enemies.push_back(entry);
        }
        if ((int)cnt % 2 == 1) {
            bec = true;
        }

        // desen si transformari inamici
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].enemystep += enemies[i].speed * deltaTimeSeconds;
            enemies[i].transxenemy = enemies[i].xenemy + (x_head - enemies[i].xenemy) * enemies[i].enemystep;
            enemies[i].transyenemy = enemies[i].yenemy + (y_head - enemies[i].yenemy) * enemies[i].enemystep;

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(enemies[i].transxenemy + 75, enemies[i].transyenemy + 75);
            RenderMesh2D(meshes["blacksq"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(enemies[i].transxenemy - 15, enemies[i].transyenemy + 75);
            RenderMesh2D(meshes["blacksq"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(enemies[i].transxenemy, enemies[i].transyenemy);
            RenderMesh2D(meshes["redsq"], shaders["VertexColor"], modelMatrix);
        }

        //coliziune inamic personaj
        for (int i = 0; i < enemies.size(); i++) {
            dist = sqrt(pow(enemies[i].transxenemy + enemysize / 2 - x_head, 2) + pow(enemies[i].transyenemy + enemysize / 2 - y_head, 2));
            if (dist <= enemysize * sqrt(2) / 2 + radius) {
                sxbar -= 0.4f;
                enemies.erase(enemies.begin() + i);
            }
        }

        // dupa o secunda jucatorul poate trage din nou
        if (currenttime = glfwGetTime() - time >= firerate) {
            shootagain = true;
        }

        // desen si tragere proiectil
        if (weapon && shootagain) {
            modelMatrix = glm::mat3(1);
            // proiectilul merge pe o distanta maxima
            if (currdistance > maxdistance) {
                currdistance = 0;
                weapon = false;
                shootagain = false;
                step = 0;
            } else if (transxproj <= 0 || transxproj >= length * mapscalex || transyproj <= 0 || transyproj >= length * mapscaley) {
                // coliziune proiectil harta
                weapon = false;
                shootagain = false;
                step = 0;
                currdistance = 0;
            }
            else {
                currdistance += sqrt(pow(from_x - transxproj, 2) + pow(from_y - transyproj, 2));
                step += 3 * deltaTimeSeconds;
                transxproj = from_x + (x_tinta - from_x) * step;
                transyproj = from_y - (y_tinta - from_y) * step;
                modelMatrix *= transform2D::Translate(transxproj, transyproj);
                modelMatrix *= transform2D::Rotate(angle1);
                RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
            }
        }

        // coliziune inamic proiectil
        for (int i = 0; i < enemies.size(); i++) {
            distance = sqrt(pow(enemies[i].transxenemy - transxproj, 2) + pow(enemies[i].transyenemy - transyproj, 2));
            if (distance <= enemysize * sqrt(2) / 2 + length * sqrt(2) / 2) {
                scor += 1;
                std::cout << "Current score: " << scor << "\n";
                enemies.erase(enemies.begin() + i);
                weapon = false;
                shootagain = false;
                step = 0;
                currdistance = 0;
            }
        }

        // desen 4 obstacole si transformari pentru fiecare
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(sx4, sy4);
        modelMatrix *= transform2D::Translate(ox4, oy4);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(sx3, sy3);
        modelMatrix *= transform2D::Translate(ox3, oy3);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(sx2, sy2);
        modelMatrix *= transform2D::Translate(ox2, oy2);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(sx1, sy1);
        modelMatrix *= transform2D::Translate(ox1, oy1);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

        // desen si transformari harta
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Scale(mapscalex, mapscaley);
        RenderMesh2D(meshes["whitesq"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // + coliziuni personaj harta
    if (window->KeyHold(GLFW_KEY_W)) {
        if (y_head + radius + radius_eye < length * mapscaley) {
            y_head += deltaTime * speed;
            y_eye1 += deltaTime * speed;
            y_eye2 += deltaTime * speed;
            ycamera += deltaTime * speed;
            ybar += deltaTime * speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        if (x_head - radius - radius_eye > 0) {
            x_head -= deltaTime * speed;
            x_eye1 -= deltaTime * speed;
            x_eye2 -= deltaTime * speed;
            xcamera -= deltaTime * speed;
            xbar -= deltaTime * speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (y_head - radius - radius_eye > 0) {
            y_head -= deltaTime * speed;
            y_eye1 -= deltaTime * speed;
            y_eye2 -= deltaTime * speed;
            ycamera -= deltaTime * speed;
            ybar -= deltaTime * speed;
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        if (x_head + radius + radius_eye < length * mapscalex) {
            x_head += deltaTime * speed;
            x_eye1 += deltaTime * speed;
            x_eye2 += deltaTime * speed;
            xcamera += deltaTime * speed;
            xbar += deltaTime * speed;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        play = 1;
        scor = 0;
        sxbar = sxbar_default;
        ok = 0;
        x_head = length * mapscalex / 2, y_head = length * mapscaley / 2;
        x_eye1 = x_head - 20, y_eye1 = y_head - 40;
        x_eye2 = x_head + 20, y_eye2 = y_head - 40;
        xbar = length * mapscalex - 340;
        ybar = length * mapscaley - 400;
        xcamera = 160, ycamera = 340;
        firerate = 1;
        drawhp = true;
        drawpower = true;
        enemies.clear();
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    a = mouseX - x_head + xcamera;
    b = ycamera + (float)resolution.y - mouseY - y_head;
    angle = glm::atan(b, a) + pi / 2;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        if (shootagain) {
            weapon = true;
            transxproj = x_head;
            transyproj = y_head;
            a = mouseX - x_head + xcamera;
            b = ycamera + (float)resolution.y - mouseY - y_head;
            angle1 = glm::atan(b, a) + pi / 2;
            x_tinta = xcamera + mouseX;
            y_tinta = ycamera + mouseY;
            from_x = x_head;
            from_y = y_head;
            time = glfwGetTime();
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
