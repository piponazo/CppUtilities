#pragma once

#include "CppUtilities_export.h"

#include <string>
#include <map>
#include <memory>
#include <typeinfo>
#include <cxxabi.h>

namespace ut
{
    template<typename I>
    class EXPORT ExtensibleFactory
    {
    public:
        typedef std::unique_ptr<I> (*CreateCallback)();

        /// \brief Returns the number of registered types
        static std::size_t registeredTypes()
        {
            return m_map.size();
        }

        /// \brief Register a ImageProducer into the factory
        static void registerType(const std::string &type, CreateCallback cb)
        {
            m_map[type] = cb;
        }

        /// \brief Clean all the registerd typed.
        static void unregisterAll()
        {
            return m_map.clear();
        }

        /// \brief Unregister a ImageProducer from the factory
        static void unregister(const std::string &type)
        {
            m_map.erase(type);
        }

        /// \brief Create a ImageProducer (only those registered)
        /// \param type [in] The type of ImageProducer to be created.
        static std::unique_ptr<I> create(const std::string &type);

    private:
        typedef std::map<std::string, CreateCallback> CallbackMap;
        static CallbackMap m_map;
    };

    // instantiate the static variable in ExtensibleFactory
    template<typename I>
    typename ExtensibleFactory<I>::CallbackMap ExtensibleFactory<I>::m_map;

    template<typename I>
    std::unique_ptr<I> ExtensibleFactory<I>::create(const std::string &type)
    {
        auto it = m_map.find(type);

        if(it == m_map.end())
        {
            /// \todo check if that works in windows. If not, use the mangled name there
            int status;
            std::string demangled = abi::__cxa_demangle(typeid(I).name(),0,0,&status);
            throw std::runtime_error(demangled + " - The type (" + type + ") is not registered");
        }

        return (it->second)();
    }

}
