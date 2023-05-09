#include "battery_bar.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include<iostream>

namespace our {
    void BatteryBarComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        id = data.value("id", id);
    }
}