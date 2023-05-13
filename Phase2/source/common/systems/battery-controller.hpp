#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <vector>
#include <iostream>
using namespace std;
#define MAX_LEVELS_TO_WIN 5
#define LEVELS_COUNT 4.0   // weight of the level

namespace our
{

    class BatterySystem
    {
        const nlohmann::json worldJson;
        World *worldPtr;
        int current_battery_level;

        void remove_bar(int bar_number)
        {
            string bar_name = "bar-" + std::to_string(bar_number);
            for (auto entity : this->worldPtr->getEntities())
            {
                if (entity->name == bar_name)
                {
                    this->worldPtr->markForRemoval(entity);
                    this->worldPtr->deleteMarkedEntities();

                }
            }
 
        }

        void remove_all_bars()
        {
            for(int i = 1; i <= 5; i++)
                remove_bar(i);
        }

        void add_bar(int bar_number)
        {
            string bar_name = "bar-" + std::to_string(bar_number);
            for (auto& obj : this->worldJson) {
                if (obj["name"] == bar_name) {
                    // cout << obj.dump(4) << std::endl; // print the object to console
                    this->worldPtr->objectDeserialize(obj);
                    break;
                }
            }
 
        }


    public:


        BatterySystem(const nlohmann::json &worldJson, World *worldPtr) : worldJson(worldJson), worldPtr(worldPtr)
        {
            // this->remove_all_bars();
            this->current_battery_level = 0;
        }


        int update_battery(int coins_accumulator){

            this->current_battery_level = floor(coins_accumulator / LEVELS_COUNT);

            // wining state
            if(this->current_battery_level >= MAX_LEVELS_TO_WIN)
            {
                return 1;
            }

            // game over state
            if(this->current_battery_level < 0){
                return -1;
            }

            // normal logic
            // this->remove_all_bars();
            for(int i = 1; i <= this->current_battery_level; i++)
                this->add_bar(i);
            
            return 0;
        }
    };

}
