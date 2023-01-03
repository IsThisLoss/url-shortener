#include "urls.hpp"

namespace url_shortener::db {

namespace {

// FIXME It would be nice to store SQL-queries as separated .sql files
static const userver::storages::postgres::Query kInsertUrls{R"(
  INSERT INTO url_shortener.urls (
    short_url,
    original_url
  ) VALUES ($1, $2)
  ON CONFLICT (short_url) DO NOTHING
  RETURNING id
  )",
  userver::storages::postgres::Query::Name{"insert_urls"}
};

static const userver::storages::postgres::Query kFindUrls{R"(
  SELECT
    original_url
  FROM
    url_shortener.urls
  WHERE
    short_url = $1
  LIMIT 1
  )",
  userver::storages::postgres::Query::Name{"find_urls"}
};

constexpr userver::storages::postgres::ClusterHostTypeFlags kWriteFlags{userver::storages::postgres::ClusterHostType::kMaster};

constexpr userver::storages::postgres::ClusterHostTypeFlags kReadFlags{userver::storages::postgres::ClusterHostType::kSlaveOrMaster};

}

Urls::Urls(const userver::storages::postgres::ClusterPtr& pg) : pg_{pg}
{}

bool Urls::Insert(const std::string& original_url, const std::string& short_url) const {
  const auto result = pg_->Execute(kWriteFlags, kInsertUrls, original_url, short_url);
  // if result set is empty it means that we triggered ON CONFLICT DO NOTHING part,
  // so given short_url already exists in database
  return !result.IsEmpty();
}

std::optional<std::string> Urls::FindOriginal(const std::string& short_url) const {
  const auto result = pg_->Execute(kReadFlags, kFindUrls, short_url);
  if (result.IsEmpty()) {
    return std::nullopt;
  }
  return result.AsSingleRow<std::string>();
}

}
