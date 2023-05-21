#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include <application.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <set>
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
    private:
        float boost_speed(float x, bool linear)
        {
            if (x == 0)
                return 0;

            int sign = (x > 0) ? 1 : -1;
            if ((!linear && abs(x) > 20) || (linear && abs(x) > glm::pi<float>() * 3))
            {
                return 0;
            }
            return sign * this->speedBooster;
        }

    public:
        float speedBooster = 0.0f;
        ll delay;
        bool updateSpeeds = false;
        float generalSpeed;

        MovementSystem()
        {
            generalSpeed = 5;
            delay = time(0);
        }

        void reset()
        {
            updateSpeeds = false;
            speedBooster = 0.0f;
            delay = time(0);
            generalSpeed = 5;
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime, Application *app)
        {
            if (time(0) - delay > 3)
            {
                speedBooster += 1.0f;
                delay = time(0);
                updateSpeeds = true;
            }
            // For each entity in the world
            float speedMax = generalSpeed;
            for (auto entity : world->getEntities())
            {

                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();

                // If the movement component exists
                if (movement)
                {
                    glm::vec3 currentLinearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
                    if (movement->linearVelocity.z > 0)
                    {
                        currentLinearVelocity = glm::vec3(0.0f, 0.0f, generalSpeed);
                    }
                    glm::vec3 currentAngularVelocity = movement->angularVelocity;
                    if (updateSpeeds)
                    {
                        currentLinearVelocity.x += this->boost_speed(currentLinearVelocity.x, false);
                        currentLinearVelocity.y += this->boost_speed(currentLinearVelocity.y, false);
                        currentLinearVelocity.z += this->boost_speed(currentLinearVelocity.z, false);

                        currentAngularVelocity.x += this->boost_speed(currentAngularVelocity.x, true);
                        currentAngularVelocity.y += this->boost_speed(currentAngularVelocity.y, true);
                        currentAngularVelocity.z += this->boost_speed(currentAngularVelocity.z, true);
                        movement->linearVelocity = currentLinearVelocity;
                        movement->angularVelocity = currentAngularVelocity;
                    }
                    speedMax = max(speedMax, currentLinearVelocity.z);
                    if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                    {
                        currentLinearVelocity.z *= 5.0f;
                        currentAngularVelocity.x *= 5.0f;
                    }
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * currentLinearVelocity;
                    entity->localTransform.rotation += deltaTime * currentAngularVelocity;
                }
            }
            updateSpeeds = false;
            generalSpeed = speedMax;
        }
    };

}
