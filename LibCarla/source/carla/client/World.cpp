// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/client/World.h"

#include "carla/Logging.h"
#include "carla/client/Actor.h"
#include "carla/client/ActorBlueprint.h"
#include "carla/client/ActorList.h"
#include "carla/client/detail/Simulator.h"

#include <exception>

namespace carla {
namespace client {

  uint32_t World::GetId() const {
    return _episode.Lock()->GetCurrentEpisodeId();
  }

  const std::string &World::GetMapName() const {
    return _episode.Lock()->GetCurrentMapName();
  }

  SharedPtr<Map> World::GetMap() const {
    return _episode.Lock()->GetCurrentMap();
  }

  SharedPtr<BlueprintLibrary> World::GetBlueprintLibrary() const {
    return _episode.Lock()->GetBlueprintLibrary();
  }

  SharedPtr<Actor> World::GetSpectator() const {
    return _episode.Lock()->GetSpectator();
  }

  rpc::WeatherParameters World::GetWeather() const {
    return _episode.Lock()->GetWeatherParameters();
  }

  void World::SetWeather(const rpc::WeatherParameters &weather) {
    _episode.Lock()->SetWeatherParameters(weather);
  }

  SharedPtr<ActorList> World::GetActors() const {
    return SharedPtr<ActorList>{new ActorList{
        _episode,
        _episode.Lock()->GetAllTheActorsInTheEpisode()}};
  }

  SharedPtr<Actor> World::SpawnActor(
      const ActorBlueprint &blueprint,
      const geom::Transform &transform,
      Actor *parent_actor) {
    try {
      return _episode.Lock()->SpawnActor(blueprint, transform, parent_actor);
    } catch (const std::exception &e) {
      log_warning("SpawnActor: failed with:", e.what());
      throw;
    }
  }

  SharedPtr<Actor> World::TrySpawnActor(
      const ActorBlueprint &blueprint,
      const geom::Transform &transform,
      Actor *parent_actor) noexcept {
    try {
      return SpawnActor(blueprint, transform, parent_actor);
    } catch (const std::exception &) {
      return nullptr;
    }
  }

  Timestamp World::WaitForTick(time_duration timeout) const {
    return _episode.Lock()->WaitForTick(timeout);
  }

  void World::OnTick(std::function<void(Timestamp)> callback) {
    return _episode.Lock()->RegisterOnTickEvent(std::move(callback));
  }

} // namespace client
} // namespace carla
