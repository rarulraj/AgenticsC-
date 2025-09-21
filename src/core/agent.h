// src/core/agent.h
#pragma once 

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <any>

namespace repo_refactorer {
    struct Result {
        bool ok = false;
        std::string message;
        std::map<std::string, std::any> data;

        static Result success(const std::string& msg = "") {
            return {true, msg, {}};
        }
        
        static Result failure(const std::string& msg) {
            return {false, msg, {}};
        }
        
        template<typename T>
        void setData(const std::string& key, const T& value) {
            data[key] = value;
        }
        
        template<typename T>
        T getData(const std::string& key) const {
            auto it = data.find(key);
            if (it != data.end()) {
                return std::any_cast<T>(it->second);
            }
            return T{};
        }
    };  // Don't forget the semicolon!
}
