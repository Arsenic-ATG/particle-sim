#include "particle.hpp"
#include <chrono>
#include <memory>
#include <random>

using namespace particle;

void world::update_worldbounds(const int new_width, const int new_height) {
  width = new_width;
  height = new_height;
}

std::unique_ptr<particle_t>
world::generate_random_particle(const coords_t location) {
  // Use a high-resolution clock for a more unique seed
  std::default_random_engine generator(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
  // Create a uniform real distribution for the desired range
  std::uniform_real_distribution<double> life_distribution(1, max_life_s),
      speed_distribution(-1 * max_speed_cms, max_speed_cms);

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
std::unique_ptr<particle_t> generate_particle_debug(const coords_t location) {
  return std::make_unique<particle_t>(
      particle_t{.velocity = {0, -100}, .life = 100, .location = location});
}

bool world::spawn_particles(const unsigned int particle_count,
                            const std::vector<coords_t> location_vector) {
  if (particle_count <= (max_particles_count - particles_buffer.size())) {
    for (auto &current_location : location_vector) {
      // particles_buffer.push_back(generate_particle_debug(current_location));
      particles_buffer.push_back(generate_random_particle(current_location));
    }
    return true;
  }
  // else make space for new particles
  return false;
}

bool world::collision(
    std::vector<std::unique_ptr<particle_t>>::iterator particle_it) {

  bool collision_happened = false;
  // loss in energy (velocity would be reduced by the factor of LOSS)
  const double loss = 0.5;

  // collision with the floor
  if ((*particle_it)->location.y >= height) {
    collision_happened = true;
    (*particle_it)->location.y = height;
    (*particle_it)->velocity.y *= -1;

    (*particle_it)->velocity.y =
        static_cast<int>((*particle_it)->velocity.y * loss);
    (*particle_it)->velocity.x =
        static_cast<int>((*particle_it)->velocity.x * loss);
  }
  // collision with the walls ( collision from the walls would be elastic in
  // nature ) idk why but it looks visually better
  else if ((*particle_it)->location.x >= width) {
    collision_happened = true;
    (*particle_it)->location.x = width;
    (*particle_it)->velocity.x *= -1;
  } else if ((*particle_it)->location.x <= 0) {
    collision_happened = true;
    (*particle_it)->location.x = 0;
    (*particle_it)->velocity.x *= -1;
  }

  return collision_happened;
}

void world::update(const double time_elapsed_s) {
  // update location of every particle
  for (auto curr_particle = particles_buffer.begin();
       curr_particle != particles_buffer.end();) {
    if ((*curr_particle)->life > 0) {
      // s = (u * t) + (0.5)*g*t*t
      auto distance_x = ((*curr_particle)->velocity.x * time_elapsed_s);
      auto distance_y = ((*curr_particle)->velocity.y * time_elapsed_s) +
                        (0.5f * g * time_elapsed_s * time_elapsed_s);

      (*curr_particle)->location.x += distance_x;
      (*curr_particle)->location.y += distance_y;

      if (!collision(curr_particle)) {
        // update the final velocity (v = u + gt)
        (*curr_particle)->velocity.y += (g * time_elapsed_s);
      }

      // update the life of the particles
      (*curr_particle)->life -= time_elapsed_s;
      curr_particle++;
    } else {
      // kill the particle if the it's time has come
      curr_particle = particles_buffer.erase(curr_particle);
    }
  }
}
