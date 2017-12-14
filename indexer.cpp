//
// Created by kchen on 12/3/17.
//

#include "indexer.h"
#include "poller_inf.h"
#include "hec_exception.h"

#include <functional>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace std;
using namespace web::http;
using namespace web::http::client;

namespace splunkhec {

Indexer::Indexer(const string& base_uri, const string& token, const shared_ptr<PollerInf>& poller, const HttpClientFactory& factory)
        : uri_(base_uri), token_("Splunk " + token), poller_(poller), client_(factory.create(base_uri)), factory_(factory) {
    auto uuid{boost::uuids::random_generator()()};
    string channel{boost::uuids::to_string(uuid)};
    swap(channel_, channel);
}

bool Indexer::send(const shared_ptr<EventBatch>& batch) {
    Indexer* indexer = const_cast<Indexer*>(this);
    try {
        string resp = post(batch->rest_endpoint(), batch->serialize(), batch->content_type());
        poller_->add(indexer->shared_from_this(), batch, resp);
    } catch (const HecException& ex) {
        poller_->fail(indexer->shared_from_this(), batch, ex);
        return false;
    }
    return true;
}

string Indexer::post(const string& uri, const vector<unsigned char>& data, const string& content_type) {
    http_request req{methods::POST};
    auto &headers = req.headers();
    headers.set_content_type(content_type);
    headers.add("X-Splunk-Request-Channel", channel_);
    headers.add("Authorization", token_);
    req.set_body(data);
    req.set_request_uri(uri);

    string resp;
    HecException except("", 0);

    pplx::task<void> res = client_->request(req).then([&](pplx::task<http_response> t) {
        try {
            http_response response = t.get();
            auto r = response.extract_string().get();

            auto code = response.status_code();
            if (code == 503) {
                // server busy
                log_backpressure();
            }

            if (code != 200 && code != 201) {
                except = HecException(r, code);
            } else {
                resp = r;
            }
        } catch (const http_exception& e) {
            log_backpressure();
            HecException ex(e.what(), e.error_code().value());
            except = ex;
        } catch (const exception &e) {
            log_backpressure();
            HecException ex(e.what(), -1);
            except = ex;
        }
    });
    res.get();

    if (except.error_code() != 0) {
        throw except;
    }

    clear_backpressure();

    return resp;
}

bool Indexer::has_backpressure() const {
    if (backpressure_ > 0) {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<std::chrono::seconds>(now - last_backpressure_time_);
        if (elapsed < backpressure_window_) {
            return true;
        }

        // passed the backpressure window
        Indexer* self = const_cast<Indexer*>(this);
        self->clear_backpressure();
        return false;
    }

    return false;
}

const chrono::seconds Indexer::backpressure_window_ = chrono::seconds{60};

} // namespace splunkhec
