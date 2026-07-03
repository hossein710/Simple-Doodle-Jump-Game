#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------
// ResourceManager<T>
// A small generic cache that loads a resource (sf::Texture, sf::Font,
// sf::SoundBuffer, ...) from disk only once and returns a reference to the
// cached instance on every subsequent request. This avoids reloading the
// same file from disk repeatedly and is reused as-is in later phases for
// any new asset type (sounds, fonts, etc.) thanks to the template.
//
// Requirement: T must expose a bool loadFromFile(const std::string&) method,
// which is true for sf::Texture, sf::Font and sf::SoundBuffer alike.
// ---------------------------------------------------------------------------
template <typename T>
class ResourceManager {
public:
    static ResourceManager& instance() {
        static ResourceManager manager;
        return manager;
    }

    T& get(const std::string& filepath) {
        auto it = resources.find(filepath);
        if (it != resources.end()) {
            return *it->second;
        }

        auto resource = std::make_unique<T>();
        if (!resource->loadFromFile(filepath)) {
            throw std::runtime_error("ResourceManager: failed to load resource: " + filepath);
        }

        T& ref = *resource;
        resources.emplace(filepath, std::move(resource));
        return ref;
    }

private:
    ResourceManager() = default;
    std::map<std::string, std::unique_ptr<T>> resources;
};
