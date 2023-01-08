#include "shortener.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/crypto/hash.hpp>
#include <userver/storages/postgres/component.hpp>

#include <db/urls.hpp>

#include <utils/postgres.hpp>

namespace url_shortener::components {

Shortener::Shortener(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : userver::components::LoggableComponentBase{config, context},
      pg_cluster_{
          context
              .FindComponent<userver::components::Postgres>(utils::kPostgres)
              .GetCluster()}
  , base_url_{config["base_url"].As<std::string>()}
  , len_{config["len"].As<std::size_t>()}
{}


std::string Shortener::MakeShortUrl(const std::string& original_url) const {
  const db::Urls urls{pg_cluster_};

  const auto url_hash = userver::crypto::hash::weak::Md5(original_url, userver::crypto::hash::OutputEncoding::kBase64);

  for (size_t i = 0; i + len_ < url_hash.size(); i += len_) {
    const auto candidate = url_hash.substr(i, len_);
    if (urls.Insert(original_url, candidate)) {
      return fmt::format("{}/{}", base_url_, candidate);
    }
  }

  throw std::runtime_error{fmt::format("Unable make short url for {}, too many collisions", original_url)};
}

}
