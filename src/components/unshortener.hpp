#pragma once

#include <userver/cache/lru_cache_component_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace url_shortener::components {

class Unshortener final : public userver::cache::LruCacheComponent<std::string, std::optional<std::string>> {
public:
  constexpr static const std::string_view kName = "unshortener";

  explicit Unshortener(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
  );

private:
  std::optional<std::string> DoGetByKey(const std::string& short_url) override;

  const userver::storages::postgres::ClusterPtr pg_cluster_;
};

}

