#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Mesh* mesh1 = new Mesh("sphere");
    mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh1->GetMeshID()] = mesh1;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0.8f;
    translateY = 1.4f;
    translateZ = 0.02f;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;
    manOY = 0;
    ankleOX = 0;
    pAngular = 0;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    /*modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);*/

    // Rendering a simple man (BONUS)
    {

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.8f, 1.40f , 0.02f);
        modelMatrix *= transform3D::Scale(0.27f, 0.29f, 0.26f);
        modelMatrix *= transform3D::RotateOY(manOY);
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
        
        //body
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0.8f, 1.4f, translateZ);
        modelMatrix *= transform3D::Scale(0.46f, 0.70f, 0.2f);
        modelMatrix *= transform3D::RotateOY(manOY);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

        //hands
       // modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(-0.6f, 0.08f, 0.03f);
        modelMatrix *= transform3D::Scale(0.25f, 0.8f, 0.5f);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

        modelMatrix *= transform3D::Translate(4.8f, 0.0f, 0.0f);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        
        //feet
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
        modelMatrix *= transform3D::RotateOY(pAngular);
        modelMatrix *= transform3D::Translate(-0.8f, -0.92f, 0.0f);
        modelMatrix *= transform3D::Translate(0.7f, 0.40f, 0.02f);
        modelMatrix *= transform3D::Scale(0.18f, 0.35f, 0.15f);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        modelMatrix *= transform3D::Translate(1.15f, 0.0f, 0.0f);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        modelMatrix *= transform3D::Translate(-1.15f, 0.0f, 0.0f);

        modelMatrix *= transform3D::Translate(0.0f, -0.45f, 0.1f);
        modelMatrix *= transform3D::Scale(1.0f, 0.4f, 1.0f);
        modelMatrix *= transform3D::RotateOX(ankleOX);
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
        modelMatrix *= transform3D::Translate(1.15f, 0.0f, 0.0f);
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
        modelMatrix *= transform3D::Translate(-1.15f, 0.0f, 0.0f);

        modelMatrix *= transform3D::Translate(0.0f, -0.9f, -0.1f);
        modelMatrix *= transform3D::Scale(1.0f, 1.68f, 1.0f);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        modelMatrix *= transform3D::Translate(1.15f, 0.0f, 0.0f);
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic

    //First square translate 
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            translateZ -= 2 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            translateZ += 2 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            translateX -= 2 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            translateX += 2 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_R)) {
            translateY += 2 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_F)) {
            translateY -= 2 * deltaTime;
        }
    }

    //Second square scalate 
    {
        if (window->KeyHold(GLFW_KEY_1)) {
            scaleX += 0.3f * deltaTime;
            scaleY += 0.3f * deltaTime;
            scaleZ += 0.3f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_2)) {
            scaleX -= 0.3f * deltaTime;
            scaleY -= 0.3f * deltaTime;
            scaleZ -= 0.3f * deltaTime;
        }
    }

    //Third square rotations 
    {
        if (window->KeyHold(GLFW_KEY_3)) {
            angularStepOX += 2.5f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_4)) {
            angularStepOX -= 2.5f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_5)) {
            angularStepOY += 2.5f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_6)) {
            angularStepOY -= 2.5f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_7)) {
            angularStepOZ += 2.5f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_8)) {
            angularStepOZ -= 2.5f * deltaTime;
        }
    }

    // Man actions
    {
        if (window->KeyHold(GLFW_KEY_U)) {
            manOY += 2.5f * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_I)) {
            if (ankleOX < 2.0f) {
                ankleOX += 2.5f * deltaTime;
            }
        }

        if (window->KeyHold(GLFW_KEY_O)) {
            if (ankleOX > -0.1f) {
                ankleOX -= 2.5f * deltaTime;
            }
        }

        if (window->KeyHold(GLFW_KEY_P)) {
            manOY += 5.0f * deltaTime;
            pAngular = manOY;
        }


    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
