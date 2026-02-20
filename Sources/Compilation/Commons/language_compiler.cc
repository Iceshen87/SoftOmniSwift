
#include <Include/Compilation/Commons/language_compiler.hh>
#include <utility>

namespace SoftOmni::Compilation::Commons
{
    LanguageCompiler::LanguageCompiler(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& source,
                                       Parsing::Commons::Kernels::LanguageFrontend& target) :
        source_(&source), target_(&target)
    {
        if (source_ == target_)
        {
            throw generate_language_compiler_for_same_frontend_exception(source);
        }

        if (compilers_.contains(name))
        {
            throw generate_compiler_already_registered_exception(name);
        }

        compilers_.try_emplace(name, this);

        name_ = name;
        display_name_ = name_;

        setup_frontends();
    }

    LanguageCompiler::LanguageCompiler(std::string name, std::string display_name,
                                       Parsing::Commons::Kernels::LanguageFrontend& source,
                                       Parsing::Commons::Kernels::LanguageFrontend& target) :
        source_(&source), target_(&target)
    {
        if (source_ == target_)
        {
            throw generate_language_compiler_for_same_frontend_exception(source);
        }

        if (compilers_.contains(name))
        {
            throw generate_compiler_already_registered_exception(name);
        }

        compilers_.try_emplace(name, this);

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        setup_frontends();
    }

    const std::string& LanguageCompiler::name() const
    {
        return name_;
    }

    const std::string& LanguageCompiler::display_name() const
    {
        return display_name_;
    }

    void LanguageCompiler::set_display_name(const std::string_view& new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Parsing::Commons::Kernels::LanguageFrontend& LanguageCompiler::source_frontend() const
    {
        return *source_;
    }

    const Parsing::Commons::Kernels::LanguageFrontend& LanguageCompiler::target_frontend() const
    {
        return *target_;
    }

    LanguageCompiler::~LanguageCompiler()
    {
        compilers_.erase(name_);

        target_->language_frontends_capable_of_targeting_[source_].erase(this);
        if (target_->language_frontends_capable_of_targeting_[source_].empty())
        {
            target_->language_frontends_capable_of_targeting_.erase(source_);
        }

        target_->languages_capable_of_targeting_[source_->language_].erase(this);
        if (target_->languages_capable_of_targeting_[source_->language_].empty())
        {
            target_->languages_capable_of_targeting_.erase(source_->language_);
        }

        target_->targeting_language_compilers_.erase(name_);
        target_->inbound_language_compilers_.erase(this);

        source_->capable_language_frontend_targets_.erase(target_);
        if (source_->capable_language_frontend_targets_.empty())
        {
            source_->capable_language_frontend_targets_.erase(target_);
        }

        source_->capable_language_targets_[target_->language_].erase(this);
        if (source_->capable_language_targets_[target_->language_].empty())
        {
            source_->capable_language_targets_.erase(target_->language_);
        }

        source_->source_language_compilers_.erase(name_);
        source_->outbound_language_compilers_.erase(this);
    }

    void LanguageCompiler::setup_frontends()
    {
        Parsing::Commons::Kernels::Language* source_language = source_->language_;

        if (target_->language_frontends_capable_of_targeting_.contains(source_))
        {
            target_->language_frontends_capable_of_targeting_[source_].emplace(this);
            target_->languages_capable_of_targeting_[source_language].emplace(this);
        }
        else
        {
            auto target_frontend_compilers = std::unordered_set<LanguageCompiler*>();
            target_frontend_compilers.emplace(this);

            target_->language_frontends_capable_of_targeting_[source_] = target_frontend_compilers;
            if (target_->languages_capable_of_targeting_.contains(source_language))
            {
                target_->languages_capable_of_targeting_[source_language].emplace(this);
            }
            else
            {
                auto target_language_compilers = std::unordered_set<LanguageCompiler*>();
                target_language_compilers.emplace(this);

                target_->languages_capable_of_targeting_[source_language] = target_language_compilers;
            }
        }

        target_->targeting_language_compilers_.try_emplace(name_, this);
        target_->inbound_language_compilers_.emplace(this);

        Parsing::Commons::Kernels::Language* target_language = target_->language_;

        if (source_->capable_language_frontend_targets_.contains(target_))
        {
            source_->capable_language_frontend_targets_[target_].emplace(this);
            source_->capable_language_targets_[target_language].emplace(this);
        }
        else
        {
            auto source_frontend_compilers = std::unordered_set<LanguageCompiler*>();
            source_frontend_compilers.emplace(this);

            source_->capable_language_frontend_targets_[target_] = source_frontend_compilers;
            if (source_->capable_language_targets_.contains(target_language))
            {
                source_->capable_language_targets_[target_language].emplace(this);
            }
            else
            {
                auto source_language_compilers = std::unordered_set<LanguageCompiler*>();
                source_language_compilers.emplace(this);

                source_->capable_language_targets_[target_language] = source_language_compilers;
            }
        }

        source_->source_language_compilers_.try_emplace(name_, this);
        source_->outbound_language_compilers_.emplace(this);
    }

    LanguageCompiler::LanguageCompilerAlreadyRegisteredException
    LanguageCompiler::generate_compiler_already_registered_exception(const std::string& name)
    {
        return LanguageCompilerAlreadyRegisteredException(name);
    }

    LanguageCompiler::LanguageCompilerForSameFrontendException
    LanguageCompiler::generate_language_compiler_for_same_frontend_exception(
        const Parsing::Commons::Kernels::LanguageFrontend& frontend)
    {
        return LanguageCompilerForSameFrontendException(frontend);
    }

    LanguageCompiler::LanguageCompilerAlreadyRegisteredException::LanguageCompilerAlreadyRegisteredException(
        const std::string& name) : name_(name), message_(generate_message(name))
    {
    }

    const std::string& LanguageCompiler::LanguageCompilerAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& LanguageCompiler::LanguageCompilerAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const LanguageCompiler&
    LanguageCompiler::LanguageCompilerAlreadyRegisteredException::get_already_existing_language_compiler() const
    {
        return *compilers_[name_];
    }

    const char* LanguageCompiler::LanguageCompilerAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string LanguageCompiler::LanguageCompilerAlreadyRegisteredException::generate_message(const std::string& name)
    {
        const LanguageCompiler* compiler = compilers_[name];
        return "ERROR: The language compiler '" + name + "' which compiles from " +
            compiler->source_->language_->name() + " (" + compiler->source_->name_ + " language frontend) to " +
            compiler->target_->language_->name() + " (" + compiler->target_->name_ +
            " language frontend) is already registered";
    }

    LanguageCompiler::LanguageCompilerForSameFrontendException::LanguageCompilerForSameFrontendException(
        const Parsing::Commons::Kernels::LanguageFrontend& frontend) : message_(generate_message(frontend))
    {
    }

    const std::string& LanguageCompiler::LanguageCompilerForSameFrontendException::message() const
    {
        return message_;
    }

    const char* LanguageCompiler::LanguageCompilerForSameFrontendException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string LanguageCompiler::LanguageCompilerForSameFrontendException::generate_message(
        const Parsing::Commons::Kernels::LanguageFrontend& source)
    {
        return "ERROR: Cannot compile from the frontend '" + source.name_ + "' (for the '" + source.language().name() +
            "' language) to the same exact frontend. Compiling from the same language but between 2 different "
            "frontends is allowed. Use an a Compilation::Commons::Optimizer for optimizing the code of a frontend.";
    }
} // namespace SoftOmni::Compilation::Commons
