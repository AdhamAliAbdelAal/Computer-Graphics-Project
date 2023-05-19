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
     * @brief This class is the Road generation system which is responsible for generating the roads and fences and everything related to the road
     */
    class RoadGenerationSystem
    {
    private:
        const nlohmann::json road;         // the road data read from json
        const nlohmann::json fence;        // the fence data read from json
        const int numberOfRoads = 10;       // the number of roads in the system
        const int numberOfFences4Road = 2; // the number of fences in each road
    public:
        RoadGenerationSystem(const nlohmann::json &road, const nlohmann::json &fence, World *world) : road(road), fence(fence)
        {
            for (int i = 0; i < numberOfRoads; ++i)
            {
                Entity *roadEntity = world->objectDeserialize(road);
                float roadZ=-float(i) * 14.0f;
                roadEntity->localTransform.position.z = roadZ;
                // generate the right fence
                for (int j = 0; j < numberOfFences4Road; j++)
                {
                    Entity *fenceEntity = world->objectDeserialize(fence);
                    fenceEntity->name = "right fence";
                    fenceEntity->localTransform.position.z = 4.0f + roadZ - float(j) * 7.0f ;
                }
                // generate the left fence
                for (int j = 0; j < numberOfFences4Road; j++)
                {
                    Entity *fenceEntity = world->objectDeserialize(fence);
                    fenceEntity->name = "left fence";
                    fenceEntity->localTransform.position.z = 3.4f + roadZ - float(j) * 7.0f;
                    fenceEntity->localTransform.position.x *= -1.0f;
                    fenceEntity->localTransform.rotation.y += glm::pi<float>();
                    // cout<<"fenceEntity->localTransform.position.z: "<<fenceEntity->localTransform.rotation.y<<endl;
                }

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
