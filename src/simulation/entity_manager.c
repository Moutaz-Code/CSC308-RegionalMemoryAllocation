#include "entity_manager.h"
#include "config.h"       // For NUM_ENTITIES, MOUSE_SMOOTHING_FACTOR
#include <stdlib.h>       // For malloc, free, rand, RAND_MAX
#include <stdio.h>        // For printf, fprintf
#include <math.h>         // For sqrtf, atan2f, cosf, sinf (if needed for complex movement)

#define MAX_SPEED 0.0001f // Maximum speed for entities


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

        

        e->x = target_x; // Snap to target position
        e->y = target_y; // Snap to target position

        

        // Bounds checking
        //if (e->x < 5.0f) e->x = 300.0f; else if (e->x >= (float)WINDOW_WIDTH-100) e->x = 500.0f;
        //if (e->y < 5.0f) e->y = 300.0f; else if (e->y >= (float)WINDOW_HEIGHT-100) e->y = 500.0f;

        if (i == 0 && entity_count > 0) {
            printf("Update Entity 0: x=%.2f, y=%.2f, vx=%.2f, vy=%.2f, target(%.0f,%.0f), dt=%.4f\n",
                   e->x, e->y, e->vx, e->vy, target_x, target_y, dt);
        }
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