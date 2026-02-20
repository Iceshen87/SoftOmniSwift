
#include <Include/Execution/Commons/language_runtime.hh>

#include "ArgumentExceptions/argument_null_exception.hh"

namespace SoftOmni::Execution::Commons
{
    LanguageRuntime::LanguageRuntime(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& frontend) :
        frontend_(&frontend)
    {
        if (language_runtimes_.contains(name))
        {
            throw generate_language_runtime_already_registered_exception(name, frontend);
        }

        language_runtimes_.try_emplace(name, this);

        name_ = name;
        display_name_ = name;

        setup_frontend();
    }

    LanguageRuntime::LanguageRuntime(std::string name, std::string display_name,
                                     Parsing::Commons::Kernels::LanguageFrontend& frontend) : frontend_(&frontend)
    {
        if (language_runtimes_.contains(name))
        {
            throw generate_language_runtime_already_registered_exception(name, frontend);
        }

        language_runtimes_.try_emplace(name, this);

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        setup_frontend();
    }

    const std::string& LanguageRuntime::name() const
    {
        return name_;
    }

    const std::string& LanguageRuntime::display_name() const
    {
        return display_name_;
    }

    void LanguageRuntime::set_display_name(const std::string_view new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Parsing::Commons::Kernels::LanguageFrontend& LanguageRuntime::frontend() const
    {
        return *frontend_;
    }

    LanguageRuntime::~LanguageRuntime()
    {
        language_runtimes_.erase(name_);

        frontend_->runtimes_.erase(name_);
        frontend_->runtimes_set_.erase(this);
    }

    void LanguageRuntime::setup_frontend()
    {
        frontend_->runtimes_.try_emplace(name_, this);
        frontend_->runtimes_set_.emplace(this);
    }

    LanguageRuntime::LanguageRuntimeAlreadyRegisteredException::LanguageRuntimeAlreadyRegisteredException(
        const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend) :
        name_(name), message_(generate_message(name, frontend))
    {
    }

    const std::string& LanguageRuntime::LanguageRuntimeAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& LanguageRuntime::LanguageRuntimeAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const LanguageRuntime&
    LanguageRuntime::LanguageRuntimeAlreadyRegisteredException::already_registered_language_runtime() const
    {
        return *language_runtimes_[name_];
    }

    const char* LanguageRuntime::LanguageRuntimeAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string LanguageRuntime::LanguageRuntimeAlreadyRegisteredException::generate_message(
        const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend) const
    {
        return "ERROR: The language runtime '" + name + "' for the '" + frontend.name() + "' language frontend (for the '" +
            frontend.language().name() + "' language) is already registered as a language runtime for the '" +
            already_registered_language_runtime().frontend().name() + "' language frontend ('" +
            already_registered_language_runtime().frontend().language().name() +
            "' language).\nYou cannot have two language runtimes " +
            "have the same name. They can have the same display name though.";
    }

    LanguageRuntime::LanguageRuntimeAlreadyRegisteredException
    LanguageRuntime::generate_language_runtime_already_registered_exception(
        const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend)
    {
        return LanguageRuntimeAlreadyRegisteredException(name, frontend);
    }
} // namespace SoftOmni::Execution::Commons
