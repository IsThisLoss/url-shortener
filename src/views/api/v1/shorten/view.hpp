#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>

#include <components/shortener.hpp>

namespace url_shortener::views::api::v1::shorten {

class View final : public userver::server::handlers::HttpHandlerJsonBase {
public:
  static constexpr std::string_view kName = "handler-api_v1_shorten";

  explicit View(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
  );

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      userver::server::request::RequestContext& context) const override;

private:
  const components::Shortener& shortener_;
};

}
