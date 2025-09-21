#pragma once

#include <string>
#include <filesystem>
#include <memory>
#include <map>
#include <any>

namespace repo_refactorer {

// Forward declarations
class ConfigManager;
class KnowledgeBase;

/**
 * @brief Global context shared across all agents
 */
class Context {
public:
    Context() = default;
    ~Context() = default;
    
    // Non-copyable but movable
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = default;
    Context& operator=(Context&&) = default;
    
    /**
     * @brief Create context from configuration file
     * @param configPath Path to YAML configuration
     * @return Context instance
     */
    static Context fromConfig(const std::string& configPath);
    
    // Core properties
    std::filesystem::path repoRoot;
    std::filesystem::path buildDir;
    std::filesystem::path outputDir;
    std::filesystem::path logDir;
    
    // Configuration
    std::shared_ptr<ConfigManager> config;
    
    // Knowledge base
    std::shared_ptr<KnowledgeBase> knowledge;
    
    // Runtime data
    std::map<std::string, std::any> runtimeData;
    
    /**
     * @brief Set runtime data
     * @param key Data key
     * @param value Data value
     */
    template<typename T>
    void setData(const std::string& key, const T& value) {
        runtimeData[key] = value;
    }
    
    /**
     * @brief Get runtime data
     * @param key Data key
     * @return Data value or default constructed T
     */
    template<typename T>
    T getData(const std::string& key) const {
        auto it = runtimeData.find(key);
        if (it != runtimeData.end()) {
            return std::any_cast<T>(it->second);
        }
        return T{};
    }
    
    /**
     * @brief Check if data exists
     * @param key Data key
     * @return True if data exists
     */
    bool hasData(const std::string& key) const {
        return runtimeData.find(key) != runtimeData.end();
    }
};

} // namespace repo_refactorer
