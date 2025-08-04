#include "particle.hpp"
#include <chrono>
#include <memory>
#include <random>

using namespace particle;

std::unique_ptr<particle_t> world::generate_random_particle(coords_t location) {
  // Use a high-resolution clock for a more unique seed
  std::default_random_engine generator(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  // Create a uniform real distribution for the desired range
  std::uniform_real_distribution<double> life_distribution(1, max_life_s),
      speed_distribution(-1 * max_speed_ms, max_speed_ms);

  // reducing the individual velocity of each component as to make sure the
  // total magnitude doesn't cross maximum limit
  vec velocity = {speed_distribution(generator) / 2.0f,
                  speed_distribution(generator) / 2.0f};
  return std::make_unique<particle_t>(
      particle_t{.velocity = velocity,
                 .life = life_distribution(generator),
                 .location = location});
}

// debug function
std::unique_ptr<particle_t> generate_particle_debug(coords_t location) {
  return std::make_unique<particle_t>(
      particle_t{.velocity = {0, -100}, .life = 100, .location = location});
}

bool world::spawn_particles(const unsigned int particle_count,
                            std::vector<coords_t> location_vector) {
  if (particle_count <= (max_particles_count - particles_buffer.size())) {
    for (auto &current_location : location_vector) {
      //particles_buffer.push_back(generate_particle_debug(current_location));
      particles_buffer.push_back(generate_random_particle(current_location));
    }
    return true;
  }
  // else make space for new particles
  return false;
}

void world::update(double time_elapsed_s) {
  // update location of every particle
  for (auto &curr_particle : particles_buffer) {
    // s = (u * t) + (0.5)*g*t*t
    auto distance_x = (curr_particle->velocity.x * time_elapsed_s);
    auto distance_y = (curr_particle->velocity.y * time_elapsed_s) +
                      (0.5f * g * time_elapsed_s * time_elapsed_s);

    curr_particle->location.x += distance_x;
    curr_particle->location.y += distance_y;
    
    // update the final velocity (v = u + gt)
    curr_particle->velocity.y += (g * time_elapsed_s);
    
    // check for collision
    // if collision then update the velocity accordingly
  }
  // cull dead particles
}
