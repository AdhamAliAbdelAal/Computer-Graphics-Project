#pragma once

#include <unordered_set>
#include "entity.hpp"

namespace our {

    // This class holds a set of entities
    class World {
        std::unordered_set<Entity*> entities; // These are the entities held by this world
        std::unordered_set<Entity*> markedForRemoval; // These are the entities that are awaiting to be deleted
                                                      // when deleteMarkedEntities is called
    public:

        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json& data, Entity* parent = nullptr);
        Entity * objectDeserialize(const nlohmann::json &data, Entity *parent= nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity* add() {
            //TODO: (Req 8) Create a new entity, set its world member variable to this,
            // and don't forget to insert it in the suitable container.
            // STEP 1: Create a new entity
            Entity*entity=new Entity();
            // STEP 2: Set the entity's world to be this world
            entity->world=this;
            // STEP 3: Insert the entity into the entities set
            entities.insert(entity);
            // STEP 4: Return a pointer to the new entity
            return entity;
        }

        // This returns and immutable reference to the set of all entities in the world.
        const std::unordered_set<Entity*>& getEntities() {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity* entity){
            //TODO: (Req 8) If the entity is in this world, add it to the "markedForRemoval" set.
            // STEP 1: Check if the entity is in this world
            if(entities.find(entity)!=entities.end()){
                // STEP 2: Add the entity to the "markedForRemoval" set
                markedForRemoval.insert(entity);
                entities.erase(entity);
            }
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities(){
            //TODO: (Req 8) Remove and delete all the entities that have been marked for removal
            // STEP 1: Remove the entities in "markedForRemoval" from the "entities" set
            for(auto&it:markedForRemoval){
                // STEP 2: Remove the entity from the "entities" set
                delete it;
            }
            // STEP 3: Clear the "markedForRemoval" set
            markedForRemoval.clear();
        }

        //This deletes all entities in the world
        void clear(){
            //TODO: (Req 8) Delete all the entities and make sure that the containers are empty
            for(auto&it:entities){
                // Delete the entity objects
                delete it;
            }
            for(auto&it:markedForRemoval){
                // Delete the entity objects in the markedForRemoval set
                delete it;
            }
            // Clear the containers make them empty
            entities.clear();
            markedForRemoval.clear();
        }

        //Since the world owns all of its entities, they should be deleted alongside it.
        ~World(){
            clear();
        }

        // The world should not be copyable
        World(const World&) = delete;
        World &operator=(World const &) = delete;
    };

}