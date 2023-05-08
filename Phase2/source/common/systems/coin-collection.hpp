#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/gain.hpp"
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
        float coin_min_dist = 1.0f;
        float fire_min_dist = 1.5f;

    public:
        int accumulator = 0;
        // This should be called every frame to update all entities.
        bool update(World *world, float deltaTime)
        {
            cout<<accumulator<<'\n';
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
                return false;
            glm::vec3 ball_position=ball->localTransform.position;
            for (auto it : entities)
            {
                GainComponent*gainComponent=it->getComponent<GainComponent>();
                if (gainComponent)
                {
                    // get the position of the entity
                    glm::vec3 position = it->localTransform.position;
                    float distance=glm::distance(position,ball_position);
                    float min_dist = gainComponent->gain == 1 ? coin_min_dist : fire_min_dist;
                    // if the distance between the coin and the player is less than the minimum distance
                    if (distance<=min_dist)
                    {
                        accumulator += gainComponent->gain;

                        if(accumulator<0) {
                            return true;
                        }
                        // cout<<"coin deleted : "<<it<<'\n';
                        // delete the entity
                        world->markForRemoval(it);
                        world->deleteMarkedEntities();
                    }
                    
                }
            
            }
            return false;
        }
    };

}
