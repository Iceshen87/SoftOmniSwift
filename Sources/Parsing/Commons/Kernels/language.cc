
#include <Include/Parsing/Commons/Kernels/language.hh>

#include <format>
#include <utility>

namespace SoftOmni::Parsing::Commons::Kernels
{
    Language::Language(const std::string& name)
    {
        setup_language(name);

        display_name_ = name;
        reference_frontend_ = std::optional<std::reference_wrapper<LanguageFrontend>>();
    }

    Language::Language(const std::string& name, std::string display_name)
    {
        setup_language(name);

        display_name_ = std::move(display_name);
        reference_frontend_ = std::optional<std::reference_wrapper<LanguageFrontend>>();
    }

    const std::string& Language::name() const
    {
        return name_;
    }

    const std::string& Language::display_name() const
    {
        return display_name_;
    }

    void Language::set_display_name(std::string new_display_name)
    {
        display_name_ = std::move(new_display_name);
    }

    Language::~Language()
    {
        language_frontends_.clear();
        reference_frontend_.reset();

        languages_.erase(name_);
    }

    Language::Language(Language&& other) noexcept :
        name_(std::move(other.name_)), display_name_(std::move(other.display_name_)),
        language_frontends_(std::move(other.language_frontends_)), reference_frontend_(other.reference_frontend_)
    {
    }

    Language& Language::operator=(const Language& other)
    {
        if (this == &other)
            return *this;
        name_ = other.name_;
        display_name_ = other.display_name_;
        language_frontends_ = other.language_frontends_;
        reference_frontend_ = other.reference_frontend_;
        return *this;
    }

    Language& Language::operator=(Language&& other) noexcept
    {
        if (this == &other)
            return *this;
        name_ = std::move(other.name_);
        display_name_ = std::move(other.display_name_);
        language_frontends_ = std::move(other.language_frontends_);
        reference_frontend_ = other.reference_frontend_;
        return *this;
    }

    std::optional<std::reference_wrapper<LanguageFrontend>> Language::operator[](const std::string& name) const
    {
        const auto language_frontend_iterator = language_frontends_.find(name);

        if (language_frontend_iterator == language_frontends_.end() || !language_frontend_iterator->second)
        {
            return std::nullopt;
        }

        return std::ref(*language_frontend_iterator->second);
    }

    std::optional<std::reference_wrapper<Language>> Language::get(const std::string& name)
    {
        if (!languages_.contains(name))
        {
            return std::nullopt;
        }

        return std::ref(*languages_[name]);
    }

    bool Language::operator==(const std::string_view& rhs) const
    {
        return name_ == rhs;
    }

    Language::LanguageAlreadyRegisteredException::LanguageAlreadyRegisteredException(std::string name) :
        name_(std::move(name))
    {
        message_ = generate_message(name_);
    }

    const std::string& Language::LanguageAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& Language::LanguageAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const char* Language::LanguageAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    const Language& Language::LanguageAlreadyRegisteredException::get_already_existing_language() const
    {
        return *languages_[name_];
    }

    std::string Language::LanguageAlreadyRegisteredException::generate_message(const std::string& name)
    {
        return "The language " + name + " is already registered.\n" + "Language names must be unique.\n" +
            "You may have two registered languages have the same display names but never the same name.";
    }

    void Language::setup_language(const std::string& name)
    {
        if (languages_.contains(name))
        {
            throw generate_language_already_registered_exception(name);
        }

        name_ = name;

        language_frontends_ = std::unordered_map<std::string, std::unique_ptr<LanguageFrontend>>();
        languages_.try_emplace(name, this);
    }

    Language::LanguageAlreadyRegisteredException
    Language::generate_language_already_registered_exception(const std::string& name)
    {
        return LanguageAlreadyRegisteredException(name);
    }
} // namespace SoftOmni::Parsing::Commons::Kernels
