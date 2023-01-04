#pragma once

#include <string>

#include <userver/formats/json_fwd.hpp>

namespace url_shortener::utils {

userver::formats::json::Value Make400(const std::string& code, const std::string& message);

}
