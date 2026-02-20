
#include <Include/Compilation/Commons/assembler.hh>
#include <utility>

namespace SoftOmni::Compilation::Commons
{
    Assembler::Assembler(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& frontend,
                         Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        source_language_frontend_(&frontend), target_machine_code_frontend_(&machine_code_frontend)
    {
        if (assemblers_.contains(name))
        {
            throw generate_assembler_already_registered_exception(name);
        }

        assemblers_.try_emplace(name, this);

        name_ = name;
        display_name_ = name;

        setup_frontends();
    }

    Assembler::Assembler(std::string name, std::string display_name,
                         Parsing::Commons::Kernels::LanguageFrontend& frontend,
                         Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        source_language_frontend_(&frontend), target_machine_code_frontend_(&machine_code_frontend)
    {
        if (assemblers_.contains(name))
        {
            throw generate_assembler_already_registered_exception(name);
        }

        assemblers_.try_emplace(name, this);

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        setup_frontends();
    }
    const std::string& Assembler::name() const
    {
        return name_;
    }

    const std::string& Assembler::display_name() const
    {
        return display_name_;
    }

    void Assembler::set_display_name(const std::string& new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Parsing::Commons::Kernels::LanguageFrontend& Assembler::source_frontend() const
    {
        return *source_language_frontend_;
    }

    const Harnesses::MachineCode::Commons::MachineCodeFrontend& Assembler::target_frontend() const
    {
        return *target_machine_code_frontend_;
    }

    Assembler::~Assembler()
    {
        assemblers_.erase(name_);

        source_language_frontend_->capable_machine_code_frontend_targets_[target_machine_code_frontend_].erase(this);
        if (source_language_frontend_->capable_machine_code_frontend_targets_[target_machine_code_frontend_].empty())
        {
            source_language_frontend_->capable_machine_code_frontend_targets_.erase(target_machine_code_frontend_);
        }

        source_language_frontend_->capable_instruction_set_targets_[target_machine_code_frontend_->instruction_set_]
            .erase(this);
        if (source_language_frontend_->capable_instruction_set_targets_[target_machine_code_frontend_->instruction_set_]
                .empty())
        {
            source_language_frontend_->capable_instruction_set_targets_.erase(
                target_machine_code_frontend_->instruction_set_);
        }

        source_language_frontend_->targeting_assemblers_.erase(name_);
        source_language_frontend_->assemblers_.erase(this);

        target_machine_code_frontend_->language_frontends_capable_of_targeting_[source_language_frontend_].erase(this);
        if (target_machine_code_frontend_->language_frontends_capable_of_targeting_[source_language_frontend_].empty())
        {
            target_machine_code_frontend_->language_frontends_capable_of_targeting_.erase(source_language_frontend_);
        }

        target_machine_code_frontend_->languages_capable_of_targeting_[source_language_frontend_->language_].erase(
            this);
        if (target_machine_code_frontend_->languages_capable_of_targeting_[source_language_frontend_->language_]
                .empty())
        {
            target_machine_code_frontend_->languages_capable_of_targeting_.erase(source_language_frontend_->language_);
        }

        target_machine_code_frontend_->assemblers_capable_of_targeting_.erase(name_);
        target_machine_code_frontend_->assemblers_.erase(this);
    }

    void Assembler::setup_frontends()
    {
        Harnesses::MachineCode::Commons::InstructionSet* instruction_set =
            target_machine_code_frontend_->instruction_set_;
        if (source_language_frontend_->capable_machine_code_frontend_targets_.contains(target_machine_code_frontend_))
        {
            source_language_frontend_->capable_machine_code_frontend_targets_[target_machine_code_frontend_].emplace(
                this);
            source_language_frontend_->capable_instruction_set_targets_[instruction_set].emplace(this);
        }
        else
        {
            auto machine_code_assemblers = std::unordered_set<Assembler*>();
            machine_code_assemblers.emplace(this);

            source_language_frontend_->capable_machine_code_frontend_targets_[target_machine_code_frontend_] =
                machine_code_assemblers;

            if (source_language_frontend_->capable_instruction_set_targets_.contains(instruction_set))
            {
                source_language_frontend_->capable_instruction_set_targets_[instruction_set].emplace(this);
            }
            else
            {
                auto instruction_set_assemblers = std::unordered_set<Assembler*>();
                instruction_set_assemblers.emplace(this);

                source_language_frontend_->capable_instruction_set_targets_[instruction_set] =
                    instruction_set_assemblers;
            }
        }

        source_language_frontend_->targeting_assemblers_.try_emplace(name_, this);
        source_language_frontend_->assemblers_.emplace(this);

        Parsing::Commons::Kernels::Language* language = source_language_frontend_->language_;
        if (target_machine_code_frontend_->language_frontends_capable_of_targeting_.contains(source_language_frontend_))
        {
            target_machine_code_frontend_->language_frontends_capable_of_targeting_[source_language_frontend_].emplace(
                this);
            target_machine_code_frontend_->languages_capable_of_targeting_[language].emplace(this);
        }
        else
        {
            auto language_frontend_assemblers = std::unordered_set<Assembler*>();
            language_frontend_assemblers.emplace(this);

            target_machine_code_frontend_->language_frontends_capable_of_targeting_[source_language_frontend_] =
                language_frontend_assemblers;

            if (target_machine_code_frontend_->languages_capable_of_targeting_.contains(language))
            {
                target_machine_code_frontend_->languages_capable_of_targeting_[language].emplace(this);
            }
            else
            {
                auto language_assemblers = std::unordered_set<Assembler*>();
                language_assemblers.emplace(this);

                target_machine_code_frontend_->languages_capable_of_targeting_[language] = language_assemblers;
            }
        }

        target_machine_code_frontend_->assemblers_capable_of_targeting_.try_emplace(name_, this);
        target_machine_code_frontend_->assemblers_.emplace(this);
    }

    Assembler::AssemblerAlreadyRegisteredException::AssemblerAlreadyRegisteredException(const std::string& name) :
        name_(name), message_(generate_message(name))
    {
    }

    const std::string& Assembler::AssemblerAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& Assembler::AssemblerAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const Assembler& Assembler::AssemblerAlreadyRegisteredException::get_already_existing_assembler() const
    {
        return *assemblers_[name_];
    }

    const char* Assembler::AssemblerAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string Assembler::AssemblerAlreadyRegisteredException::generate_message(const std::string& name)
    {
        const Assembler* assembler = assemblers_[name];
        return "ERROR: The assembler '" + name + "' which assembles from " +
            assembler->source_language_frontend_->language_->name() + " (" +
            assembler->source_language_frontend_->name_ + " language frontend) to " +
            assembler->target_machine_code_frontend_->instruction_set_->name() + " (" +
            assembler->target_machine_code_frontend_->instruction_set_->name() +
            " machine code frontend) is already registered";
    }

    Assembler::AssemblerAlreadyRegisteredException
    Assembler::generate_assembler_already_registered_exception(const std::string& name)
    {
        return AssemblerAlreadyRegisteredException(name);
    }

    std::size_t
    Assembler::AssemblerRefHash::operator()(const std::reference_wrapper<Assembler>& assembler_reference) const noexcept
    {
        return std::hash<Assembler*>{}(&assembler_reference.get());
    }

    bool Assembler::AssemblerRefEq::operator()(const std::reference_wrapper<Assembler>& lhs,
                                               const std::reference_wrapper<Assembler>& rhs) const
    {
        return &lhs.get() == &rhs.get();
    }
} // namespace SoftOmni::Compilation::Commons
