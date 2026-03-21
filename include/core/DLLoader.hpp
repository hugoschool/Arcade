#pragma once

#include <dlfcn.h>
#include <memory>
#include <string>
#include "common/Exception.hpp"

namespace arcade {
    template <typename T>
    class DLLoader {
        public:
            DLLoader() = delete;

            DLLoader(const std::string libraryName) : _libraryName(libraryName), _handle(nullptr)
            {
            };

            ~DLLoader()
            {
                closeHandle();
            };

            void *openHandle()
            {
                _handle = dlopen(_libraryName.c_str(), RTLD_LAZY);
                if (_handle == nullptr)
                    throw arcade::Exception(dlerror());
                return _handle;
            }

            std::unique_ptr<T> getInstance(const std::string functionName)
            {
                if (_handle == nullptr)
                    openHandle();

                T *(*function)() = reinterpret_cast<T *(*)()>(dlsym(_handle, functionName.c_str()));
                if (function == nullptr)
                    throw arcade::Exception(dlerror());

                T *instance = (*function)();
                if (instance == nullptr)
                    throw arcade::Exception("Impossible to find instance");

                return std::unique_ptr<T>(instance);
            };

            void closeHandle()
            {
                if (_handle == nullptr)
                    return;
                if (dlclose(_handle) != 0)
                    throw arcade::Exception(dlerror());

                _handle = nullptr;
            }

        private:
            const std::string _libraryName;
            void *_handle;
    };
}
