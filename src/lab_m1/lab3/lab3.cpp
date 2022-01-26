#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2DD.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;
     
    // Initialize tx and ty (the translation steps)
    translate = true;
    translateX = 150;
    translateY = 250;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scalate = true;

    // Initialize angularStep
    angularStep = 0;
    angularStep_aux = 0;
    rotation_number = 0;
    sqX = 0; sqY = 0;
    // Initialize rotation_number

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0.635f, 0.545f, 0.545f), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0.043f, 0.756f, 0.388f), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0.984f, 0.772f, 0.054f), true);
    AddMeshToList(square3);

    earthRadius = 250 + cx;
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


/*************************************************************************
* 
Commented initial Update function with 3 squares with different animations

***************************************************************************/

//void Lab3::Update(float deltaTimeSeconds)
//{
//    // TODO(student): Update steps for translation, rotation and scale,
//    // in order to create animations. Use the class variables in the
//    // class header, and if you need more of them to complete the task,
//    // add them over there!
//
//    if (translate) {
//        translateY += 35 * deltaTimeSeconds;
//        if (translateY > 350) {
//            translate = 0;
//        }
//    }
//    else {
//        translateY -= 35 * deltaTimeSeconds;
//        if (translateY < 150) {
//            translate = 1;
//        }
//    }
//    
//    modelMatrix = glm::mat3(1);
//    modelMatrix *= transform2D::Translate(translateX, translateY);
//
//    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
//  
//    // TODO(student): Create animations by multiplying the current
//    // transform matrix with the matrices you just implemented.
//    // Remember, the last matrix in the chain will take effect first!
//
//    modelMatrix = glm::mat3(1);
//    modelMatrix *= transform2D::Translate(400, 250);
//
//    if (rotation_number < 250) {
//        rotation_number++;
//        angularStep -= 10 * deltaTimeSeconds;
//        if (rotation_number == 249) {
//            angularStep = 0;
//        }
//    }
//    else {
//        rotation_number++;
//        angularStep += 10 * deltaTimeSeconds;
//        if (rotation_number == 500) {
//            rotation_number = 0;
//            angularStep = 0;
//        }
//    }
//    
//    modelMatrix *= transform2D::Translate(cx, cy);
//    modelMatrix *= transform2D::Rotate(angularStep);
//    modelMatrix *= transform2D::Translate(-cx, -cy);
//    // TODO(student): Create animations by multiplying the current
//    // transform matrix with the matrices you just implemented
//    // Remember, the last matrix in the chain will take effect first!
//
//    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
//
//    modelMatrix = glm::mat3(1);
//    modelMatrix *= transform2D::Translate(650, 250);
//
//    if (scalate) {
//        scaleX += 1 * deltaTimeSeconds;
//        scaleY += 1 * deltaTimeSeconds;
//        if (scaleX >= 2) {
//            scalate = false;
//        }
//    }
//    else
//    {
//        scaleX -= 1 * deltaTimeSeconds;
//        scaleY -= 1 * deltaTimeSeconds;
//        if (scaleX <= 0.6f) {
//            scalate = true;
//        }
//    }
//
//    modelMatrix *= transform2D::Translate(cx, cy);
//    modelMatrix *= transform2D::Scale(scaleX, scaleY);
//    modelMatrix *= transform2D::Translate(-cx, -cy);
//    
//    // TODO(student): Create animations by multiplying the current
//    // transform matrix with the matrices you just implemented
//    // Remember, the last matrix in the chain will take effect first!
//
//    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
//} 


void Lab3::Update(float deltaTimeSeconds)
{
   
    // First square rotation 

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 300);

    angularStep -= 0.8f * deltaTimeSeconds;

    modelMatrix *= transform2D::Translate(earthRadius, cy);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(-earthRadius, -cy);
 
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
 

    // Second square rotation around the first square 

    sqX = cos(angularStep) * deltaTimeSeconds;
    sqY = sin(angularStep) * deltaTimeSeconds;

    sqX = sqX + 100 / 2;
    sqY = sqY + 100 / 2;

    // Scaling the second square for a better view 
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(0.6f, 0.6f);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    modelMatrix *= transform2D::Translate(180, 0);

    // Second square rotation
    angularStep_aux += 5 * deltaTimeSeconds;
    modelMatrix *= transform2D::Translate(sqX - 180, sqY);
    modelMatrix *= transform2D::Rotate(angularStep_aux);
    modelMatrix *= transform2D::Translate(-sqX + 180, -sqY); 

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    // Third square translation and scaling
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 300);

    if (scalate) {
        scaleX += 1 * deltaTimeSeconds;
        scaleY += 1 * deltaTimeSeconds;
        if (scaleX >= 1.05f) {
            scalate = false;
        }
    }
    else
    {
        scaleX -= 1 * deltaTimeSeconds;
        scaleY -= 1 * deltaTimeSeconds;
        if (scaleX <= 0.6f) {
            scalate = true;
        }
    }

    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    //GC
}

void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
