#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include <application.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

using namespace std;
#include <ctime>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"

    typedef long long int ll;

    class MovementSystem
    {

    public:
        float speedBooster = 5.0f;
        ll delay;
        bool updateSpeeds = false;

        MovementSystem()
        {
            srand(time(0));
            delay = time(0);
        }

        void reset()
        {
            speedBooster = 5.0f;
            delay = time(0);
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime, Application *app)
        {
            if (time(0) - delay > 10)
            {
                speedBooster += 1.0f;
                cout << "delay: " << speedBooster << '\n';
                delay = time(0);
                updateSpeeds = true;
            }
            // For each entity in the world
            for (auto entity : world->getEntities())
            {

                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();

                // If the movement component exists
                if (movement)
                {
                    glm::vec3 currentLinearVelocity = movement->linearVelocity;
                    currentLinearVelocity.x += (currentLinearVelocity.x) ? speedBooster * currentLinearVelocity.x / abs(currentLinearVelocity.x) : 0;
                    currentLinearVelocity.y += (currentLinearVelocity.y) ? speedBooster * currentLinearVelocity.y / abs(currentLinearVelocity.y) : 0;
                    currentLinearVelocity.z += (currentLinearVelocity.z) ? speedBooster * currentLinearVelocity.z / abs(currentLinearVelocity.z) : 0;

                    glm::vec3 currentAngularVelocity = movement->angularVelocity;
                    currentAngularVelocity.x += (currentAngularVelocity.x) ? speedBooster * currentAngularVelocity.x / abs(currentAngularVelocity.x) : 0;
                    currentAngularVelocity.y += (currentAngularVelocity.y) ? speedBooster * currentAngularVelocity.y / abs(currentAngularVelocity.y) : 0;
                    currentAngularVelocity.z += (currentAngularVelocity.z) ? speedBooster * currentAngularVelocity.z / abs(currentAngularVelocity.z) : 0;
                    if (updateSpeeds)
                    {
                        movement->linearVelocity = currentLinearVelocity;
                        movement->angularVelocity = currentAngularVelocity;
                        updateSpeeds = false;
                    }
                    if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                    {
                        currentLinearVelocity.z *= 5.0f;
                        currentAngularVelocity.x *= 5.0f;
                    }
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * currentLinearVelocity;
                    entity->localTransform.rotation += deltaTime * currentAngularVelocity;
                    // cnt+=(deltaTime*movement->angularVelocity.x);
                    // cout<<"rotate("<<movement->angularVelocity.x<< " , "<<movement->angularVelocity.y<<')'<<'\n';
                    // cout<<"rotate("<<entity->localTransform.rotation.x<< " , "<<entity->localTransform.rotation.y<<')'<<'\n';
                    // cout<<cnt<<'\n';
                }
            }
        }
    };

}
