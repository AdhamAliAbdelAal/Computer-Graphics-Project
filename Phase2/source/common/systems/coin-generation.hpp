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

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CoinGenerationSystem
    {
    private:
        ll curr_time;

    public:
        const nlohmann::json &data;

        CoinGenerationSystem(const nlohmann::json &data) : data(data),curr_time(0) {}

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            cout << "coin generation system update" << time(NULL) << '\n';
            if (curr_time > time(NULL))
                return;
            curr_time = time(NULL);
            Entity *entity = nullptr;
            if (curr_time % 13 == 0 || curr_time % 17==0 || curr_time % 11==0)
            {
                entity = world->objectDeserialize(data);
            }
            if (!entity)
                return;
            glm::vec3 position;
            if (curr_time % 17==0)
            {
                position = glm::vec3(-2, 0, -2); // The position is defined as a vec3. (0,0,0) means no translation
            }
            else if (curr_time % 13==0)
            {
                position = glm::vec3(0, 0, -2); // The position is defined as a vec3. (0,0,0) means no translation
            }
            else if (curr_time % 11==0)
            {
                position = glm::vec3(2, 0, -2); // The position is defined as a vec3. (0,0,0) means no translation
            }
            entity->localTransform.position = position;
            ++curr_time;
        }
    };

}
