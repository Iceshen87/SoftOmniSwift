#include <Include/Execution/Machines/processor.hh>

namespace SoftOmni::Execution::Machines
{
    Processor::Processor(const std::string& name, Harnesses::MachineCode::Commons::MachineCodeFrontend& frontend) :
        machine_code_frontend_(&frontend)
    {
        if (processors_.contains(name))
        {
            throw generate_processor_already_registered_exception(name, frontend);
        }

        processors_.try_emplace(name, this);

        name_ = name;
        display_name_ = name;

        setup_frontend();
    }

    Processor::Processor(std::string name, std::string display_name,
                         Harnesses::MachineCode::Commons::MachineCodeFrontend& frontend) :
        machine_code_frontend_(&frontend)
    {
        if (processors_.contains(name))
        {
            throw generate_processor_already_registered_exception(name, frontend);
        }

        processors_.try_emplace(name, this);

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        setup_frontend();
    }

    const std::string& Processor::name() const
    {
        return name_;
    }

    const std::string& Processor::display_name() const
    {
        return display_name_;
    }

    void Processor::set_display_name(const std::string_view new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Harnesses::MachineCode::Commons::MachineCodeFrontend& Processor::machine_code_frontend() const
    {
        return *machine_code_frontend_;
    }

    Processor::~Processor()
    {
        processors_.erase(name_);

        machine_code_frontend_->processors_.erase(name_);
        machine_code_frontend_->processors_set_.erase(this);
    }

    void Processor::setup_frontend()
    {
        machine_code_frontend_->processors_.try_emplace(name_, this);
        machine_code_frontend_->processors_set_.emplace(this);
    }

    Processor::ProcessorAlreadyRegisteredException::ProcessorAlreadyRegisteredException(
        const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        name_(name), message_(generate_message(name, machine_code_frontend))
    {
    }

    const std::string& Processor::ProcessorAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& Processor::ProcessorAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const Processor& Processor::ProcessorAlreadyRegisteredException::already_registered_processor() const
    {
        return *processors_[name_];
    }

    const char* Processor::ProcessorAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string Processor::ProcessorAlreadyRegisteredException::generate_message(
        const std::string& name,
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const
    {
        return "ERROR: The processor '" + name + "' for the '" + machine_code_frontend.name() +
            "' machine code frontend (for the '" + machine_code_frontend.instruction_set().name() +
            "' instruction set) is already registered as a processor for the '" +
            already_registered_processor().machine_code_frontend().name() + "' machine code frontend ('" +
            already_registered_processor().machine_code_frontend().instruction_set().name() +
            "' instruction set).\nYou cannot have two program processors " +
            "have the same name. They can have the same display name though.";
    }

    Processor::ProcessorAlreadyRegisteredException Processor::generate_processor_already_registered_exception(
        const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        return ProcessorAlreadyRegisteredException(name, machine_code_frontend);
    }
} // namespace SoftOmni::Execution::Machines
