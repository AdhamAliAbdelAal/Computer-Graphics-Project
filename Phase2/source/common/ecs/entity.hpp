#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <list>
#include <iterator>
#include <string>
#include <glm/glm.hpp>

namespace our
{

    class World; // A forward declaration of the World Class

    class Entity
    {
        World *world;                      // This defines what world own this entity
        std::list<Component *> components; // A list of components that are owned by this entity

        friend World;       // The world is a friend since it is the only class that is allowed to instantiate an entity
        Entity() = default; // The entity constructor is private since only the world is allowed to instantiate an entity
    public:
        /*
            @author:Adham Ali
            For jumping
        */
        float jump = 0.0f;
        bool JumpDirection = false;
        /*
            false means down
            true means up
        */
        std::string name;         // The name of the entity. It could be useful to refer to an entity by its name
        Entity *parent;           // The parent of the entity. The transform of the entity is relative to its parent.
                                  // If parent is null, the entity is a root entity (has no parent).
        Transform localTransform; // The transform of this entity relative to its parent.

        World *getWorld() const { return world; } // Returns the world to which this entity belongs

        glm::mat4 getLocalToWorldMatrix() const;  // Computes and returns the transformation from the entities local space to the world space
        void deserialize(const nlohmann::json &); // Deserializes the entity data and components from a json object

        // This template method create a component of type T,
        // adds it to the components map and returns a pointer to it
        template <typename T>
        T *addComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            // TODO: (Req 8) Create an component of type T, set its "owner" to be this entity, then push it into the component's list
            //  Don't forget to return a pointer to the new component
            //  STEP 1: Create a new component of type T
            T *component = new T();
            // STEP 2: Set the component's owner to be this entity
            component->owner = this;
            // STEP 3: Push the component into the components list
            components.push_back(component);
            // STEP 4: Return a pointer to the new component
            return component;
        }

        // This template method searches for a component of type T and returns a pointer to it
        // If no component of type T was found, it returns a nullptr
        template <typename T>
        T *getComponent()
        {
            // TODO: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            //  Return the component you found, or return null of nothing was found.
            for (auto &it : components)
            {
                // it is a pointer to a component that is currently tested
                T *component = dynamic_cast<T *>(it);
                // if the component is not null, then it is of type T so return it
                if (component != nullptr)
                    return component;
            }
            // if no component of type T was found, return nullptr
            return nullptr;
        }

        // This template method dynamic and returns a pointer to it
        // If no component of type T was found, it returns a nullptr
        template <typename T>
        T *getComponent(size_t index)
        {
            auto it = components.begin();
            std::advance(it, index);
            if (it != components.end())
                return dynamic_cast<T *>(*it);
            return nullptr;
        }

        // This template method searches for a component of type T and deletes it
        template <typename T>
        void deleteComponent()
        {
            // TODO: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            //  If found, delete the found component and remove it from the components list
            for (auto it = components.begin(); it != components.end(); it++)
            {
                // it is a pointer(or iterator)to pointer to a component that is currently tested
                // *it means pointer to the component that is currently tested
                T *component = dynamic_cast<T *>(*it);
                // if the component is not null, then it is of type T so delete it and remove it from the list
                if (component != nullptr)
                {
                    // delete the component object first
                    delete component;
                    // then remove the pointer from the list
                    components.erase(it);
                    break;
                }
            }
        }

        // This template method searches for a component of type T and deletes it
        void deleteComponent(size_t index)
        {
            auto it = components.begin();
            std::advance(it, index);
            if (it != components.end())
            {
                delete *it;
                components.erase(it);
            }
        }

        // This template method searches for the given component and deletes it
        template <typename T>
        void deleteComponent(T const *component)
        {
            // TODO: (Req 8) Go through the components list and find the given component "component".
            //  If found, delete the found component and remove it from the components list
            for (auto it = components.begin(); it != components.end(); it++)
            {
                // it is a pointer(or iterator)to pointer to a component that is currently tested
                // *it means pointer to the component that is currently tested
                // if the two pointers are equal, then delete the component object first, then remove the pointer from the list
                if (*it == component)
                {
                    delete *it;
                    components.erase(it);
                    break;
                }
            }
        }

        // Since the entity owns its components, they should be deleted alongside the entity
        ~Entity()
        {
            // TODO: (Req 8) Delete all the components in "components".
            for (auto &it : components)
            {
                // delete the component object first for each component in the list
                delete it;
            }
            // then clear the list remove all the pointers from it to become empty
            components.clear();
        }

        // Entities should not be copyable
        Entity(const Entity &) = delete;
        Entity &operator=(Entity const &) = delete;
    };

}