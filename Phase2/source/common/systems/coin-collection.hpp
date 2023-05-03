#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../ecs/entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

using namespace std;

namespace our
{

    // the coin collection system is responsible for collecting the coins
    // This system is added as a simple example for how use the ECS framework to implement logic.
    class CoinCollectionSystem
    {
    private:
        // the minimum distance between the coin and the player
        float min_dist = 1.0f;

    public:
        // This should be called every frame to update all entities.
        void update(World *world, float deltaTime)
        {
            const unordered_set<Entity *> entities = world->getEntities();
            Entity *ball = nullptr;
            for (auto it : entities)
            {
                if (it->name == "ball")
                {
                    ball = it;
                    break;
                }
            }
            if (!ball)
                return;
            glm::vec3 ball_position=ball->localTransform.position;
            for (auto it : entities)
            {
                if (it->name == "coin")
                {
                    // get the position of the entity
                    glm::vec3 position = it->localTransform.position;
                    float distance=glm::distance(position,ball_position);
                    // if the distance between the coin and the player is less than the minimum distance
                    if (distance<=min_dist)
                    {
                        // cout<<"coin deleted : "<<it<<'\n';
                        // delete the entity
                        world->markForRemoval(it);
                        world->deleteMarkedEntities();
                    }
                }
            }
        }
    };

}
