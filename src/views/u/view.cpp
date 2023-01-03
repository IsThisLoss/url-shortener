#include "view.hpp"

#include <userver/components/component_context.hpp>

#include <userver/storages/postgres/component.hpp>

#include <userver/formats/json.hpp>

#include <db/urls.hpp>

namespace url_shortener::views::u {

namespace {

// TODO utils
userver::formats::json::Value Make400(const std::string& code, const std::string& message) {
  userver::formats::json::ValueBuilder result;
  result["code"] = code;
  result["message"] = message;
  return result.ExtractValue();
}

}

View::View(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context)
      : HttpHandlerJsonBase(config, context),
        pg_cluster_(
            context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

userver::formats::json::Value View::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value&,
    userver::server::request::RequestContext&) const {
  auto& response = request.GetHttpResponse();

  const auto short_url = request.GetPathArg("key");

  url_shortener::db::Urls urls{pg_cluster_};

  const auto original_url = urls.FindOriginal(short_url);
  if (!original_url.has_value()) {
    response.SetStatus(userver::server::http::HttpStatus::kNotFound);
    return Make400("NOT_FOUND", fmt::format("Url {} was not found", short_url));
  }

  response.SetStatus(userver::server::http::HttpStatus::kMovedPermanently);
  response.SetHeader("Location", original_url.value());
  return {};
}

}
