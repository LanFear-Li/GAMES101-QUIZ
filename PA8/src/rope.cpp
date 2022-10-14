#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes) {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
        Vector2D step = (end - start) / (num_nodes - 1);
        for (int i = 0; i < num_nodes; i++) {
            Vector2D pos = start + i * step;
            Mass *m = new Mass(pos, node_mass, false);
            masses.push_back(m);

            if (i) {
                Spring *s = new Spring(masses[i - 1], masses[i], k);
                springs.push_back(s);
            }
        }

        for (auto &i: pinned_nodes) {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity) {
        for (auto &s: springs) {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Mass *a = s->m1;
            Mass *b = s->m2;
            Vector2D f = -s->k *
                         (b->position - a->position).unit() *
                         ((b->position - a->position).norm() - s->rest_length);

            b->forces += f;
            a->forces += -f;
        }

        for (auto &m: masses) {
            if (!m->pinned) {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += m->mass * gravity;

                // TODO (Part 2): Add global damping
                m->forces -= 0.01 * m->velocity;

                Vector2D a = m->forces / m->mass;
                m->velocity = m->velocity + a * delta_t;
                m->position = m->position + m->velocity * delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    // skip explicit Verlet method ^^
    void Rope::simulateVerlet(float delta_t, Vector2D gravity) {
        for (auto &s: springs) {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
        }

        for (auto &m: masses) {
            if (!m->pinned) {
                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass

                // TODO (Part 4): Add global Verlet damping
            }
        }
    }
}
