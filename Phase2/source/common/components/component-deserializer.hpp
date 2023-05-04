#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "gain.hpp"
#include <iostream>
using namespace std;
namespace our {

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json& data, Entity* entity){
        std::string type = data.value("type", "");

        
        // We check the type of the component and add it to the entity.
        // Then we deserialize the rest of the data to the component
        Component* component = nullptr;
        //TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        if(type == CameraComponent::getID()){
            component = entity->addComponent<CameraComponent>();
        } else if (type == FreeCameraControllerComponent::getID()) {
            component = entity->addComponent<FreeCameraControllerComponent>();
        } else if (type == MovementComponent::getID()) {
            component = entity->addComponent<MovementComponent>();
        } else if(type == MeshRendererComponent::getID()){
            component = entity->addComponent<MeshRendererComponent>();
        }
        else if(type== GainComponent::getID()){
            component = entity->addComponent<GainComponent>();
        }
        if(component) component->deserialize(data);
    }

}