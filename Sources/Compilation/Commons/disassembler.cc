
#include <Include/Compilation/Commons/disassembler.hh>
#include <utility>

namespace SoftOmni::Compilation::Commons
{
    Disassembler::Disassembler(const std::string& name,
                               Harnesses::MachineCode::Commons::MachineCodeFrontend& source_machine_code_frontend,
                               Parsing::Commons::Kernels::LanguageFrontend& target_language_frontend) :
        source_machine_code_frontend_(&source_machine_code_frontend),
        target_language_frontend_(&target_language_frontend)
    {
        if (disassemblers_.contains(name))
        {
            throw generate_disassembler_already_registered_exception(name);
        }

        disassemblers_.try_emplace(name, this);

        name_ = name;
        display_name_ = name;

        setup_frontends();
    }

    Disassembler::Disassembler(std::string name, std::string display_name,
                               Harnesses::MachineCode::Commons::MachineCodeFrontend& source_machine_code_frontend,
                               Parsing::Commons::Kernels::LanguageFrontend& target_language_frontend) :
        source_machine_code_frontend_(&source_machine_code_frontend),
        target_language_frontend_(&target_language_frontend)
    {
        if (disassemblers_.contains(name))
        {
            throw generate_disassembler_already_registered_exception(name);
        }

        disassemblers_.try_emplace(name, this);

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        setup_frontends();
    }

    const std::string& Disassembler::name() const
    {
        return name_;
    }

    const std::string& Disassembler::display_name() const
    {
        return display_name_;
    }

    void Disassembler::set_display_name(const std::string_view& new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Harnesses::MachineCode::Commons::MachineCodeFrontend& Disassembler::source_frontend() const
    {
        return *source_machine_code_frontend_;
    }

    const Parsing::Commons::Kernels::LanguageFrontend& Disassembler::target_frontend() const
    {
        return *target_language_frontend_;
    }

    Disassembler::~Disassembler()
    {
        disassemblers_.erase(name_);

        target_language_frontend_->machine_code_frontends_capable_of_targeting_[source_machine_code_frontend_].erase(
            this);
        if (target_language_frontend_->machine_code_frontends_capable_of_targeting_[source_machine_code_frontend_]
                .empty())
        {
            target_language_frontend_->machine_code_frontends_capable_of_targeting_.erase(
                source_machine_code_frontend_);
        }

        target_language_frontend_
            ->instruction_sets_capable_of_targeting_[source_machine_code_frontend_->instruction_set_]
            .erase(this);
        if (target_language_frontend_
                ->instruction_sets_capable_of_targeting_[source_machine_code_frontend_->instruction_set_]
                .empty())
        {
            target_language_frontend_->instruction_sets_capable_of_targeting_.erase(
                source_machine_code_frontend_->instruction_set_);
        }

        target_language_frontend_->source_disassemblers_.erase(name_);
        target_language_frontend_->disassemblers_.erase(this);

        source_machine_code_frontend_->capable_language_frontend_targets_[target_language_frontend_].erase(this);
        if (source_machine_code_frontend_->capable_language_frontend_targets_[target_language_frontend_].empty())
        {
            source_machine_code_frontend_->capable_language_frontend_targets_.erase(target_language_frontend_);
        }

        source_machine_code_frontend_->capable_language_targets_[target_language_frontend_->language_].erase(this);
        if (source_machine_code_frontend_->capable_language_targets_[target_language_frontend_->language_].empty())
        {
            source_machine_code_frontend_->capable_language_targets_.erase(target_language_frontend_->language_);
        }

        source_machine_code_frontend_->targeting_disassemblers_.erase(name_);
        source_machine_code_frontend_->disassemblers_.erase(this);
    }

    void Disassembler::setup_frontends()
    {
        Harnesses::MachineCode::Commons::InstructionSet* instruction_set =
            source_machine_code_frontend_->instruction_set_;
        if (target_language_frontend_->machine_code_frontends_capable_of_targeting_.contains(
                source_machine_code_frontend_))
        {
            target_language_frontend_->machine_code_frontends_capable_of_targeting_[source_machine_code_frontend_]
                .emplace(this);
            target_language_frontend_->instruction_sets_capable_of_targeting_[instruction_set].emplace(this);
        }
        else
        {
            auto machine_code_frontend_disassemblers = std::unordered_set<Disassembler*>();
            machine_code_frontend_disassemblers.emplace(this);

            target_language_frontend_->machine_code_frontends_capable_of_targeting_[source_machine_code_frontend_] =
                machine_code_frontend_disassemblers;

            if (target_language_frontend_->instruction_sets_capable_of_targeting_.contains(instruction_set))
            {
                target_language_frontend_->instruction_sets_capable_of_targeting_[instruction_set].emplace(this);
            }
            else
            {
                auto instruction_set_disassemblers = std::unordered_set<Disassembler*>();
                instruction_set_disassemblers.emplace(this);

                target_language_frontend_->instruction_sets_capable_of_targeting_[instruction_set] =
                    instruction_set_disassemblers;
            }
        }

        target_language_frontend_->source_disassemblers_.try_emplace(name_, this);
        target_language_frontend_->disassemblers_.emplace(this);

        Parsing::Commons::Kernels::Language* language = target_language_frontend_->language_;
        if (source_machine_code_frontend_->capable_language_frontend_targets_.contains(target_language_frontend_))
        {
            source_machine_code_frontend_->capable_language_frontend_targets_[target_language_frontend_].emplace(this);
            source_machine_code_frontend_->capable_language_targets_[language].emplace(this);
        }
        else
        {
            auto language_frontend_disassemblers = std::unordered_set<Disassembler*>();
            language_frontend_disassemblers.emplace(this);

            source_machine_code_frontend_->capable_language_frontend_targets_[target_language_frontend_] =
                language_frontend_disassemblers;

            if (source_machine_code_frontend_->capable_language_targets_.contains(language))
            {
                source_machine_code_frontend_->capable_language_targets_[language].emplace(this);
            }
            else
            {
                auto language_disassemblers = std::unordered_set<Disassembler*>();
                language_disassemblers.emplace(this);

                source_machine_code_frontend_->capable_language_targets_[language] = language_disassemblers;
            }
        }

        source_machine_code_frontend_->targeting_disassemblers_.try_emplace(name_, this);
        source_machine_code_frontend_->disassemblers_.emplace(this);
    }

    Disassembler::DisassemblerAlreadyRegisteredException::DisassemblerAlreadyRegisteredException(
        const std::string& name) : name_(name), message_(generate_message(name))
    {
    }

    const std::string& Disassembler::DisassemblerAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& Disassembler::DisassemblerAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const Disassembler& Disassembler::DisassemblerAlreadyRegisteredException::get_already_existing_disassembler() const
    {
        return *disassemblers_[name_];
    }

    const char* Disassembler::DisassemblerAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string Disassembler::DisassemblerAlreadyRegisteredException::generate_message(const std::string& name)
    {
        const Disassembler* disassembler = disassemblers_[name];
        return "ERROR: The disassembler '" + name + "' which disassembles from " +
            disassembler->source_machine_code_frontend_->instruction_set_->name() + " (" +
            disassembler->source_machine_code_frontend_->name_ + " machine code frontend) to " +
            disassembler->target_language_frontend_->language_->name() + " (" +
            disassembler->target_language_frontend_->name_ + " language frontend) is already registered";
    }

    Disassembler::DisassemblerAlreadyRegisteredException
    Disassembler::generate_disassembler_already_registered_exception(const std::string& name)
    {
        return DisassemblerAlreadyRegisteredException(name);
    }
} // namespace SoftOmni::Compilation::Commons
