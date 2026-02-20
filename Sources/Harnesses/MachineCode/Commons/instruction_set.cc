
#include <Include/Harnesses/MachineCode/Commons/instruction_set.hh>

namespace SoftOmni::Harnesses::MachineCode::Commons
{
    InstructionSet::InstructionSet(const std::string& name)
    {
        setup_instruction_set(name);

        display_name_ = name;
        reference_machine_code_frontend_ = std::optional<std::reference_wrapper<MachineCodeFrontend>>();
    }

    InstructionSet::InstructionSet(const std::string& name, std::string display_name)
    {
        setup_instruction_set(name);

        display_name_ = std::move(display_name);
        reference_machine_code_frontend_ = std::optional<std::reference_wrapper<MachineCodeFrontend>>();
    }

    const std::string& InstructionSet::name() const
    {
        return name_;
    }

    const std::string& InstructionSet::display_name() const
    {
        return display_name_;
    }

    void InstructionSet::set_display_name(std::string new_display_name)
    {
        display_name_ = std::move(new_display_name);
    }

    std::optional<std::reference_wrapper<MachineCodeFrontend>> InstructionSet::operator[](const std::string& name) const
    {
        const auto machine_code_frontend_iterator = machine_code_frontends_.find(name);
        if (machine_code_frontend_iterator == machine_code_frontends_.end() || !machine_code_frontend_iterator->second)
        {
            return std::nullopt;
        }

        return std::ref(*machine_code_frontend_iterator->second);
    }

    std::optional<std::reference_wrapper<InstructionSet>> InstructionSet::get(const std::string& name)
    {
        if (!instruction_sets_.contains(name))
        {
            return std::nullopt;
        }

        return std::ref(*instruction_sets_[name]);
    }

    bool InstructionSet::operator==(const std::string_view& rhs) const
    {
        return name_ == rhs;
    }

    InstructionSet::InstructionSet(InstructionSet&& other) noexcept :
        name_(std::move(other.name_)), display_name_(std::move(other.display_name_)),
        machine_code_frontends_(std::move(other.machine_code_frontends_)),
        reference_machine_code_frontend_(other.reference_machine_code_frontend_)
    {
    }

    InstructionSet& InstructionSet::operator=(const InstructionSet& other)
    {
        if (this == &other)
            return *this;

        name_ = other.name_;
        display_name_ = other.display_name_;
        machine_code_frontends_ = other.machine_code_frontends_;
        reference_machine_code_frontend_ = other.reference_machine_code_frontend_;
        return *this;
    }

    InstructionSet& InstructionSet::operator=(InstructionSet&& other) noexcept
    {
        if (this == &other)
            return *this;

        name_ = other.name_;
        display_name_ = other.display_name_;
        machine_code_frontends_ = other.machine_code_frontends_;
        reference_machine_code_frontend_ = other.reference_machine_code_frontend_;
        return *this;
    }

    InstructionSet::~InstructionSet()
    {
        machine_code_frontends_.clear();
        reference_machine_code_frontend_.reset();

        machine_code_frontends_.erase(name_);
    }

    InstructionSet::InstructionSetAlreadyRegisteredException::InstructionSetAlreadyRegisteredException(
        const std::string& name)
        : name_(name), message_(generate_message(name))
    {
    }

    const std::string& InstructionSet::InstructionSetAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& InstructionSet::InstructionSetAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const InstructionSet&
    InstructionSet::InstructionSetAlreadyRegisteredException::get_already_existing_instruction_set() const
    {
        return *instruction_sets_[name_];
    }

    const char* InstructionSet::InstructionSetAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string InstructionSet::InstructionSetAlreadyRegisteredException::generate_message(const std::string& name)
    {
        return "The instruction set " + name + " is already registered.\n" + "Instruction set names must be unique.\n" +
            "You may have two registered instruction sets have the same display names but never the same name.";
    }

    void InstructionSet::setup_instruction_set(const std::string& name)
    {
        if (instruction_sets_.contains(name))
        {
            throw generate_instruction_set_already_registered_exception(name);
        }

        name_ = name;

        machine_code_frontends_ = std::unordered_map<std::string, std::unique_ptr<MachineCodeFrontend>>();
        instruction_sets_.try_emplace(name, this);
    }

    InstructionSet::InstructionSetAlreadyRegisteredException
    InstructionSet::generate_instruction_set_already_registered_exception(const std::string& name)
    {
        return InstructionSetAlreadyRegisteredException(name);
    }
} // namespace SoftOmni::Harnesses::MachineCode::Commons
