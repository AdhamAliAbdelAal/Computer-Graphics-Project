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
#include <ctime>

typedef unsigned long long int ll;

#define MAX_RANGE 5.5f

namespace our
{

    // The Coin generator system is responsible for generating coins and fires randomly
    class CoinGenerationSystem
    {
    private:
        // for coin generation
        ll curr_time;
        ll delay;
        // coins, fire, monster, turbo percentages
        float ratios[4]={0.3,0.3,0.2,0.2};
        float y_pos[4]= {0, -1, 0, 0};

        float generateRandomFloat(float min = -MAX_RANGE, float max = MAX_RANGE)
        {
            return min + rand() * (max - min) / RAND_MAX;
        }

    public:
        // this is the data for the coin
        // to hold the data of the coin read from app config to use it later in the update function
        // const nlohmann::json coin, fire, monster, turbo;
        nlohmann::json objects[4];

        CoinGenerationSystem(const nlohmann::json &coin, const nlohmann::json &fire, const nlohmann::json &monster, const nlohmann::json &turbo) : curr_time(0), delay(500)
        {
            srand(time(0));
            objects[0] = coin;
            objects[1] = fire;
            objects[2] = monster;
            objects[3] = turbo;
            for(int i=1;i<4;++i){
                ratios[i]+=ratios[i-1];
            }
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // check if the time passed is less than the delay
            ll now=glfwGetTime()*1000;
            if (now - curr_time < delay)
                return;
            curr_time = glfwGetTime()*1000;;
            // position of the coin or fire
            glm::vec3 position;
            // create a new entity
            Entity *entity = nullptr;
            // deserialize the data of the coin or fire
            int randomNumber = rand() % 10+1;
            float randomFloat = (float)randomNumber / 10;
            for(int i=0;i<4;++i){
                if(randomFloat<=ratios[i]){
                    entity = world->objectDeserialize(objects[i]);
                    position = glm::vec3(generateRandomFloat(), y_pos[i], -20);
                    break;
                }
            }
            if (!entity)
                return;
            entity->localTransform.position = position;

            // delete all coins that are out of the screen
            const unordered_set<Entity *> entities = world->getEntities();
            for (auto it : entities)
            {
                if (!it)
                    continue;
                // if the entity has a gain component
                GainComponent *gain = it->getComponent<GainComponent>();
                if (gain)
                {
                    // get the position of the entity
                    glm::vec3 &position = it->localTransform.position;
                    // if the position of the entity is greater than 10
                    if (position.z > 10)
                    {
                        // delete the entity
                        world->markForRemoval(it);
                    }
                }
                if(it->name=="ball"){
                    MovementComponent *ball = it->getComponent<MovementComponent>();
                    if(ball){
                        // objects are generated based on the speed of the ball
                        delay=max(ball->angularVelocity.x*60+1000.0f, 300.0f);
                    }
                }
            }
            world->deleteMarkedEntities();
        }
    };

}
