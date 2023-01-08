#include "http.hpp"

#include <userver/formats/json.hpp>

namespace url_shortener::utils {

userver::formats::json::Value Make400(const std::string& code, const std::string& message) {
  userver::formats::json::ValueBuilder result;
  result["code"] = code;
  result["message"] = message;
  return result.ExtractValue();
}

}
