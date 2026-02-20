
#include <Include/Compilation/Commons/program_optimizer.hh>

namespace SoftOmni::Compilation::Commons
{
    ProgramOptimizer::ProgramOptimizer(const std::string& name,
                                       Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        machine_code_frontend_(&machine_code_frontend)
    {
        if (program_optimizers_.contains(name))
        {
            throw generate_programOptimizer_already_registered_exception(name, machine_code_frontend);
        }

        name_ = name;
        display_name_ = name;

        program_optimizers_.try_emplace(name_, this);
        setup_frontend();
    }

    ProgramOptimizer::ProgramOptimizer(std::string name, std::string display_name,
                                       Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        machine_code_frontend_(&machine_code_frontend)
    {
        if (program_optimizers_.contains(name))
        {
            throw generate_programOptimizer_already_registered_exception(name, machine_code_frontend);
        }

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        program_optimizers_.try_emplace(name_, this);
        setup_frontend();
    }

    const std::string& ProgramOptimizer::name() const
    {
        return name_;
    }

    const std::string& ProgramOptimizer::display_name() const
    {
        return display_name_;
    }

    void ProgramOptimizer::set_display_name(const std::string_view& new_display_name)
    {
        display_name_ = new_display_name;
    }

    ProgramOptimizer::~ProgramOptimizer()
    {
        program_optimizers_.erase(name_);

        machine_code_frontend_->program_optimizers_.erase(name_);
        machine_code_frontend_->program_optimizers_set_.erase(this);
    }

    void ProgramOptimizer::setup_frontend()
    {
        machine_code_frontend_->program_optimizers_.try_emplace(name_, this);
        machine_code_frontend_->program_optimizers_set_.emplace(this);
    }

    ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException::ProgramOptimizerAlreadyRegisteredException(
        const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        name_(name), message_(generate_message(name, machine_code_frontend))
    {
    }

    const std::string& ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const ProgramOptimizer&
    ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException::already_existing_program_optimizer() const
    {
        return *program_optimizers_[name_];
    }

    const char* ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException::generate_message(
        const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const
    {
        return "ERROR: The program optimizer '" + name + "' for the '" + machine_code_frontend.name() +
            "' machine code frontend (for the '" + machine_code_frontend.instruction_set().name() +
            "' instruction set) is already registered as a program optimizer for the '" +
            already_existing_program_optimizer().machine_code_frontend().name() + "' machine code frontend ('" +
            already_existing_program_optimizer().machine_code_frontend().instruction_set().name() +
            "' instruction set).\nYou cannot have two program optimizers " +
            "have the same name. They can have the same display name though.";
    }

    ProgramOptimizer::ProgramOptimizerAlreadyRegisteredException
    ProgramOptimizer::generate_programOptimizer_already_registered_exception(
        const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        return ProgramOptimizerAlreadyRegisteredException(name, machine_code_frontend);
    }
} // namespace SoftOmni::Compilation::Commons
