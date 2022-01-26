#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "Enemy.h"

using namespace std;
using namespace m1;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    resX = (float)resolution.x;
    resY = (float)resolution.y;
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;     
    logicSpace.y = 0;      
    logicSpace.width = 4;   
    logicSpace.height = 4;  

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;
    playerRadius = 0.4f;
  
    cx = corner.x + 0.4f / 2;
    cy = corner.y + 0.4f / 2;
     

    // Initialize angularStep
    angularStep = 0;


    // Initialize player parameters
    playerSpeed = 2.0f;
    projectileState = 0;

   
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* score_bar = object2D::CreateSquare("score_bar", corner, squareSide, glm::vec3(0.98f, 0.933f, 0.039f), true);
    AddMeshToList(score_bar);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0.403f, 0.4f, 0.439f), true);
    AddMeshToList(square2);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(0), true);
    AddMeshToList(square4);

    Mesh* square3 = object2D::CreateSquare("square3", corner, 10, glm::vec3(0.925f, 0.701f, 0.411f), true);
    AddMeshToList(square3);

    Mesh* health_box = object2D::CreateSquare("health_box", corner, squareSide, glm::vec3(0), false);
    AddMeshToList(health_box);

    Mesh* minimap_box = object2D::CreateSquare("minimap_box", corner, 1, glm::vec3(0), true);
    AddMeshToList(minimap_box);


    Mesh* triangle1 = object2D::CreateTriangle("triangle1", corner, squareSide, glm::vec3(0), true);
    AddMeshToList(triangle1);

    Mesh* circle1 = object2D::CreateCircle("circle1", 360, playerRadius, glm::vec3(0.96f, 0.921f, 0.141f), true);
    AddMeshToList(circle1);
    Mesh* circleMiniMap = object2D::CreateCircle("circleMiniMap", 360, playerRadius, glm::vec3(0.886f, 0.623f, 0.011f), true);
    AddMeshToList(circleMiniMap);

    Mesh* circleContur = object2D::CreateCircle("circleC", 1000, playerRadius, glm::vec3(0.525f, 0.411f, 0.356f), true);
    AddMeshToList(circleContur);

    Mesh* circle2 = object2D::CreateCircle("circle2", 360, playerRadius, glm::vec3(0.329f, 0.113f, 0.011f), true);
    AddMeshToList(circle2);

    nrOfenemies = 0;
    maxNrOfEnemies = 25;
    lastEnemySpawn = 0.f;

    projectileSpeed = 7;
    projectileMaxDistance = 30;
    Projectile projectile1(0, 0, 1, projectileSpeed, projectileMaxDistance);
    projectile = projectile1;
    lastShootms = 0;
    fireRate = 0.25f;
    projNr = 0;
    projIdx = 0;

    globTime = 0;

    playerBoxColision = false;
    nrOfObstacles = 0;
    initObstacles();

    spawnTime = 3.f;
    // INTERFACE
    health_level = 1.1f;
    score_level = 0.f;
    score = 0;
    gameOver = false;
    printNewGameMessage = 0;

}

void Tema1::initObstacles() {
    obstacles[nrOfObstacles++] = Obstacle(0, 0, 0.4f, 3);
    obstacles[nrOfObstacles++] = Obstacle(-4, 0, 3, 0.4f);
    obstacles[nrOfObstacles++] = Obstacle(-3.2f, 2.8f, 4, 0.4f);
    obstacles[nrOfObstacles++] = Obstacle(3.2f, 4, 1, 1);
    obstacles[nrOfObstacles++] = Obstacle(6.1f, 4.52f, 1, 1.5f);
    obstacles[nrOfObstacles++] = Obstacle(6.4f, 0, 3, 0.4f);
    obstacles[nrOfObstacles++] = Obstacle(10, 1, 0.4f, 3.4f);
    obstacles[nrOfObstacles++] = Obstacle(4.8f, -1, 0.6f, 2.7f);
    obstacles[nrOfObstacles++] = Obstacle(-4, 6, 1, 1);
    obstacles[nrOfObstacles++] = Obstacle(-1, 3, 0.5f, 2);
    obstacles[nrOfObstacles++] = Obstacle(9.5f, 5.50f, 1.5f, 1.5f);
    obstacles[nrOfObstacles++] = Obstacle(2, 0, 2, 0.4f);
    obstacles[nrOfObstacles++] = Obstacle(6.5f, 2.5f, 0.7f, 0.7f);
}

bool Tema1::CheckCollision(Projectile p, float posX, float posY, float width, float height) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = p.posX + 0.15f > posX &&
         posX + width > p.posX;
    // collision y-axis?
    bool collisionY = p.posY + 0.15f > posY &&
        posY + height > p.posY;
    // collision only if on both axes
    if (!(collisionX && collisionY)) {
        float radius = playerRadius * 0.65f;
        if (p.posX > 13 || p.posX < -4) {
            return true;
        }
        else if (p.posY > 7 || p.posY < -1) {
            return true;
        }
    }

    return collisionX && collisionY;



}

bool Tema1::CheckCollision(float objX, float objY, float objW, float objH) // AABB - Circle collision
{
    // get center point circle first
    glm::vec2 playerPos;
    playerPos.x = logicSpace.x + logicSpace.width / 2;
    playerPos.y = logicSpace.y + logicSpace.height / 2;
    glm::vec2 center(playerPos);

    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(objW / 2.0f, objH / 2.0f);
    glm::vec2 aabb_center(
        objX + aabb_half_extents.x,
        objY + aabb_half_extents.y
    );

    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return glm::length(difference) < (playerRadius * 0.65f);
}


// Function to check obstacle/edge of the map colision 
bool Tema1::checkPlayerMapCollisions() 
  {
    bool collision = false;
    for (int i = 0; i < nrOfObstacles; i++) {
        if (CheckCollision(obstacles[i].posX, obstacles[i].posY,
            obstacles[i].width, obstacles[i].height)) {
            collision = true;
            break;
        }
    }
    if (!collision) {
        float radius = playerRadius * 0.65f;
        if (logicSpace.x + logicSpace.width / 2 + radius > 13 || logicSpace.x + logicSpace.width / 2 - radius < -4) {
            collision = true;
        } 
        else if (logicSpace.y + logicSpace.height / 2 + radius > 7 || logicSpace.y + logicSpace.height / 2 - radius < -1) {
            collision = true;
        }
    }
    return collision;
  }



bool Tema1::checkProjectilMapCollisions(Projectile p)
{
    bool collision = false;
    for (int i = 0; i < nrOfObstacles; i++) {
        if (CheckCollision(obstacles[i].posX, obstacles[i].posY,
            obstacles[i].width, obstacles[i].height)) {
            collision = true;
            break;
        }
    }
    if (!collision) {
        float radius = playerRadius * 0.65f;
        if (logicSpace.x + logicSpace.width / 2 + radius > 13 || logicSpace.x + logicSpace.width / 2 - radius < -4) {
            collision = true;
        }
        else if (logicSpace.y + logicSpace.height / 2 + radius > 7 || logicSpace.y + logicSpace.height / 2 - radius < -1) {
            collision = true;
        }
    }
    return collision;
}

void Tema1::checkEnemyPlayerCollisions() {
    //Iterate through alive enemies
    for (int i = 0; i < nrOfenemies; i++) {
        if (!enemies[i].dead) {
            if (CheckCollision(enemies[i].posX, enemies[i].posY,
                enemies[i].sizeX, enemies[i].sizeY)) {
                enemies[i].dead = true;
                if (health_level > 0) {
                    health_level -= 0.1f;
                    if (health_level <= 0) {
                        gameOver = true;
                        printNewGameMessage = true;
                        cout << "YOU DIED!" << endl << "Your score: " << score << endl;
                    }
                }
            }
        }
    }
}

glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTime)
{

    // Player Model Matrix and rendering
    glm::mat3 auxMatrix;
    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + logicSpace.width / 2,
        logicSpace.y + logicSpace.height / 2);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Scale(0.62f, 0.62f);
    RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix);

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Translate(-0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.3f, 0.3f);
    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;
    modelMatrix *= transform2D::Translate(-0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.335f, 0.335f);
    RenderMesh2D(meshes["circleC"], shaders["VertexColor"], modelMatrix);


    modelMatrix = auxMatrix;
    modelMatrix *= transform2D::Translate(0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.3f, 0.3f);
    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;
    modelMatrix *= transform2D::Translate(0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.33f, 0.33f);
    RenderMesh2D(meshes["circleC"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + logicSpace.width / 2,
        logicSpace.y + logicSpace.height / 2);
    modelMatrix *= transform2D::Scale(0.65f, 0.65f);
    RenderMesh2D(meshes["circleC"], shaders["VertexColor"], modelMatrix);


    // Rendering USER INTERFACE
    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + logicSpace.width - 0.15f,
        logicSpace.y + logicSpace.height / 1.12f);
    modelMatrix *= transform2D::Scale(1.1f, 0.27f);
    RenderMesh2D(meshes["health_box"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + logicSpace.width - 0.15f,
        logicSpace.y + logicSpace.height / 1.12f);
    modelMatrix *= transform2D::Scale(health_level, 0.27f);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x  - 1,
        logicSpace.y + logicSpace.height / 1.12f);
    modelMatrix *= transform2D::Scale(1.1f, 0.27f);
    RenderMesh2D(meshes["health_box"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x - 1,
        logicSpace.y + logicSpace.height / 1.12f);
    modelMatrix *= transform2D::Scale(score_level, 0.27f);
    RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);

    //Projectile rendering
    if (projectileState) {
        for (int i = 0; i < projNr; i++) {
            bool projCollisionCheck = false;
            projVec[i].moveProjectile(deltaTime);
            if (projVec[i].distance >= projVec[i].maxDistance) {
                projVec[i].destroyed = 1;
            }
            // Check for projectile-obstacle/map collisions
            {
                for (int j = 0; j < nrOfObstacles; j++) {
                    if (CheckCollision(projVec[i], obstacles[j].posX, obstacles[j].posY,
                        obstacles[j].width, obstacles[j].height)) {
                        projCollisionCheck = true;
                        break;
                    }
                }
                // If collision detected destroy projectile
                if (projCollisionCheck) {
                    projVec[i].destroyed = 1;
                }
                // Enemy collision check
                if (!projCollisionCheck && !projVec[i].destroyed) {

                    for (int j = 0; j < nrOfenemies; j++) {
                        if (CheckCollision(projVec[i], enemies[j].posX, enemies[j].posY,
                            enemies[j].sizeX, enemies[j].sizeY)) {
                            projCollisionCheck = true;
                            enemies[j].dead = true;
                            enemies[j].posX = -5;
                            enemies[j].posY = -2;
                            // If collision detected destroy projectile & increase SCORE
                            projVec[i].destroyed = 1;
                            score += 25;
                            // If score < maxScore increase user interface score_bar
                            if (score_level < 1.1f) {
                                score_level += 0.00275f;
                            }
                            // Print score and decrease spawn time to make the gameplay harder
                            if (score % 100 == 0) {
                                cout << "YOUR SCORE IS: " << score << " KEEP FIGHTING !" << endl;
                            }
                            if (score % 500 == 0 && spawnTime > 0) {
                                spawnTime -= 0.25f;
                              
                            }

                            break;
                            
                        }
                    }
                }
            }
            
            if (!projVec[i].destroyed && projVec[i].distance < projVec[i].maxDistance ) {
                DrawProjectile(visMatrix, projVec[i]);
            }
        }   
    }

    //Check for enemy - player Collision
    checkEnemyPlayerCollisions();

    
    // Enemy rendering
    for (int i = 0; i < nrOfenemies; i++) {
            if (!enemies[i].dead) {
                enemies[i].moveEnemy(logicSpace.x + logicSpace.width / 2, logicSpace.y + logicSpace.height / 2, deltaTime);
                DrawEnemy(enemies[i], visMatrix);
            }
    }

    DrawMap(modelMatrix, visMatrix);

   
}


void Tema1::DrawMiniMapScene(glm::mat3 visMatrix) {
    // Player Model Matrix and rendering
    glm::mat3 auxMatrix;
    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + logicSpace.width / 2,
        logicSpace.y + logicSpace.height / 2);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Scale(0.62f, 0.62f);
    RenderMesh2D(meshes["circleMiniMap"], shaders["VertexColor"], modelMatrix);

    auxMatrix = modelMatrix;
    modelMatrix *= transform2D::Translate(-0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.3f, 0.3f);
    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;
    modelMatrix *= transform2D::Translate(-0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.335f, 0.335f);
    RenderMesh2D(meshes["circleC"], shaders["VertexColor"], modelMatrix);


    modelMatrix = auxMatrix;
    modelMatrix *= transform2D::Translate(0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.3f, 0.3f);
    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);
    modelMatrix = auxMatrix;
    modelMatrix *= transform2D::Translate(0.19f, -0.4f);
    modelMatrix *= transform2D::Scale(0.33f, 0.33f);
    RenderMesh2D(meshes["circleC"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + logicSpace.width / 2,
        logicSpace.y + logicSpace.height / 2);
    modelMatrix *= transform2D::Scale(0.65f, 0.65f);
    RenderMesh2D(meshes["circleC"], shaders["VertexColor"], modelMatrix);


    // Enemy rendering
        for (int i = 0; i < nrOfenemies; i++) {
            if (!enemies[i].dead) {
                DrawEnemy(enemies[i], visMatrix);
            }
        }

        DrawMap(modelMatrix, visMatrix);

}

void Tema1::DrawEnemy(Enemy en, glm::mat3 visMatrix) {

    glm::mat3 modelMatrix;
    modelMatrix = visMatrix * transform2D::Translate(en.posX, en.posY);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(en.enemyAngle);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    modelMatrix *= transform2D::Scale(0.4f, 0.4f);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);


    glm::mat3 auxM = modelMatrix;
    modelMatrix *= transform2D::Translate(0, 1);
    modelMatrix *= transform2D::Scale(0.5f, 0.3f);
    RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = auxM;
    modelMatrix *= transform2D::Translate(0.5f, 1);
    modelMatrix *= transform2D::Scale(0.5f, 0.3f);
    RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawMap(glm::mat3 modelMatrix, glm::mat3 visMatrix) {
    glm::mat3 auxMMatrix = modelMatrix;
  
    // Render map
    for (int i = 0; i < nrOfObstacles; i++) {
        modelMatrix = visMatrix * transform2D::Translate(obstacles[i].posX, obstacles[i].posY);
        modelMatrix *= transform2D::Scale(obstacles[i].width, obstacles[i].height);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix * transform2D::Translate(-4, -1);
    modelMatrix *= transform2D::Scale(1.7f, 0.8f);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
}

void Tema1::DrawProjectile(glm::mat3 visMatrix, Projectile p) {
    glm::mat3 modelMatrix;
    modelMatrix = visMatrix * transform2D::Translate(p.posX, p.posY);
    modelMatrix *= transform2D::Rotate(p.projectileAngle);
    modelMatrix *= transform2D::Scale(0.17f, 0.14f);
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);

}



void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    if (resolution.x != resX || resolution.y != resY) {
        resX = (float)resolution.x;
        resY = (float)resolution.y;
    }

    //create a logical viewport and put the player in center of it
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);


    if (!gameOver) {
        globTime += deltaTimeSeconds;
        // Enemies spawn 
        if (globTime - lastEnemySpawn > spawnTime) {
            lastEnemySpawn = globTime;
            float randSpeed = (float)(rand() % 5 + 2) / (rand() % 3 + 2);
            float posX = (float(rand() % 17 + -4));
            float posY = (float(rand() % 8 + -1));
            if (nrOfenemies < maxNrOfEnemies) {
                enemies[nrOfenemies++] = Enemy(posX, posY, 0, randSpeed, 0.4f, 0.4f);
            }
            // Spawn new enemies by changing dead ones
            else {
                for (int i = 0; i < nrOfenemies; i++) {
                    if (enemies[i].dead) {
                        enemies[i] = Enemy(posX, posY, 0, randSpeed, 0.4f, 0.4f);
                        break;
                    }
                }
            }
        }

        /////////// MINIMAP FUNCTIONALITY /////////////
        visMatrixMiniMap = glm::mat3(1);
        LogicSpace logic_space = LogicSpace(0,
            0, 9, 6);
        ViewportSpace view_space = ViewportSpace(resX - resX / 4.8f, 20, resX / 9.2f, resY / 4);
        visMatrixMiniMap *= VisualizationTransf2DUnif(logic_space, view_space);

        DrawMiniMapScene(visMatrixMiniMap);

        modelMatrix = visMatrixMiniMap * transform2D::Translate(-4.25f, -1.2f);
        modelMatrix *= transform2D::Scale(17.5f, 8.4f);
        RenderMesh2D(meshes["minimap_box"], shaders["VertexColor"], modelMatrix);  

        ///////////////////////////////////

        // Original Scene
        DrawScene(visMatrix, deltaTimeSeconds);
    }
    if (gameOver && printNewGameMessage) {
            cout << "Press R to start a new game !" << endl;
            printNewGameMessage = false;
      }
}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

    if (window->KeyHold(GLFW_KEY_W)) {
        logicSpace.y += playerSpeed * deltaTime;
        if (checkPlayerMapCollisions()) {
            logicSpace.y -= playerSpeed * deltaTime;
         }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        logicSpace.y -= playerSpeed * deltaTime;
        if (checkPlayerMapCollisions()) {
            logicSpace.y += playerSpeed * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        logicSpace.x -= playerSpeed * deltaTime;
        if (checkPlayerMapCollisions()) {
            logicSpace.x += playerSpeed * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        logicSpace.x += playerSpeed * deltaTime;
        if (checkPlayerMapCollisions()) {
            logicSpace.x -= playerSpeed * deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // New game 
    if (key == GLFW_KEY_R && gameOver) {
        gameOver = 0;
        score = 0;
        spawnTime = 3.f;
        health_level = 1.1f;
        globTime = spawnTime + 1;
        lastEnemySpawn = 0;
        lastShootms = 0;
        nrOfenemies = 0;
        logicSpace.x = 0;
        logicSpace.y = 0;
        cout << "Starting a new game !" << endl;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    angularStep = -atan2(mouseY - resY / 2, mouseX - resX / 2) + M_PI / 2;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{     
 
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // When pressed LKM, check if fire rate is respected, build new projectile 
    if (button == 1 && (globTime - lastShootms > fireRate)) {
        projectile = Projectile(0, 0, 1, projectileSpeed, projectileMaxDistance);
        lastShootms = globTime;
        projectile.projectileAngle = -atan2(mouseY - resY / 2, mouseX - resX / 2);
        // Ready to fire, used in drawScene
        projectileState = 1;
        
        if (projectile.projectileAngle > 0) {
            projectile.posX = logicSpace.x + logicSpace.width / 2 + (cos(projectile.projectileAngle) * 0.28f) + 0.04f;
        }
        else {
            projectile.posX = logicSpace.x + logicSpace.width / 2 + (cos(projectile.projectileAngle) * 0.28f) - 0.04f;
        }
        projectile.posY = logicSpace.y + logicSpace.height / 2 + (sin(projectile.projectileAngle) * 0.28f);

        
        
        // Player's gun functionality
        // Reloading :D 
        projVec[projIdx % 5] = projectile;
        projIdx++;
        projIdx = projIdx % 5;
        projNr++;
        if (projNr >= 5) {
            projNr = 5;
        }
    }
}



void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
