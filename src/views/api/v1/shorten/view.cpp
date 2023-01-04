#include "view.hpp"

#include <userver/components/component_context.hpp>

#include <userver/formats/json.hpp>

#include <db/urls.hpp>

#include <utils/http.hpp>

namespace url_shortener::views::api::v1::shorten {

namespace {

userver::formats::json::Value Make200(const std::string& original_url, const std::string& shorten_url) {
  userver::formats::json::ValueBuilder result;
  result["original_url"] = original_url;
  result["shorten_url"] = shorten_url;
  return result.ExtractValue();
}

}

View::View(
  const userver::components::ComponentConfig& config,
  const userver::components::ComponentContext& context
)
  : HttpHandlerJsonBase{config, context}
  , shortener_{context.FindComponent<components::Shortener>()}
{}

userver::formats::json::Value View::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  auto& response = request.GetHttpResponse();

  std::string original_url{};
  try {
    original_url = request_json["original_url"].As<std::string>();
  } catch (const std::exception& e) {
    response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    return utils::Make400("BAD_REQUEST", "Failed to parse request body");
  }

  const auto short_url = shortener_.MakeShortUrl(original_url);

  response.SetStatusOk();
  return Make200(original_url, short_url);
}

}
