#ifndef ENTITY_H
#define ENTITY_H

// Represents a single object in the simulation
typedef struct {
    float x;        // Current x-coordinate
    float y;        // Current y-coordinate
    float vx;       // Current velocity in x
    float vy;       // Current velocity in y
    // We can add color, size, lifetime, etc. later if needed
} Entity;

#endif // ENTITY_H