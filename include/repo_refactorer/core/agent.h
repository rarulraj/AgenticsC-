#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <any>

namespace repo_refactorer {

// Forward declarations
class Context;

/**
 * @brief Result of an agent operation
 */
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
};

/**
 * @brief Base interface for all agents
 */
class IAgent {
public:
    virtual ~IAgent() = default;
    
    /**
     * @brief Execute one step of the agent's work
     * @param ctx Current context
     * @return Result of the operation
     */
    virtual Result tick(const Context& ctx) = 0;
    
    /**
     * @brief Get the agent's name
     * @return Agent name
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief Check if the agent is ready to run
     * @param ctx Current context
     * @return True if ready
     */
    virtual bool isReady(const Context& ctx) const = 0;
    
    /**
     * @brief Reset the agent to initial state
     */
    virtual void reset() = 0;
};

} // namespace repo_refactorer
