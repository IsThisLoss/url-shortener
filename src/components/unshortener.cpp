#include "unshortener.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/component.hpp>

#include <db/urls.hpp>

#include <utils/postgres.hpp>

namespace url_shortener::components {

Unshortener::Unshortener(
  const userver::components::ComponentConfig& config,
  const userver::components::ComponentContext& context
)
  : userver::cache::LruCacheComponent<std::string, std::optional<std::string>>{config, context}
  , pg_cluster_{
        context
            .FindComponent<userver::components::Postgres>(utils::kPostgres)
            .GetCluster()}
{}

std::optional<std::string> Unshortener::DoGetByKey(const std::string& short_url) {
  const db::Urls urls{pg_cluster_};
  return urls.FindOriginal(short_url);
}

}
