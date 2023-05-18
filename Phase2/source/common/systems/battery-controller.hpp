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
#define LEVEL_WEIGHT 4.0    // weight of the level
#define LOW_ENERGY_BAR_COLOR "battery_bar_red"
#define MEDIUM_ENERGY_BAR_COLOR "battery_bar_yellow"
#define HIGH_ENERGY_BAR_COLOR "battery_bar_green"

namespace our
{

    class BatterySystem
    {
        nlohmann::json worldJson;
        World *worldPtr;
        int current_battery_level;
        nlohmann::json texturesJson;

        void remove_bar(int bar_number)
        {
            string bar_name = "bar-" + std::to_string(bar_number);
            for (auto entity : this->worldPtr->getEntities())
            {
                if (entity->name == bar_name)
                {
                    this->worldPtr->markForRemoval(entity);
                    this->worldPtr->deleteMarkedEntities();
                    return;
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
                    obj.at("components")[0].at("material") = this->change_bar_color();
                    this->worldPtr->objectDeserialize(obj);
                    break;
                }
            }
 
        }

         string change_bar_color(){
            switch (this->current_battery_level)
            {
            case 1:
                return LOW_ENERGY_BAR_COLOR;
                break;

            case 2:
            case 3:
                return MEDIUM_ENERGY_BAR_COLOR;
                break;
            
            case 4:
            case 5:
                return HIGH_ENERGY_BAR_COLOR;
                break;

            default:
                return LOW_ENERGY_BAR_COLOR;
                break;
            }
            
        }


    public:


        BatterySystem(const nlohmann::json &worldJson, const nlohmann::json &texturesJson, World *worldPtr) : worldJson(worldJson), texturesJson(texturesJson), worldPtr(worldPtr)
        {
            this->remove_all_bars();
            this->current_battery_level = 0;
        }


        int update_battery(int coins_accumulator){

            // wining state
            if(coins_accumulator > MAX_LEVELS_TO_WIN * LEVEL_WEIGHT)
                return 1;

            this->current_battery_level = ceil(coins_accumulator / LEVEL_WEIGHT);

            // game over state
            if(coins_accumulator < 0)
                return -1;

            // normal logic
            this->remove_all_bars();
            for(int i = 1; i <= this->current_battery_level; i++)
                this->add_bar(i);
            
            return 0;
        }
    };

}
