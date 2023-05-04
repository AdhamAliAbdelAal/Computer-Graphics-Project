#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        const float MAX_X_POSITION = 5.5;
        const float MAX_FORWARD_POSITION = -2.f;
        const float MAX_BACKWARD_POSITION = 5.5;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            MeshRendererComponent *meshRenderer = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                meshRenderer = entity->getComponent<MeshRendererComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (meshRenderer && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(meshRenderer && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = controller->getOwner();

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions

            glm::vec3 front = glm::vec3(glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(glm::vec4(0, 1, -0.5, 0)),
                      right = glm::vec3(glm::vec4(1, 0, 0, 0));
            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // Check for Jump
            if (controller->jumpDirection)
            {
                // controller->velocity += deltaTime * controller->gravity;
                if (position.y >= 3.0f)
                {
                    controller->jumpDirection = false;
                }
                else
                {
                    controller->velocity += deltaTime * controller->gravity;
                    position += up * (deltaTime * controller->velocity);
                    position += front * (deltaTime * controller->jumpBoost);
                }
            }
            else
            {
                if (position.y > 0.0f)
                {
                    controller->velocity -= deltaTime * controller->gravity;
                    position -= up * (deltaTime * controller->velocity);
                    position += front * (deltaTime * controller->jumpBoost);
                }
                else
                {
                    position.y = 0.0f;
                    controller->enabled = true;
                }
            }

            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            // W & UP moves the player forward
            if (app->getKeyboard().isPressed(GLFW_KEY_W) || app->getKeyboard().isPressed(GLFW_KEY_UP))
            {
                if (position.z > MAX_FORWARD_POSITION)
                    position += front * (deltaTime * current_sensitivity.z);
            }
            // S & DOWN moves the player backward
            if (app->getKeyboard().isPressed(GLFW_KEY_S) || app->getKeyboard().isPressed(GLFW_KEY_DOWN))
            {
                if (position.z < MAX_BACKWARD_POSITION)
                    position -= front * (deltaTime * current_sensitivity.z);
            }

            // Space moves the player up
            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE)&&controller->enabled)
            {
                controller->jumpDirection = true;
                controller->enabled = false;
            }
            // D & RIGHT moves the player right
            if (app->getKeyboard().isPressed(GLFW_KEY_D) || app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
            {
                if (position.x < MAX_X_POSITION)
                    position += right * (deltaTime * current_sensitivity.x);
            }
            // A & LEFT moves the player left
            if (app->getKeyboard().isPressed(GLFW_KEY_A) || app->getKeyboard().isPressed(GLFW_KEY_LEFT))
            {
                if (position.x > -MAX_X_POSITION)
                    position -= right * (deltaTime * current_sensitivity.x);
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}
