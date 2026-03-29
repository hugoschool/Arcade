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

            LibraryManager(const std::string entrypoint, const std::string initialLibrary = "") :
                _entrypoint(entrypoint), _initialLibrary(initialLibrary),
                _loader(std::nullopt), _ptr(nullptr),
                _libraries(), _index(0)
            {
                getAllLibraries();

                // Make sure the initial library is in the vector
                if (!_initialLibrary.empty()) {
                    const std::string initialLibraryFilename = std::filesystem::path(_initialLibrary).filename();
                    bool found = false;

                    for (const std::string &lib : _libraries) {
                        const std::string filename = std::filesystem::path(lib).filename();
                        if (filename == initialLibraryFilename) {
                            found = true;
                            break;
                        }
                    }

                    if (found == false)
                        throw arcade::Exception("Couldn't find " + initialLibraryFilename + " in the libraries");
                }

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
            const std::string _initialLibrary;

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
                    const std::filesystem::path libraryPath = entry.path();
                    const std::string libraryName = libraryPath.string();

                    DLLoader<T> loader(libraryName);

                    try {
                        loader.openHandle();
                        if (loader.symbolExists(_entrypoint) == true) {
                            if (!_initialLibrary.empty() &&
                                libraryPath.filename() == std::filesystem::path(_initialLibrary).filename())
                                _libraries.insert(_libraries.begin(), libraryName);
                            else
                                _libraries.push_back(libraryName);
                        }
                    } catch (const std::exception &) {
                        continue;
                    }
                }
            }
    };
}
