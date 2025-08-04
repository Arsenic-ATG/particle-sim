#ifndef PARTICLE_H
#define PARTICLE_H

#include <memory>
#include <vector>

namespace particle {

typedef struct {
  double x, y;
} vec;

typedef struct {
  float x, y;
} coords_t;

typedef struct {
  vec velocity;
  double life;
  coords_t location;
} particle_t;

class world {
  // Maximum life allowed of each particle before it despawns in seconds
  const double max_life_s;
  // Maximum speed a particle can attain in meters per second
  const double max_speed_ms;
  // Maximum number of particles that can be present on the screen ( this need
  // to be controlled to prevent my computer processor from overcooking itself)
  const int max_particles_count;
  // Acceleration due to gravity in our world
  const double g;

  std::vector<std::unique_ptr<particle_t>> particles_buffer;

public:
  world(const double max_life_s = 20, const int max_particles_count = 1000,
        const double max_speed_ms = 700, const double g = 980)
      : max_life_s(max_life_s), max_speed_ms(max_speed_ms),
        max_particles_count(max_particles_count), g(g) {}

  // geters
  auto get_particles_buffer() const { return &particles_buffer; }

  auto generate_particle(coords_t location) -> std::unique_ptr<particle_t>;
  auto generate_random_particle(coords_t location)
      -> std::unique_ptr<particle_t>;
  auto prepare_particle_batch() -> void;
  auto spawn_particles(const unsigned int particle_count,
                       std::vector<coords_t> location_vector) -> bool;
  auto update(double time_elabsed_ms) -> void;
};

} // namespace particle

#endif /* PARTICLE_H */
