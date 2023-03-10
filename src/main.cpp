#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/daemon_run.hpp>

#include <components/shortener.hpp>
#include <components/unshortener.hpp>

#include <views/api/v1/shorten/view.hpp>
#include <views/u/view.hpp>

#include <utils/postgres.hpp>

int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::clients::dns::Component>()
                            .Append<userver::components::Postgres>(std::string{url_shortener::utils::kPostgres})
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<url_shortener::components::Shortener>()
                            .Append<url_shortener::components::Unshortener>()
                            .Append<url_shortener::views::api::v1::shorten::View>()
                            .Append<url_shortener::views::u::View>();

  return userver::utils::DaemonMain(argc, argv, component_list);
}
