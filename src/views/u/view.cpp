#include "view.hpp"

#include <userver/components/component_context.hpp>

#include <userver/storages/postgres/component.hpp>

#include <userver/formats/json.hpp>

#include <db/urls.hpp>

#include <utils/http.hpp>
#include <utils/postgres.hpp>

namespace url_shortener::views::u {

View::View(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context)
      : HttpHandlerJsonBase(config, context)
      , unshortener_{context.FindComponent<components::Unshortener>()}
{}

userver::formats::json::Value View::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value&,
    userver::server::request::RequestContext&) const {
  auto& response = request.GetHttpResponse();

  const auto short_url = request.GetPathArg("key");

  const auto original_url = unshortener_.GetCache().Get(short_url);
  if (!original_url.has_value()) {
    response.SetStatus(userver::server::http::HttpStatus::kNotFound);
    return utils::Make400("NOT_FOUND", fmt::format("Url {} was not found", short_url));
  }

  response.SetStatus(userver::server::http::HttpStatus::kMovedPermanently);
  response.SetHeader("Location", original_url.value());
  return {};
}

}
