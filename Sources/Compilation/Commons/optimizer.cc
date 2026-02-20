
#include <Include/Compilation/Commons/optimizer.hh>


namespace SoftOmni::Compilation::Commons
{
    Optimizer::Optimizer(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& frontend) :
        frontend_(&frontend)
    {
        if (optimizers_.contains(name))
        {
            throw generate_optimizer_already_registered_exception(name, frontend);
        }

        name_ = name;
        display_name_ = name;

        optimizers_.try_emplace(name_, this);
        setup_frontend();
    }

    Optimizer::Optimizer(std::string name, std::string display_name,
                         Parsing::Commons::Kernels::LanguageFrontend& frontend) : frontend_(&frontend)
    {
        if (optimizers_.contains(name))
        {
            throw generate_optimizer_already_registered_exception(name, frontend);
        }

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        optimizers_.try_emplace(name_, this);
        setup_frontend();
    }

    const std::string& Optimizer::name() const
    {
        return name_;
    }

    const std::string& Optimizer::display_name() const
    {
        return display_name_;
    }

    void Optimizer::set_display_name(const std::string_view& new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Parsing::Commons::Kernels::LanguageFrontend& Optimizer::frontend() const
    {
        return *frontend_;
    }

    Optimizer::~Optimizer()
    {
        optimizers_.erase(name_);

        frontend_->optimizers_.erase(name_);
        frontend_->optimizers_set_.erase(this);
    }

    void Optimizer::setup_frontend()
    {
        frontend_->optimizers_.try_emplace(name_, this);
        frontend_->optimizers_set_.emplace(this);
    }

    Optimizer::OptimizerAlreadyRegisteredException::OptimizerAlreadyRegisteredException(const std::string& name,
        const Parsing::Commons::Kernels::LanguageFrontend& frontend) :
        name_(name), message_(generate_message(name, frontend))
    {
    }

    const std::string& Optimizer::OptimizerAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& Optimizer::OptimizerAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const Optimizer& Optimizer::OptimizerAlreadyRegisteredException::already_existing_optimizer() const
    {
        return *optimizers_[name_];
    }

    const char* Optimizer::OptimizerAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string Optimizer::OptimizerAlreadyRegisteredException::generate_message(
        const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend) const
    {
        return "ERROR: The optimizer '" + name + "' for the '" + frontend.name() + "' language frontend (for the '" +
            frontend.language().name() + "' language) is already registered as an optimizer for the '" +
            already_existing_optimizer().frontend().name() + "' language frontend ('" +
            already_existing_optimizer().frontend().language().name() +
            "' language).\nYou cannot have two optimizers " +
            "have the same name. They can have the same display name though.";
    }

    Optimizer::OptimizerAlreadyRegisteredException
    Optimizer::generate_optimizer_already_registered_exception(const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend)
    {
        return OptimizerAlreadyRegisteredException(name, frontend);
    }
} // namespace SoftOmni::Compilation::Commons
