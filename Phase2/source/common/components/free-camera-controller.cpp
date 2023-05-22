#include "free-camera-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads sensitivities & speedupFactor from the given json object
    void FreeCameraControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        positionSensitivity = data.value("positionSensitivity", positionSensitivity);
        speedupFactor = data.value("speedupFactor", speedupFactor);
    }
}