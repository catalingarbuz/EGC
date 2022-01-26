#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h> 

#include "lab_m1/Tema2/transform3D.h"
#include "components/transform.h"

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
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* enemyShader = new Shader("enemyShader");
        enemyShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShaderEnemy.glsl"), GL_VERTEX_SHADER);
        enemyShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderEnemy.glsl"), GL_FRAGMENT_SHADER);
        enemyShader->CreateAndLink();
        shaders[enemyShader->GetName()] = enemyShader;
    }

    firstPersCam = false;
    projectionMode = false;
    enemyAnim = false;

    
    left = -8.0f;
    right = 8.0f;
    bottom = -4.0f;
    top = 4.0f;
    zNear = 0.01f;
    zFar = 100.0f;
    fov = 60.0f;

    playerX = 0;
    playerY = 0.92f;
    playerZ = 0;

    playerAngle = 0;

    projectileAngle = 0;
    fireRate = 0;
    projectileSpeed = 0;

    // UI 
    lifeBar = 1;
    timeBar = 1;

    ifstream File1;
    int lastMazeNr = 0;

    // Asigur faptul ca la doua rulari diferite avem labirinturi diferite
    File1.open("mazes\\lastMazeNr.txt");
    File1 >> lastMazeNr;
    File1.close();
    cout << "last mazeMatrix nr " << lastMazeNr << endl;
    int number = rand() % 7 + 1;
    while (number == lastMazeNr) {
        number = rand() % 7 + 1;
    }

    ofstream myfile("mazes\\lastMazeNr.txt");
    myfile << number;
    myfile.close();
 
    cout << "actual mazeMatrix number " << number << endl;


    string matrixPath = "mazes\\" + to_string(number) + ".txt";
    ifstream File;
    File.open(matrixPath);
    while (!File.eof())
    {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                File >> mazeMatrix[i][j];
    }
    File.close();

    // Pozitionarea initiala a jucatorului
    playerPosX = rand() % 7;
    playerPosY = rand() % 7;

    while (mazeMatrix[playerPosX][playerPosY] == 1 || mazeMatrix[playerPosX][playerPosY] == 2)
    {
        playerPosX = rand() % 7;
        playerPosY = rand() % 7;
    }

    gameOver = 0;

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    // Setarea camerei la pozitia initiala
    camera = new implemented::Cam();
    camera->Set(glm::vec3(playerPosX + 0.5f, 1.5f, playerPosY + 2.5f), glm::vec3(playerPosX + 0.5f, 1, playerPosY), glm::vec3(0, 1, 0));

    EnemyX = 0;
    EnemyY = 0;
    EnemyZ = 0;
    enemyStage = 0;
    mode = 0;
    enemyDyingTime = 0.4f;

    moveFeet = 0;
    feetAngle = 20;
    steps = 1;
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


void Tema2::Update(float deltaTimeSeconds)
{
    glm::mat4 modelMatrixInit = glm::mat4(1);
    modelMatrixInit = glm::translate(modelMatrixInit, glm::vec3(0, -1.5f, 0));
    modelMatrixInit = glm::translate(modelMatrixInit, camera->GetTargetPosition());
    modelMatrixInit *= transform3D::RotateOY(playerAngle);
    if (!firstPersCam)
    {
        glm::vec3 body_color = glm::vec3(0.776f, 0.486f, 0.184f);
        glm::vec3 skin_color = glm::vec3(0.976f, 0.835f, 0.705f);
        glm::vec3 feet_color = glm::vec3(0.231f, 0.298f, 0.407f);
        // Player rendering
        {
            // body
            glm::mat4 modelMatrix = modelMatrixInit;
            modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
            modelMatrix *= transform3D::Scale(0.3f, 0.45f, 0.2f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, body_color);

            // Head
            glm::mat4 modelMatrixH = modelMatrix;
            modelMatrixH *= transform3D::Translate(0.0f, 0.65f, 0.0f);
            modelMatrixH *= transform3D::Scale(0.65f, 0.4f, 0.7f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrixH, skin_color);

            // hands
            modelMatrix *= transform3D::Translate(-0.65f, 0.08f, 0.0f);
            modelMatrix *= transform3D::Scale(0.25f, 0.82f, 0.5f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, skin_color);

            modelMatrix *= transform3D::Translate(5.2f, 0.0f, 0.0f);
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, skin_color);

            // feet
            modelMatrix *= transform3D::Translate(-3.5f, -1.0f, 0.0f);
            modelMatrix *= transform3D::Scale(1.7f, 1.3f, 1.5f);
            
            //A try to animate player's feet
            if (moveFeet && steps % 10 == 0) {
                steps = 1;
                feetAngle *= -1;
                float angle = 0;
                if (feetAngle < 0) {
                    angle = -1.14f;
                }
                else {
                    angle = 0.57f;
                }
                modelMatrix *= transform3D::RotateOX(angle);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, feet_color);
                if (feetAngle < 0)
                    angle = 0.57f;
                else
                    angle = -1.14f;
                modelMatrix *= transform3D::RotateOX(angle);
                modelMatrix *= transform3D::Translate(1.15f, 0.0f, 0.0f);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, feet_color);
            }

            else {
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, feet_color);

                modelMatrix *= transform3D::Translate(1.15f, 0.0f, 0.0f);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, feet_color);
            }
            moveFeet = 0;
        }
    }

        // Plane 
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(5, 0.01f, 5);
        modelMatrix *= transform3D::Scale(0.2f, 0.2f, 0.2f);
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0.309f, 0.525f, 0.890f));
    }

    // Bara de timp
    {
        glm::mat4 modelMatrixTime = modelMatrixInit;
        modelMatrixTime *= transform3D::Translate(0, 1.55, 0);
        modelMatrixTime *= transform3D::Scale(0.45f, 0.04f, 0.01f);
        modelMatrixTime *= transform3D::Scale(timeBar, 1, 1);
        float timeSpeed = 0.02f;
        if (timeBar >= 0)
            timeBar -= timeSpeed * deltaTimeSeconds;
        else if (!gameOver)
        {
            gameOver = 1;
            std::cout << "TIME UP! GAME OVER!" << std::endl;
            exit(0);
        }
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrixTime, glm::vec3(0, 1, 0));

    }

        //Bara de viata
    {
        glm::mat4 modelMatrixLife = modelMatrixInit;
        modelMatrixLife *= transform3D::Translate(0, 1.5, 0);
        modelMatrixLife *= transform3D::Scale(0.45f, 0.04f, 0.01f);
        modelMatrixLife *= transform3D::Scale(lifeBar, 1, 1);
        if (lifeBar < 0 && !gameOver)
          {
             std::cout << "GAME OVER!" << std::endl;
             gameOver = 1;
             exit(0);
          }
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrixLife, glm::vec3(1, 0, 0));
    }   

    //Maze
    {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (mazeMatrix[i][j] == 1)
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5f, 1, j + 0.5f));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2, 1));
                    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.858, 0.635, 0.058));
                }
    }


    // Miscarea inamicilor
   {
       float enemySpeed = 3.5f;
       if (enemyStage == 0)
       {
           EnemyX += enemySpeed * deltaTimeSeconds;
           if (EnemyX >= .5f)
               enemyStage = 1;
       } else if (enemyStage == 1) {
           EnemyZ += enemySpeed * deltaTimeSeconds;
           if (EnemyZ >= .5f)
               enemyStage = 2;
       } else if (enemyStage == 2) {
           EnemyX -= enemySpeed * deltaTimeSeconds;
           if (EnemyX <= -0.5f)
               enemyStage = 3;
       } else if (enemyStage == 3) {
           EnemyZ -= enemySpeed * deltaTimeSeconds;
           if (EnemyZ <= -0.5f)
               enemyStage = 0;
       }
       
       // Parcurg toata matricea si randez inamicii in scena
       for (int i = 0; i < 10; i++)
           for (int j = 0; j < 10; j++)
               if (mazeMatrix[i][j] == 2)
               {
                   glm::mat4 modelMatrix = glm::mat4(1);
                   modelMatrix += transform3D::Translate(EnemyX, EnemyY, EnemyZ);
                   modelMatrix *= transform3D::Translate(i + 0.5f, 0.5, j + 0.5f);
                   modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
                   if (enemyAnim)
                   {
                       RenderSimpleMesh(meshes["sphere"], shaders["enemyShader"], modelMatrix, glm::vec3(0.898, 0.058, 0.043));
                      
                       // Enemy dying functionality
                    /*   enemyDyingTime -= deltaTimeSeconds;
                       if (enemyDyingTime <= 0) {
                           mazeMatrix[i][j] = 0;
                       }*/
                   }
                   else
                   {
                       RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.898, 0.058, 0.043));
                   }
               
               }

    
   }

   //Projectile
   if (fireRate > 0 && firstPersCam)
   {
       fireRate -= deltaTimeSeconds;
       projectileSpeed += 3.15f * deltaTimeSeconds;
       glm::mat4 modelMatrix = glm::mat4(1);
       modelMatrix = glm::translate(modelMatrix, projectileSpeed * projectileMove);
       modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.4f, 0));
       modelMatrix = glm::translate(modelMatrix, projectilePosition);
       modelMatrix *= transform3D::RotateOY(projectileAngle);
       modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17f));
       RenderSimpleMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
   }
}

void Tema2::FrameEnd()
{
  //  DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(deltaTime * cameraSpeed);
            moveFeet = 1;
            steps++;
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-deltaTime * cameraSpeed);
            moveFeet = 1;
            steps++;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-deltaTime * cameraSpeed);
            moveFeet = 1;
            steps++;
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(deltaTime * cameraSpeed);
            moveFeet = 1;
            steps++;
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
    if (window->KeyHold(GLFW_KEY_Z) && !projectionMode) {
        fov += deltaTime * 10.0f;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    }
    if (window->KeyHold(GLFW_KEY_X) && !projectionMode) {
        fov -= deltaTime * 10.0f;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
    }
   
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        if (firstPersCam == false)
        {
            camera->MoveForward(0.5f);
            camera->TranslateUpward(-0.25f);
            firstPersCam = true;
        }
        else
        {
            camera->MoveForward(-0.5f);
            camera->TranslateUpward(0.25f);
            firstPersCam = false;
        }

    }

    if (key == GLFW_KEY_P)
    {
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);
        projectionMode = false;
    }

    if (key == GLFW_KEY_F)
    {
        if (mode == 0) {
            enemyAnim = true;
            mode = 1;
            enemyDyingTime = 0.4f;
        }
        else if (mode == 1)
        {
            enemyAnim = false;
            mode = 0;
        }
    }

    if (key == GLFW_KEY_1)
    {
        if (lifeBar >= 0)
            lifeBar -= 0.1f;
    }

}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    GLint obj_col = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(obj_col, 1, glm::value_ptr(color));

    GLfloat timeLocation = glGetUniformLocation(shader->GetProgramID(), "Time");
    glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
        float sensivityOX = 0.005f;
        float sensivityOY = 0.005f;
      
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {
       
            if (firstPersCam)
            {
                camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
            }
            else
            {
                camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
                camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
            }
            playerAngle -= sensivityOY * deltaX;
        }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1 && fireRate <= 0)
    {
        projectilePosition = camera->GetTargetPosition();
        fireRate = 0.8f;
        projectileSpeed = 0;
        projectileAngle = playerAngle;
        projectileMove = camera->forward;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
