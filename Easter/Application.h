#pragma once
#include "AppBase.h"
#include "btBulletDynamicsCommon.h"

namespace Easter
{
class MyGUI : public AppBase
{
public:
    MyGUI()
    {
        show_demo_window = true;
        show_another_window = false;
        btBroadphaseInterface *broadphase = new btDbvtBroadphase();
        btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
    }

    ~MyGUI()
    {
        delete dynamicsWorld;
        delete cubeBody;
    }

    void StartUp()
    {
        btCollisionShape *cubeShape = new btBoxShape(btVector3(1, 1, 1));
        btDefaultMotionState *motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0)));
        btRigidBody::btRigidBodyConstructionInfo cubeCI(1.0, motionState, cubeShape);
        cubeBody = new btRigidBody(cubeCI);
        dynamicsWorld->addRigidBody(cubeBody);
    }
        
    void Update()
    {
        // Enable menubar
        DrawMenuBar();

        // Enable dockspace
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if(show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if(ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Render a cube for proper bullet physics use
        dynamicsWorld->stepSimulation(1 / 60.f, 10);
        {
            ImGui::Begin("Bullet Physics");

            btTransform trans;
            cubeBody->getMotionState()->getWorldTransform(trans);

            double mat[16];
            trans.getOpenGLMatrix(mat);

            glBegin(GL_QUADS);
            glColor3f(0.5f, 0.5f, 1.0f);
            glMultMatrixd(mat);

            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glEnd();

            ImGui::End();
        }
    }

    void DrawMenuBar()
    {
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 30));
        if(ImGui::BeginMainMenuBar())
        {
            // File
            if(ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("(demo menu)", NULL, false, false);
                if(ImGui::MenuItem("New")) {}
                if(ImGui::MenuItem("Open", "Ctrl+O")) {}
                if(ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    if(ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Hello");
                        ImGui::MenuItem("Sailor");
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if(ImGui::MenuItem("Save", "Ctrl+S")) {}
                if(ImGui::MenuItem("Save As..")) {}

                ImGui::Separator();

                if(ImGui::BeginMenu("Options"))
                {
                    static bool enabled = true;
                    ImGui::MenuItem("Enabled", "", &enabled);
                    ImGui::BeginChild("child", ImVec2(0, 60), true);
                    for(int i = 0; i < 10; i++)
                        ImGui::Text("Scrolling Text %d", i);
                    ImGui::EndChild();
                    static float f = 0.5f;
                    static int n = 0;
                    ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                    ImGui::InputFloat("Input", &f, 0.1f);
                    ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("Colors"))
                {
                    float sz = ImGui::GetTextLineHeight();
                    for(int i = 0; i < ImGuiCol_COUNT; i++)
                    {
                        const char *name = ImGui::GetStyleColorName((ImGuiCol)i);
                        ImVec2 p = ImGui::GetCursorScreenPos();
                        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                        ImGui::Dummy(ImVec2(sz, sz));
                        ImGui::SameLine();
                        ImGui::MenuItem(name);
                    }
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("Options")) // <-- Append!
                {
                    static bool b = true;
                    ImGui::Checkbox("SomeOption", &b);
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("Disabled", false)) // Disabled
                {
                    IM_ASSERT(0);
                }
                if(ImGui::MenuItem("Checked", NULL, true)) {}
                ImGui::Separator();
                if(ImGui::MenuItem("Quit", "Alt+F4")) {}

                ImGui::EndMenu();
            }

            // Edit
            if(ImGui::BeginMenu("Edit"))
            {
                if(ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
                ImGui::Separator();
                if(ImGui::MenuItem("Cut", "CTRL+X")) {}
                if(ImGui::MenuItem("Copy", "CTRL+C")) {}
                if(ImGui::MenuItem("Paste", "CTRL+V")) {}

                ImGui::EndMenu();
            }
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 20);
            if(ImGui::Button("X", ImVec2(20, 20)))
            {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
            }
            ImGui::EndMainMenuBar();
        }
    }
private:
    bool show_demo_window = true;
    bool show_another_window = true;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btRigidBody *cubeBody;
};
};