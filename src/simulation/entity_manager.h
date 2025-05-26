#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entity.h" // For the Entity struct
#include <stddef.h> // For size_t

// Initializes the entity manager, allocating memory for a fixed number of entities.
// num_entities: The total number of entities to manage.
// screen_width, screen_height: Used to initialize entity positions randomly within bounds.
// Returns 1 on success, 0 on failure (e.g., memory allocation failed).
int entity_manager_init(size_t num_entities, int screen_width, int screen_height);

// Updates the state of all entities.
// dt: Delta time (time since the last frame) for physics-based movement.
// target_x, target_y: The current target position (e.g., mouse cursor) for entities to follow.
void entity_manager_update(float dt, float target_x, float target_y);

// Gets a pointer to the array of all entities.
// Used by the renderer to get data for drawing.
// IMPORTANT: The caller should not free this memory.
const Entity* entity_manager_get_entities(void);

// Gets the total number of active entities.
size_t entity_manager_get_count(void);

// Cleans up resources used by the entity manager (frees allocated memory).
void entity_manager_cleanup(void);

#endif // ENTITY_MANAGER_H