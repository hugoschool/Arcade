#pragma once

#include "core/DLLoader.hpp"
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace arcade {
    template<typename T>
    class LibraryManager {
        public:
            LibraryManager() = delete;

            LibraryManager(const std::string entrypoint) : _entrypoint(entrypoint),
                _loader(std::nullopt), _ptr(nullptr),
                _libraries(), _index(0)
            {
                getAllLibraries();
                modifyPointer();
            }

            ~LibraryManager()
            {
            }

            std::shared_ptr<T> getPreviousInstance()
            {
                if (_index != 0)
                    _index--;
                modifyPointer();
                return _ptr;
            }

            std::shared_ptr<T> getNextInstance()
            {
                _index++;
                modifyPointer();
                return _ptr;
            }

            std::shared_ptr<T> getPointer()
            {
                return _ptr;
            }

        private:
            const std::string _entrypoint;
            std::optional<DLLoader<T>> _loader;
            std::shared_ptr<T> _ptr;

            std::vector<std::string> _libraries;
            std::size_t _index;

            void modifyPointer()
            {
                _ptr = nullptr;

                _loader = DLLoader<T>(_libraries[_index % _libraries.size()]);

                std::shared_ptr<T> newPtr = _loader->getInstance(_entrypoint);

                _ptr.swap(newPtr);
            }

            void getAllLibraries()
            {
                const std::string LIBRARY_PATH = "./lib";

                for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(LIBRARY_PATH)) {
                    const std::string libraryName = entry.path().string();

                    DLLoader<T> loader(libraryName);

                    try {
                        loader.openHandle();
                        if (loader.symbolExists(_entrypoint) == true) {
                            _libraries.push_back(libraryName);
                        }
                    } catch (const std::exception &) {
                        continue;
                    }
                }
            }
    };
}
