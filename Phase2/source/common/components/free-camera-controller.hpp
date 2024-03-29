#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    // This component denotes that the FreeCameraControllerSystem will move the owning entity using user inputs.
    // It will also control the camera field of view accroding to the scrolling of the mouse wheel
    // This component is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/free-camera-controller.hpp"
    // For a more simple example of how to use the ECS framework, see "movement.hpp"
    class FreeCameraControllerComponent : public Component {
    public:
        glm::vec3 positionSensitivity = {10.0f, 10.0f, 10.0f}; // The unity per second of camera movement if WASD is pressed
        float speedupFactor = 5.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.
        float velocity = 10.0f; // The velocity of the jump
        float jumpBoost = 3.0f; // The jump boost in z axis
        float gravity = -9.81f; // The gravity of the jump
        /*
            @author:Adham Ali
            For jumping
            false means down
            true means up
            this boolean used for jumping to indicates the direction of the jump
        */
        bool jumpDirection = false;
        bool enabled = true; // If this is false, the system will not jump the entity

        // The ID of this component type is "Free Camera Controller"
        static std::string getID() { return "Free Camera Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}