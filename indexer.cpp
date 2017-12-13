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
        : token_("Splunk " + token), poller_(poller), client_(factory.create(base_uri)), factory_(factory) {
    auto uuid{boost::uuids::random_generator()()};
    string channel{boost::uuids::to_string(uuid)};
    swap(channel_, channel);
}

void Indexer::send(const shared_ptr<EventBatch>& batch) const {
    Indexer* indexer = const_cast<Indexer*>(this);
    try {
        string resp = post(batch->rest_endpoint(), batch->serialize(), batch->content_type());
        poller_->add(indexer->shared_from_this(), batch, resp);
    } catch (const HecException& ex) {
        poller_->fail(indexer->shared_from_this(), batch, ex);
    }
}

string Indexer::post(const string& uri, const vector<unsigned char>& data, const string& content_type) const {
    http_request req{methods::POST};
    auto &headers = req.headers();
    headers.set_content_type(content_type);
    headers.add("X-Splunk-Request-Channel", channel_);
    headers.add("Authorization", token_);
    req.set_body(data);
    req.set_request_uri(uri);

    string resp;
    bool has_exception = false;
    HecException except("", 0);
    pplx::task<void> res = client_->request(req).then([&](pplx::task<http_response> t) {
        Indexer *indexer = const_cast<Indexer *>(this);
        try {
            http_response response = t.get();
            resp = response.extract_string().get();
        } catch (const http_exception& e) {
            HecException ex(e.what(), e.error_code().value());
            except = ex;
            has_exception = true;
        } catch (const exception &e) {
            HecException ex(e.what(), -1);
            except = ex;
            has_exception = true;
        }
    });
    res.get();

    if (has_exception) {
        throw except;
    }

    return resp;
}

} // namespace splunkhec

namespace std {
    template<> struct hash<splunkhec::Indexer> {
        std::size_t operator()(const splunkhec::Indexer& indexer) const {
            return std::hash<std::string>{}(indexer.channel());
        }
    };
} // namespace std
