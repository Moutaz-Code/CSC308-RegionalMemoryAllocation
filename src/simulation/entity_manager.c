#include "entity_manager.h"
#include "config.h"       // For NUM_ENTITIES, MOUSE_SMOOTHING_FACTOR
#include <stdlib.h>       // For malloc, free, rand, RAND_MAX
#include <stdio.h>        // For printf, fprintf
#include <math.h>         // For sqrtf, atan2f, cosf, sinf (if needed for complex movement)

static Entity* entities = NULL; // Pointer to our array of entities
static size_t entity_count = 0; // The number of entities we are managing

// Helper to generate a random float between min and max
static float random_float(float min, float max) {
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

int entity_manager_init(size_t num_entities, int screen_width, int screen_height) {
    if (entities != NULL) {
        fprintf(stderr, "Entity manager already initialized.\n");
        // Or free existing and reinitialize, depending on desired behavior
        return 0; 
    }

    entities = (Entity*)malloc(num_entities * sizeof(Entity));
    if (!entities) {
        fprintf(stderr, "ERROR (EntityManager): Failed to allocate memory for %zu entities.\n", num_entities);
        return 0;
    }

    entity_count = num_entities;
    printf("Entity manager initialized for %zu entities.\n", entity_count);

    // Initialize entities with random positions and zero velocity
    for (size_t i = 0; i < entity_count; ++i) {
        entities[i].x = random_float(0.0f, (float)screen_width);
        entities[i].y = random_float(0.0f, (float)screen_height);
        entities[i].vx = 0.0f;
        entities[i].vy = 0.0f;
    }
    return 1;
}

void entity_manager_update(float dt, float target_x, float target_y) {
    if (!entities) return;

    for (size_t i = 0; i < entity_count; ++i) {
        Entity* e = &entities[i];

        // Calculate direction to target
        float dx = target_x - e->x;
        float dy = target_y - e->y;
        
        // Simple interpolation towards target (adjust MOUSE_SMOOTHING_FACTOR in config.h)
        // This creates a basic "follow" behavior.
        e->vx += dx * MOUSE_SMOOTHING_FACTOR * dt * 60.0f; // Multiply by 60 for frame rate independence assumption
        e->vy += dy * MOUSE_SMOOTHING_FACTOR * dt * 60.0f;

        // Apply some damping/friction to velocity so they don't accelerate indefinitely
        e->vx *= 0.95f; 
        e->vy *= 0.95f;

        // Update position based on velocity
        e->x += e->vx * dt;
        e->y += e->vy * dt;

        // Optional: Keep entities within screen bounds (simple wrap-around or clamp)
        // if (e->x < 0) e->x = 0; else if (e->x > WINDOW_WIDTH) e->x = WINDOW_WIDTH;
        // if (e->y < 0) e->y = 0; else if (e->y > WINDOW_HEIGHT) e->y = WINDOW_HEIGHT;
    }
}

const Entity* entity_manager_get_entities(void) {
    return entities;
}

size_t entity_manager_get_count(void) {
    return entity_count;
}

void entity_manager_cleanup(void) {
    if (entities) {
        free(entities);
        entities = NULL;
    }
    entity_count = 0;
    printf("Entity manager cleaned up.\n");
}