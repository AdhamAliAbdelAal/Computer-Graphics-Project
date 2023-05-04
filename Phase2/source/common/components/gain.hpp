#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    // this class is responsible for the coins and fires gains 1 for coins and -1 for fires
    class GainComponent : public Component {
        
    public:
        int gain=0;
        // The ID of this component type is "Gain"
        static std::string getID() { return "Gain"; }
        void deserialize(const nlohmann::json& data) override;
    };

}