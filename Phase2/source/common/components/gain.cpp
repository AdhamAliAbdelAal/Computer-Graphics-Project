#include "gain.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include<iostream>
using namespace std;
namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void GainComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        gain = data.value("gain", gain);
        effect = data.value("effect", effect);
    }
}