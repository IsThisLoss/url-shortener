#pragma once

#include <userver/components/loggable_component_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace url_shortener::components {

class Shortener : public userver::components::LoggableComponentBase {
public:
  constexpr static const std::string_view kName = "shortener";

  explicit Shortener(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
  );

  std::string MakeShortUrl(const std::string& original_url) const;

private:
  const userver::storages::postgres::ClusterPtr pg_cluster_;
  const std::string base_url_;
  const std::size_t len_;
};

}
