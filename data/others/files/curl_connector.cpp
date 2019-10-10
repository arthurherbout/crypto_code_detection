/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <sstream>

#include "curl_connector.hpp"
#include "../log.hpp"

using std::string;
using std::stringstream;

using nervana::http::merge_http_paths;

namespace
{
    string address_with_port(const string& address, int port);
}

namespace nervana
{
    curl_connector::curl_connector(const string& address, unsigned int port)
    {
        m_address = address_with_port(address, port);

        // curl_global_init is supposed to be called only once globally
        curl_global_init(CURL_GLOBAL_ALL);
    }

    curl_connector::~curl_connector() { curl_global_cleanup(); }
    http_response curl_connector::get(const string& endpoint, const http_query_t& query)
    {
        CURL* curl_handle = curl_easy_init();
        if (NULL == curl_handle)
        {
            throw std::runtime_error("curl init error");
        }

        // given a url, make an HTTP GET request and fill stream with
        // the body of the response
        stringstream stream;

        string url = merge_http_paths(m_address, endpoint);
        url        = url_with_query(url, query);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &stream);
        curl_easy_setopt(curl_handle, CURLOPT_NOPROXY, "*");

        string call = "[GET] " + url;
        INFO << call;
        CURLcode res = curl_easy_perform(curl_handle);
        check_response(res, call);

        long http_code = get_http_code(curl_handle);

        curl_easy_cleanup(curl_handle);

        return http_response(http_code, stream.str());
    }

    http_response curl_connector::post(const string& endpoint, const string& body)
    {
        CURL* curl_handle = curl_easy_init();
        if (NULL == curl_handle)
        {
            throw std::runtime_error("curl init error");
        }

        // given a url, make an HTTP POST request and fill stream with
        // the body of the response
        stringstream stream;
        stringstream read_stream;
        read_stream.write(body.c_str(), body.size());

        string url = merge_http_paths(m_address, endpoint);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &stream);
        curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl_handle, CURLOPT_READDATA, &read_stream);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, body.size());
        curl_easy_setopt(curl_handle, CURLOPT_NOPROXY, "*");

        string call = "[POST] " + url;
        INFO << call;
        CURLcode res = curl_easy_perform(curl_handle);
        check_response(res, call);

        long http_code = get_http_code(curl_handle);

        curl_easy_cleanup(curl_handle);

        return http_response(http_code, stream.str());
    }

    http_response curl_connector::post(const string& endpoint, const http_query_t& query)
    {
        CURL* curl_handle = curl_easy_init();
        if (NULL == curl_handle)
        {
            throw std::runtime_error("curl init error");
        }

        // given a url, make an HTTP POST request and fill stream with
        // the body of the response
        stringstream stream;
        string       query_string = query_to_string(query);

        string url = merge_http_paths(m_address, endpoint);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &stream);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, query_string.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, query_string.size());
        curl_easy_setopt(curl_handle, CURLOPT_NOPROXY, "*");

        string call = "[POST] " + url;
        INFO << call;
        CURLcode res = curl_easy_perform(curl_handle);
        check_response(res, call);

        long http_code = get_http_code(curl_handle);

        curl_easy_cleanup(curl_handle);

        return http_response(http_code, stream.str());
    }

    http_response curl_connector::del(const string& endpoint, const http_query_t& query)
    {
        CURL* curl_handle = curl_easy_init();
        if (NULL == curl_handle)
        {
            throw std::runtime_error("curl init error");
        }

        // given a url, make an HTTP DELETE request and fill stream with
        // the body of the response
        stringstream stream;

        string url = merge_http_paths(m_address, endpoint);
        url        = url_with_query(url, query);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &stream);
        curl_easy_setopt(curl_handle, CURLOPT_NOPROXY, "*");
        curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");

        string call = "[DELETE] " + url;
        INFO << call;
        CURLcode res = curl_easy_perform(curl_handle);
        check_response(res, call);

        long http_code = get_http_code(curl_handle);

        curl_easy_cleanup(curl_handle);

        return http_response(http_code, stream.str());
    }

    size_t curl_connector::write_callback(void* ptr, size_t size, size_t nmemb, void* stream)
    {
        stringstream& ss = *(stringstream*)stream;
        // callback used by curl.  writes data from ptr into the
        // stringstream passed in to `stream`.

        ss.write((const char*)ptr, size * nmemb);
        return size * nmemb;
    }

    size_t curl_connector::read_callback(void* ptr, size_t size, size_t nmemb, void* stream)
    {
        stringstream& ss = *(stringstream*)stream;
        if (size * nmemb < 1 || ss.eof())
            return 0;

        ss.read(static_cast<char*>(ptr), 1);
        return 1;
    }

    void curl_connector::check_response(CURLcode response, const string& call)
    {
        if (response != CURLE_OK)
        {
            stringstream ss;
            ss << "HTTP call to '" << call << "' failed. ";
            ss << "Curl return: " << curl_easy_strerror(response);

            throw std::runtime_error(ss.str());
        }
    }

    long curl_connector::get_http_code(CURL* curl_handle)
    {
        long http_code = 0;
        curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
        return http_code;
    }

    string curl_connector::url_with_query(const string& url, const nervana::http_query_t& query)
    {
        if (query.empty())
            return url;
        return url + "?" + query_to_string(query);
    }

    string curl_connector::query_to_string(const http_query_t& query)
    {
        stringstream ss;
        bool         first = true;
        for (const auto& item : query)
        {
            if (first)
                first = false;
            else
                ss << "&";
            ss << escape(item.first) << "=" << escape(item.second);
        }
        return ss.str();
    }

    string curl_connector::escape(const string& value)
    {
        CURL* curl_handle = curl_easy_init();
        if (NULL == curl_handle)
        {
            throw std::runtime_error("curl init error");
        }

        char* output = curl_easy_escape(curl_handle, value.c_str(), value.size());
        if (!output)
        {
            throw std::runtime_error("could not escape string: " + value);
        }
        string result{output};
        curl_free(output);
        curl_easy_cleanup(curl_handle);
        return result;
    }
}

namespace
{
    string address_with_port(const string& address, int port)
    {
        int          size = address.size();
        stringstream ss;
        if (size > 0 && address[size - 1] == '/')
        {
            ss << address.substr(0, size - 1);
        }
        else
        {
            ss << address;
        }
        ss << ":" << port;
        return ss.str();
    }
}
