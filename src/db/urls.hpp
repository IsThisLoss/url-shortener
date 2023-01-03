#pragma once

#include <userver/storages/postgres/cluster.hpp>

namespace url_shortener::db {

/// @brief Gives access to urls table in postgres
class Urls {
public:
  explicit Urls(const userver::storages::postgres::ClusterPtr& pg);

  /// @brief inserts new entry in table
  ///
  /// returns true if urls was successfully inserted otherwise false
  bool Insert(const std::string& original_url, const std::string& short_url) const;

  /// @brief finds original url by its short version
  ///
  /// returns nullopt if there is no such short url in databas
  std::optional<std::string> FindOriginal(const std::string& short_url) const;
private:
  const userver::storages::postgres::ClusterPtr pg_;
};

}
