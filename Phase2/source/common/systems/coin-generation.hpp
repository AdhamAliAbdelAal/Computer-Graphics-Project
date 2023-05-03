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
#include <ctime>

typedef long long int ll;

#define MAX_RANGE 5.5f

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CoinGenerationSystem
    {
    private:
        // for coin generation
        ll curr_time;
        ll delay;

        float generateRandomFloat(float min = -MAX_RANGE, float max = MAX_RANGE)
        {
            return min + rand() * (max - min) / RAND_MAX;
        }

    public:
        // this is the data for the coin
        // to hold the data of the coin read from app config to use it later in the update function
        const nlohmann::json &data;

        CoinGenerationSystem(const nlohmann::json &data) : data(data), curr_time(0), delay(3)
        {
            srand(time(0));
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // check if the time passed is less than the delay
            if (time(NULL) - curr_time < delay)
                return;
            curr_time = time(NULL);
            // create a new entity
            Entity *entity = nullptr;
            // deserialize the data of the coin
            entity = world->objectDeserialize(data);
            cout<<"coin generated : "<<entity<<'\n';
            if (!entity)
                return;
            // set the position of the coin by generating a random float in x axis in the range of -5.5 to 5.5
            glm::vec3 position = glm::vec3(generateRandomFloat(), 0, -2);
            // cout << generateRandomFloat() << '\n';
            // change the position of the coin to the generated position
            entity->localTransform.position = position;

            // delete all coins that are out of the screen
            const unordered_set<Entity*> entities = world->getEntities();
            for (auto it : entities)
            {
                cout<<"entity : "<<it<<'\n';
                if(!it) continue;
                // if the entity has a movement component
                if (it->name == "coin")
                {
                    // get the position of the entity
                    glm::vec3 &position = it->localTransform.position;
                    // if the position of the entity is greater than 10
                    if (position.z > 10)
                    {
                        cout<<"coin deleted : "<<it<<'\n';
                        // delete the entity
                        world->markForRemoval(it);
                        world->deleteMarkedEntities();
                    }
                }
            }
        }
    };

}
