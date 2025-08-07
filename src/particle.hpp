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

// NOTE: everything in this world is in cm/second (to make the calculation
// easier)
class world {
  int width, height;

  // Maximum life allowed of each particle before it despawns in seconds
  const double max_life_s;
  // Maximum speed a particle can attain in meters per second
  const double max_speed_cms;
  // Maximum number of particles that can be present on the screen ( this need
  // to be controlled to prevent my computer processor from overcooking itself)
  const int max_particles_count;
  // Acceleration due to gravity in our world
  const double g;
  std::vector<std::unique_ptr<particle_t>> particles_buffer;

public:
  world(const int width, const int height, const double max_life_s = 10,
        const int max_particles_count = 1000, const double max_speed_cms = 1000,
        const double g = 980)
      : width(width), height(height), max_life_s(max_life_s),
        max_speed_cms(max_speed_cms), max_particles_count(max_particles_count),
        g(g) {}

  // geters
  auto get_particles_buffer() const { return &particles_buffer; }
  auto get_max_life_s() const { return max_life_s; }
  
  auto update_worldbounds(const int new_width, const int new_height) -> void;

  auto generate_particle(const coords_t location)
      -> std::unique_ptr<particle_t>;
  auto generate_random_particle(const coords_t location)
      -> std::unique_ptr<particle_t>;
  auto spawn_particles(const unsigned int particle_count,
                       const std::vector<coords_t> location_vector) -> bool;

  // operations on each and every particle
  auto collision(std::vector<std::unique_ptr<particle_t>>::iterator particle_it)
      -> bool;
  auto update(const double time_elabsed_ms) -> void;
};

} // namespace particle

#endif /* PARTICLE_H */
