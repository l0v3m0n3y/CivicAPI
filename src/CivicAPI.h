#ifndef CIVIC_API_H
#define CIVIC_API_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <map>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class CivicAPI {
private:
    std::string api_base = "https://civicapi.org/api/v2";
    http_client_config client_config;
    
    http_request create_request(const std::string& path, method m) {
        http_request request(m);
        request.headers().add("Connection", "keep-alive");
        request.headers().add("User-Agent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36");
        request.headers().add("Host", "civicapi.org");
        request.headers().add("Content-Type", "application/json");
        request.headers().add("Accept", "application/json");
        request.set_request_uri(utility::conversions::to_string_t(path));
        return request;
    }

    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                query += param.first + "=" + param.second;
                first = false;
            }
        }
        return query;
    }

public:
    // Поиск выборов
    pplx::task<json::value> search_races(
        const std::string& startDate = "",
        const std::string& endDate = "",
        const std::string& query = "",
        const std::string& country = "",
        const std::string& province = "",
        const std::string& district = "",
        const std::string& election_type = "",
        int limit = 0) {
        
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        
        std::map<std::string, std::string> params;
        if (!startDate.empty()) params["startDate"] = startDate;
        if (!endDate.empty()) params["endDate"] = endDate;
        if (!query.empty()) params["query"] = query;
        if (!country.empty()) params["country"] = country;
        if (!province.empty()) params["province"] = province;
        if (!district.empty()) params["district"] = district;
        if (!election_type.empty()) params["election_type"] = election_type;
        if (limit > 0) params["limit"] = std::to_string(limit);
        
        std::string path = "/race/search" + build_query_params(params);
        auto request = create_request(path, methods::GET);
        
        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: " + std::to_string(response.status_code())));
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: " + std::string(e.what())));
                    return error_obj;
                }
            });
    }

    // Получение информации о конкретных выборах
    pplx::task<json::value> get_race(
        const std::string& raceid,
        bool generateMap = false,
        bool generateMapPNG = false,
        const std::string& data = "",
        bool embed = false,
        bool precinct = false) {
        
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        
        std::map<std::string, std::string> params;
        if (generateMap) params["generateMap"] = "true";
        if (generateMapPNG) params["generateMapPNG"] = "true";
        if (!data.empty()) params["data"] = data;
        if (embed) params["embed"] = "true";
        if (precinct) params["precinct"] = "true";
        
        std::string path = "/race/" + raceid + build_query_params(params);
        auto request = create_request(path, methods::GET);
        
        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: " + std::to_string(response.status_code())));
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: " + std::string(e.what())));
                    return error_obj;
                }
            });
    }

    // Получение исторических данных выборов
    pplx::task<json::value> get_race_history(
        const std::string& raceid,
        const std::string& timestamp) {
        
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        
        std::string path = "/race/" + raceid + "/history/" + timestamp;
        auto request = create_request(path, methods::GET);
        
        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: " + std::to_string(response.status_code())));
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: " + std::string(e.what())));
                    return error_obj;
                }
            });
    }

    // Получение дат выборов
    pplx::task<json::value> get_election_dates(
        const std::string& country = "",
        const std::string& province = "",
        const std::string& year = "") {
        
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        
        std::map<std::string, std::string> params;
        if (!country.empty()) params["country"] = country;
        if (!province.empty()) params["province"] = province;
        if (!year.empty()) params["year"] = year;
        
        std::string path = "/getElectionDates" + build_query_params(params);
        auto request = create_request(path, methods::GET);
        
        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: " + std::to_string(response.status_code())));
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: " + std::string(e.what())));
                    return error_obj;
                }
            });
    }
};

#endif
