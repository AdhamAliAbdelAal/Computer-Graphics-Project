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

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"

    class MovementSystem {
    public:
        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime, Application* app) {
            
            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                
                // If the movement component exists
                if(movement){
                    glm::vec3 currentLinearVelocity = movement->linearVelocity;

                    if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                        currentLinearVelocity.z *= 5.0f;
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * currentLinearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                    // cnt+=(deltaTime*movement->angularVelocity.x);
                    // cout<<"rotate("<<movement->angularVelocity.x<< " , "<<movement->angularVelocity.y<<')'<<'\n';
                    // cout<<"rotate("<<entity->localTransform.rotation.x<< " , "<<entity->localTransform.rotation.y<<')'<<'\n';
                    // cout<<cnt<<'\n';
                }
            }
        }

    };

}
