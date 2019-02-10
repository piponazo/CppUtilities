#pragma once

#include "utilities_export.h"

#include <string>
#include <map>
#include <memory>
#include <typeinfo>
#ifdef __linux__
#include <cxxabi.h>
#endif

namespace ut
{
    template<typename I>
    class UTILITIES_EXPORT ExtensibleFactory
    {
    public:
        typedef std::unique_ptr<I> (*CreateCallback)();

        static ExtensibleFactory<I> & instance();

        /// \brief Returns the number of registered types
        std::size_t registeredTypes() const
        {
            return m_map.size();
        }

        /// \brief Register a ImageProducer into the factory
        void registerType(const std::string &type, CreateCallback cb)
        {
            m_map[type] = cb;
        }

        /// \brief Clean all the registerd typed.
        void unregisterAll()
        {
            return m_map.clear();
        }

        /// \brief Unregister a ImageProducer from the factory
        void unregister(const std::string &type)
        {
            m_map.erase(type);
        }

        /// \brief Create a ImageProducer (only those registered)
        /// \param type [in] The type of ImageProducer to be created.
        std::unique_ptr<I> create(const std::string &type);

    private:
        typedef std::map<std::string, CreateCallback> CallbackMap;
        CallbackMap m_map;
    };

    template<typename I>
    ExtensibleFactory<I> & ExtensibleFactory<I>::instance()
    {
        static ExtensibleFactory<I> instance;
        return instance;
    }

    template<typename I>
    std::unique_ptr<I> ExtensibleFactory<I>::create(const std::string &type)
    {
        auto it = m_map.find(type);

        if(it == m_map.end())
        {
#ifdef __linux__
            /// \todo check if that works in windows. If not, use the mangled name there
            int status;
            std::string demangled = abi::__cxa_demangle(typeid(I).name(),0,0,&status);
            throw std::runtime_error(demangled + " - The type (" + type + ") is not registered");
#endif
        }

        return (it->second)();
    }

}
