
#include <Include/Compilation/Commons/translation_layer.hh>

namespace SoftOmni::Compilation::Commons
{
    TranslationLayer::TranslationLayer(const std::string& name,
                                       Harnesses::MachineCode::Commons::MachineCodeFrontend& source,
                                       Harnesses::MachineCode::Commons::MachineCodeFrontend& target) :
        source_(&source), target_(&target)
    {
        if (source == target)
        {
            throw generate_translation_layer_between_same_machine_code_frontends_exception(source);
        }

        if (translation_layers_.contains(name))
        {
            throw generate_translation_unit_already_registered_exception(name);
        }

        translation_layers_.try_emplace(name_, this);

        name_ = name;
        display_name_ = name;

        setup_translation_layers_relationships();
        translation_layers_.try_emplace(name, this);
    }

    TranslationLayer::TranslationLayer(std::string name, std::string display_name,
                                       Harnesses::MachineCode::Commons::MachineCodeFrontend& source,
                                       Harnesses::MachineCode::Commons::MachineCodeFrontend& target) :
        source_(&source), target_(&target)
    {
        if (source == target)
        {
            throw generate_translation_layer_between_same_machine_code_frontends_exception(source);
        }

        if (translation_layers_.contains(name))
        {
            throw generate_translation_unit_already_registered_exception(name);
        }

        translation_layers_.try_emplace(name_, this);

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        setup_translation_layers_relationships();
    }

    const std::string& TranslationLayer::name() const
    {
        return name_;
    }

    const std::string& TranslationLayer::display_name() const
    {
        return display_name_;
    }

    void TranslationLayer::set_display_name(std::string new_display_name)
    {
        display_name_ = std::move(new_display_name);
    }

    const Harnesses::MachineCode::Commons::MachineCodeFrontend& TranslationLayer::source() const
    {
        return *source_;
    }

    const Harnesses::MachineCode::Commons::MachineCodeFrontend& TranslationLayer::target() const
    {
        return *target_;
    }

    TranslationLayer::~TranslationLayer()
    {
        translation_layers_.erase(name_);

        target_->frontends_capable_of_targeting_[source_].erase(this);
        if (target_->frontends_capable_of_targeting_[source_].empty())
        {
            target_->frontends_capable_of_targeting_.erase(source_);
        }

        target_->instruction_sets_capable_of_targeting[source_->instruction_set_].erase(this);
        if (target_->instruction_sets_capable_of_targeting[source_->instruction_set_].empty())
        {
            target_->instruction_sets_capable_of_targeting.erase(source_->instruction_set_);
        }

        target_->source_translation_layers_.erase(name_);
        target_->inbound_translation_layers_.erase(this);

        source_->capable_machine_code_targets[target_].erase(this);
        if (source_->capable_machine_code_targets[target_].empty())
        {
            source_->capable_machine_code_targets.erase(target_);
        }

        source_->targetable_instruction_sets_[target_->instruction_set_].erase(this);
        if (source_->targetable_instruction_sets_[target_->instruction_set_].empty())
        {
            source_->targetable_instruction_sets_.erase(target_->instruction_set_);
        }

        source_->targeting_translation_layers_.erase(name_);
        source_->outbound_translation_layers_.erase(this);
    }

    void TranslationLayer::setup_translation_layers_relationships()
    {
        Harnesses::MachineCode::Commons::InstructionSet* source_instruction_set = source_->instruction_set_;

        if (target_->frontends_capable_of_targeting_.contains(source_))
        {
            target_->frontends_capable_of_targeting_[source_].emplace(this);
            target_->instruction_sets_capable_of_targeting[source_instruction_set].emplace(this);
        }
        else
        {
            auto target_frontend_translation_layers = std::unordered_set<TranslationLayer*>();
            target_frontend_translation_layers.emplace(this);

            target_->frontends_capable_of_targeting_[source_] = target_frontend_translation_layers;
            if (target_->instruction_sets_capable_of_targeting.contains(source_instruction_set))
            {
                target_->instruction_sets_capable_of_targeting[source_instruction_set].emplace(this);
            }
            else
            {
                auto target_instruction_set_translation_layers = std::unordered_set<TranslationLayer*>();
                target_instruction_set_translation_layers.emplace(this);

                target_->instruction_sets_capable_of_targeting[source_instruction_set] = target_instruction_set_translation_layers;
            }
        }

        target_->source_translation_layers_.try_emplace(name_, this);
        target_->inbound_translation_layers_.emplace(this);

        Harnesses::MachineCode::Commons::InstructionSet* target_instruction_set = target_->instruction_set_;

        if (source_->capable_machine_code_targets.contains(target_))
        {
            source_->capable_machine_code_targets[target_].emplace(this);
            source_->targetable_instruction_sets_[target_instruction_set].emplace(this);
        }
        else
        {
            auto source_frontend_translation_layers = std::unordered_set<TranslationLayer*>();
            source_frontend_translation_layers.emplace(this);

            source_->capable_machine_code_targets[target_] = source_frontend_translation_layers;
            if (source_->targetable_instruction_sets_.contains(target_instruction_set))
            {
                source_->targetable_instruction_sets_[target_instruction_set].emplace(this);
            }
            else
            {
                auto source_instruction_set_translation_layers = std::unordered_set<TranslationLayer*>();
                source_instruction_set_translation_layers.emplace(this);

                source_->targetable_instruction_sets_[target_instruction_set] = source_instruction_set_translation_layers;
            }
        }

        source_->targeting_translation_layers_.try_emplace(name_, this);
        source_->outbound_translation_layers_.emplace(this);
    }

    TranslationLayer::TranslationLayerAlreadyRegisteredException
    TranslationLayer::generate_translation_unit_already_registered_exception(const std::string& name)
    {
        return TranslationLayerAlreadyRegisteredException(name);
    }

    TranslationLayer::TranslationLayerBetweenSameMachineCodeFrontendsException
    TranslationLayer::generate_translation_layer_between_same_machine_code_frontends_exception(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        return TranslationLayerBetweenSameMachineCodeFrontendsException(machine_code_frontend);
    }

    const std::string& TranslationLayer::TranslationLayerAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& TranslationLayer::TranslationLayerAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const TranslationLayer&
    TranslationLayer::TranslationLayerAlreadyRegisteredException::get_already_existing_translation_layer() const
    {
        return *translation_layers_[name_];
    }

    const char* TranslationLayer::TranslationLayerAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    TranslationLayer::TranslationLayerAlreadyRegisteredException::TranslationLayerAlreadyRegisteredException(
        const std::string& name) : name_(name), message_(generate_message(name))
    {
    }

    std::string TranslationLayer::TranslationLayerAlreadyRegisteredException::generate_message(const std::string& name)
    {
        const TranslationLayer* translation_layer = translation_layers_[name];
        return "ERROR: The translation layer '" + name + "' which translates from " +
            translation_layer->source_->instruction_set_->name() + " (" + translation_layer->source_->name_ +
            " machine code frontend) to " + translation_layer->target_->instruction_set_->name() + " (" +
            translation_layer->target_->name_ + " machine code frontend) is already registered";
    }

    TranslationLayer::TranslationLayerBetweenSameMachineCodeFrontendsException::
        TranslationLayerBetweenSameMachineCodeFrontendsException(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) :
        message_(generate_message(machine_code_frontend))
    {
    }

    const std::string& TranslationLayer::TranslationLayerBetweenSameMachineCodeFrontendsException::message() const
    {
        return message_;
    }

    const char* TranslationLayer::TranslationLayerBetweenSameMachineCodeFrontendsException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string TranslationLayer::TranslationLayerBetweenSameMachineCodeFrontendsException::generate_message(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        return "ERROR: Cannot translate from the machine code frontend '" + machine_code_frontend.name() +
            "' (for the '" + machine_code_frontend.instruction_set().name() +
            "' instruction set) to the same exact machine code frontend. Compiling from the same instruction set but " +
            "between 2 different machine code frontends is allowed. Use an a Compilation::Commons::ProgramOptimizer " +
            "for optimizing the code of a machine code frontend.";
    }

} // namespace SoftOmni::Compilation::Commons
