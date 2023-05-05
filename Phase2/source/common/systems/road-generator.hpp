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
    /**
     * @brief This class is the Road generation system which is responsible for generating the roads
     */
    class RoadGenerationSystem
    {
    private:
        const nlohmann::json road; // the road data read from json
        const int numberOfRoads = 6;   // the number of roads in the system
    public:
        RoadGenerationSystem(const nlohmann::json &road, World *world) : road(road)
        {
            for (int i = 0; i < numberOfRoads ; ++i)
            {
                Entity *roadEntity = world->objectDeserialize(road);
                roadEntity->localTransform.position.z = -float(i) * 14.0f;
            }
        }
        /**
         * @brief empty function
         */
        void update(World *world, float deltaTime)
        {
        }
    };

}
