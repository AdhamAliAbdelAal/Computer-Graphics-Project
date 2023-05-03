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
        ll curr_time;
        ll delay;

        float generateRandomFloat(float min=-MAX_RANGE, float max=MAX_RANGE)
        {
            return min + rand()*(max - min) / RAND_MAX;
        }
    public:
        const nlohmann::json &data;

        CoinGenerationSystem(const nlohmann::json &data) : data(data),curr_time(0),delay(3) {
            srand(time(0));
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // cout << "coin generation system update" << time(NULL) << '\n';
            if (time(NULL)-curr_time < delay)
                return;
            curr_time = time(NULL);
            Entity *entity = nullptr;
            entity = world->objectDeserialize(data);
            if (!entity)
                return;
            glm::vec3 position=glm::vec3(generateRandomFloat(),0,-2);
            cout<<generateRandomFloat()<<'\n';
            entity->localTransform.position = position;
        }
    };

}
