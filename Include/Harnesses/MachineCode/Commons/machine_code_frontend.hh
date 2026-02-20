
#ifndef SOFTOMNISWIFT_MACHINE_CODE_FRONTEND_HH
#define SOFTOMNISWIFT_MACHINE_CODE_FRONTEND_HH

#include <Include/Harnesses/MachineCode/Commons/instruction_set.hh>
#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

#include <Include/Compilation/Commons/disassembler.hh>
#include <Include/Compilation/Commons/program_optimizer.hh>
#include <Include/Compilation/Commons/translation_layer.hh>
#include <Include/Execution/Machines/processor.hh>

namespace SoftOmni::Execution::Machines
{
    class Processor;
}

namespace SoftOmni::Compilation::Commons
{
    class TranslationLayer;
    class ProgramOptimizer;
    class Assembler;
    class Disassembler;
}

namespace SoftOmni::Parsing::Commons::Kernels
{
    class LanguageFrontend;
}

namespace SoftOmni::Harnesses::MachineCode::Commons
{
    class MachineCodeFrontend
    {
        std::string name_;

        std::string display_name_;

        InstructionSet* instruction_set_;

        std::unordered_map<Parsing::Commons::Kernels::LanguageFrontend*,
                           std::unordered_set<Compilation::Commons::Disassembler*>>
            capable_language_frontend_targets_;

        std::unordered_map<Parsing::Commons::Kernels::Language*,
                           std::unordered_set<Compilation::Commons::Disassembler*>>
            capable_language_targets_;

        std::unordered_map<std::string, Compilation::Commons::Disassembler*> targeting_disassemblers_;

        std::unordered_set<Compilation::Commons::Disassembler*> disassemblers_;

        std::unordered_map<Parsing::Commons::Kernels::LanguageFrontend*,
                           std::unordered_set<Compilation::Commons::Assembler*>>
            language_frontends_capable_of_targeting_;

        std::unordered_map<Parsing::Commons::Kernels::Language*, std::unordered_set<Compilation::Commons::Assembler*>>
            languages_capable_of_targeting_;

        std::unordered_map<std::string, Compilation::Commons::Assembler*> assemblers_capable_of_targeting_;

        std::unordered_set<Compilation::Commons::Assembler*> assemblers_;

        std::unordered_map<MachineCodeFrontend*, std::unordered_set<Compilation::Commons::TranslationLayer*>>
            capable_machine_code_targets;

        std::unordered_map<InstructionSet*, std::unordered_set<Compilation::Commons::TranslationLayer*>>
            targetable_instruction_sets_;

        std::unordered_map<std::string, Compilation::Commons::TranslationLayer*> targeting_translation_layers_;

        std::unordered_set<Compilation::Commons::TranslationLayer*> outbound_translation_layers_;

        std::unordered_map<MachineCodeFrontend*, std::unordered_set<Compilation::Commons::TranslationLayer*>>
            frontends_capable_of_targeting_;

        std::unordered_map<std::string, Compilation::Commons::TranslationLayer*> source_translation_layers_;

        std::unordered_set<Compilation::Commons::TranslationLayer*> inbound_translation_layers_;

        std::unordered_map<InstructionSet*, std::unordered_set<Compilation::Commons::TranslationLayer*>>
            instruction_sets_capable_of_targeting;

        std::unordered_map<std::string, Execution::Machines::Processor*> processors_;

        std::unordered_set<Execution::Machines::Processor*> processors_set_;

        std::unordered_map<std::string, Compilation::Commons::ProgramOptimizer*> program_optimizers_;

        std::unordered_set<Compilation::Commons::ProgramOptimizer*> program_optimizers_set_;

        static std::unordered_map<InstructionSet*, std::unordered_map<std::string, MachineCodeFrontend*>>
            machine_code_frontends_;

    protected:
        MachineCodeFrontend(std::string name, const std::string& instruction_set);

        MachineCodeFrontend(std::string name, InstructionSet& instruction_set);

        MachineCodeFrontend(std::string name, std::string display_name, const std::string& instruction_set);

        MachineCodeFrontend(std::string name, std::string display_name, InstructionSet& instruction_set);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(std::string new_display_name);

        [[nodiscard]] const InstructionSet& instruction_set() const;

        std::size_t number_of_machine_code_targets() const;

        std::size_t number_of_language_frontend_targets() const;

        std::size_t number_of_targets() const;

        std::size_t number_of_supported_disassemblers() const;

        std::size_t number_of_supported_disassemblers(Parsing::Commons::Kernels::LanguageFrontend& target) const;

        std::size_t number_of_supported_disassemblers(Parsing::Commons::Kernels::Language& target) const;

        std::size_t number_of_outbound_translation_units() const;

        std::size_t number_of_outbound_translation_units(MachineCodeFrontend& target) const;

        std::size_t number_of_outbound_translation_units(InstructionSet& target) const;

        std::size_t number_of_targetable_instruction_sets() const;

        std::size_t number_of_targetable_languages() const;

        std::size_t number_of_machine_code_sources() const;

        std::size_t number_of_language_frontend_sources() const;

        std::size_t number_of_supported_assemblers() const;

        std::size_t number_of_supported_assemblers(Parsing::Commons::Kernels::LanguageFrontend& source) const;

        std::size_t number_of_supported_assemblers(Parsing::Commons::Kernels::Language& source) const;

        std::size_t number_of_inbound_translation_units() const;

        std::size_t number_of_inbound_translation_units(MachineCodeFrontend& source) const;

        std::size_t number_of_inbound_translation_units(InstructionSet& source) const;

        std::size_t number_of_instruction_sets_which_can_target() const;

        std::size_t number_of_languages_which_can_target() const;

        std::size_t number_of_sources() const;

        std::size_t number_of_processors() const;

        std::size_t number_of_program_optimizers() const;

        static std::optional<std::reference_wrapper<MachineCodeFrontend>>
        get(const std::string& instruction_set_name, const std::string& machine_code_frontend_name);

        std::optional<std::reference_wrapper<MachineCodeFrontend>>
        machine_code_target(const std::string& machine_code_frontend_name) const;

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_machine_code_frontend(const std::string& machine_code_frontend_name);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_machine_code_frontend(const MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting(const MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_instruction_set(const std::string& instruction_set_name);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_instruction_set(const InstructionSet& instruction_set);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting(const InstructionSet& machine_code_frontend);

        std::optional<std::reference_wrapper<Parsing::Commons::Kernels::LanguageFrontend>>
        language_target(const std::string& language_frontend_name) const;

        std::unordered_set<Compilation::Commons::Disassembler&>
        disassemblers_capable_of_targeting_language_frontend(const std::string& language_frontend_name);

        std::unordered_set<Compilation::Commons::Disassembler&> disassemblers_capable_of_targeting_language_frontend(
            const Parsing::Commons::Kernels::LanguageFrontend& language_frontend);

        std::unordered_set<Compilation::Commons::Disassembler&>
        disassemblers_capable_of_targeting(const Parsing::Commons::Kernels::LanguageFrontend& language_frontend);

        std::unordered_set<Compilation::Commons::Disassembler&>
        disassemblers_capable_of_targeting_language(const std::string& language_name);

        std::unordered_set<Compilation::Commons::Disassembler&>
        disassemblers_capable_of_targeting_language(const Parsing::Commons::Kernels::Language& language);

        std::unordered_set<Compilation::Commons::Disassembler&>
        disassemblers_capable_of_targeting(const Parsing::Commons::Kernels::Language& language);

        std::optional<std::reference_wrapper<Compilation::Commons::TranslationLayer>>
        outbound_translation_layer(const std::string& translation_layer_name) const;

        std::optional<std::reference_wrapper<MachineCodeFrontend>>
        machine_code_source(const std::string& machine_code_frontend_name) const;

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_from_machine_code_frontend(
            const std::string& machine_code_frontend_name);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_from_machine_code_frontend(
            const MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_from(const MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_from_instruction_set(const std::string& instruction_set_name);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_from_instruction_set(const InstructionSet& instruction_set);

        std::unordered_set<Compilation::Commons::TranslationLayer&>
        translation_layers_capable_of_targeting_from(const InstructionSet& instruction_set);

        std::optional<std::reference_wrapper<Parsing::Commons::Kernels::LanguageFrontend>>
        language_source(const std::string& language_frontend_name) const;

        std::unordered_set<Compilation::Commons::Assembler&>
        assemblers_capable_of_targeting_from_language_frontend(const std::string& language_frontend_name);

        std::unordered_set<Compilation::Commons::Assembler&> assemblers_capable_of_targeting_from_language_frontend(
            const Parsing::Commons::Kernels::LanguageFrontend& language_frontend);

        std::unordered_set<Compilation::Commons::Assembler&>
        assemblers_capable_of_targeting_from(const Parsing::Commons::Kernels::LanguageFrontend& language_frontend);

        std::unordered_set<Compilation::Commons::Assembler&>
        assemblers_capable_of_targeting_from_language(const std::string& language_name);

        std::unordered_set<Compilation::Commons::Assembler&>
        assemblers_capable_of_targeting_from_language(const Parsing::Commons::Kernels::Language& language);

        std::unordered_set<Compilation::Commons::Assembler&>
        assemblers_capable_of_targeting_from(const Parsing::Commons::Kernels::Language& language);

        std::optional<std::reference_wrapper<Execution::Machines::Processor>>
        processor(const std::string& processor_name) const;

        std::optional<std::reference_wrapper<Compilation::Commons::ProgramOptimizer>>
        program_optimizer(const std::string& program_optimizer_name) const;

        bool can_target_machine_code_frontend(const std::string& machine_code_frontend_name) const;

        bool can_target_machine_code_frontend(const MachineCodeFrontend& machine_code_frontend) const;

        bool can_target(const MachineCodeFrontend& machine_code_frontend) const;

        bool can_target_instruction_set(const std::string& instruction_set_name) const;

        bool can_target_instruction_set(const InstructionSet& instruction_set) const;

        bool can_target(const InstructionSet& instruction_set) const;

        bool can_target_language_frontend(const std::string& language_frontend_name) const;

        bool can_target_language_frontend(const Parsing::Commons::Kernels::LanguageFrontend& language_frontend) const;

        bool can_target(const Parsing::Commons::Kernels::LanguageFrontend& language_frontend) const;

        bool can_target_language(const std::string& language_name);

        bool can_target_language(const Parsing::Commons::Kernels::Language& language);

        bool can_target(const Parsing::Commons::Kernels::Language& language);

        bool can_be_targeted_by_machine_code_frontend(const std::string& machine_code_frontend_name) const;

        bool can_be_targeted_by_machine_code_frontend(const MachineCodeFrontend& machine_code_frontend) const;

        bool can_be_targeted_by(const MachineCodeFrontend& machine_code_frontend) const;

        bool can_be_targeted_by_instruction_set(const std::string& instruction_set_name) const;

        bool can_be_targeted_by_instruction_set(const InstructionSet& instruction_set) const;

        bool can_be_targeted_by(const InstructionSet& instruction_set) const;

        bool can_be_targeted_by_language_frontend(const std::string& language_frontend_name) const;

        bool can_be_targeted_by_language_frontend(
            const Parsing::Commons::Kernels::LanguageFrontend& language_frontend) const;

        bool can_be_targeted_by(const Parsing::Commons::Kernels::LanguageFrontend& language_frontend) const;

        bool can_be_targeted_by_language(const std::string& language_name) const;

        bool can_be_targeted_by_language(const Parsing::Commons::Kernels::Language& language) const;

        bool can_be_targeted_by(const Parsing::Commons::Kernels::Language& language) const;

        bool processor_supports_frontend(const std::string& processor_name) const;

        bool processor_supports_frontend(const Execution::Machines::Processor& processor) const;

        bool program_optimizer_supports_frontend(const std::string& program_optimizer_name) const;

        bool program_optimizer_supports_frontend(const Compilation::Commons::ProgramOptimizer& program_optimizer) const;

        friend bool operator==(const MachineCodeFrontend& lhs, const MachineCodeFrontend& rhs)
        {
            return &lhs == &rhs;
        }

        friend bool operator==(const MachineCodeFrontend& lhs, const std::string& rhs)
        {
            return lhs.name_ == rhs;
        }

        friend bool operator==(const std::string& lhs, const MachineCodeFrontend& rhs)
        {
            return lhs == rhs.name_;
        }

        ~MachineCodeFrontend();

        friend class ::SoftOmni::Execution::Machines::Processor;

        friend class ::SoftOmni::Compilation::Commons::TranslationLayer;

        friend class ::SoftOmni::Compilation::Commons::Assembler;

        friend class ::SoftOmni::Compilation::Commons::Disassembler;

        friend class ::SoftOmni::Compilation::Commons::ProgramOptimizer;

        class MachineCodeFrontendAlreadyRegisteredException : std::exception
        {
            std::string name_;

            std::string message_;

            explicit MachineCodeFrontendAlreadyRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const MachineCodeFrontend& existing_machine_code_frontend() const;

            [[nodiscard]] const char* what() const noexcept override;

        private:
            std::string generate_message(const std::string& name);
        };

    protected:
        template <typename key, typename value, typename return_value>
        class UnorderedMapIterator
        {
            std::unordered_map<key, value>* map_;

            std::unordered_map<key, value>::const_iterator iterator_;

            explicit UnorderedMapIterator(std::unordered_map<key, value>* map);

        public:
            return_value operator*() const;

            UnorderedMapIterator& operator++();
            UnorderedMapIterator operator++(int offset);

            friend bool operator==(const UnorderedMapIterator& lhs, const UnorderedMapIterator& rhs)
            {
                return lhs.iterator_ == rhs.iterator_;
            }

            friend class MachineCodeFrontend;
        };

        template <typename key, typename return_value>
        class UnorderedSetIterator
        {
            std::unordered_set<key>* map_;

            std::unordered_set<key>::const_iterator iterator_;

            explicit UnorderedSetIterator(std::unordered_set<key>* map);

        public:
            return_value operator*() const;

            UnorderedSetIterator& operator++();

            UnorderedSetIterator operator++(int offset);

            friend bool operator==(const UnorderedSetIterator& lhs, const UnorderedSetIterator& rhs)
            {
                return lhs.iterator_ == rhs.iterator_;
            }

            friend class MachineCodeFrontend;
        };

        using MachineCodeFrontendIterator =
            UnorderedMapIterator<MachineCodeFrontend*, Compilation::Commons::TranslationLayer*,
                                 const Compilation::Commons::TranslationLayer&>;

        using InstructionSetIterator = UnorderedMapIterator<InstructionSet*, Compilation::Commons::TranslationLayer*,
                                                            const Compilation::Commons::TranslationLayer&>;

        using TranslationLayerIterator = UnorderedSetIterator<Compilation::Commons::TranslationLayer*,
                                                              const Compilation::Commons::TranslationLayer&>;

        using AssemblersIterator =
            UnorderedSetIterator<Compilation::Commons::Assembler*, const Compilation::Commons::Assembler&>;

        using LanguageFrontendIterator =
            UnorderedMapIterator<Parsing::Commons::Kernels::LanguageFrontend*, Compilation::Commons::Disassembler*,
                                 const Compilation::Commons::Disassembler&>;

        using LanguageIterator =
            UnorderedMapIterator<Parsing::Commons::Kernels::Language*, Compilation::Commons::Disassembler*,
                                 const Compilation::Commons::Disassembler&>;

        using DisAssemblersIterator =
            UnorderedSetIterator<Compilation::Commons::Disassembler*, const Compilation::Commons::Disassembler&>;

        using ProcessorsIterator =
            UnorderedSetIterator<Execution::Machines::Processor*, const Execution::Machines::Processor&>;

        using ProgramOptimizerIterator =
            UnorderedSetIterator<Compilation::Commons::ProgramOptimizer*, const Compilation::Commons::ProgramOptimizer&>;

    public:
        MachineCodeFrontendIterator targetable_machine_code_frontends_begin() const;

        MachineCodeFrontendIterator targetable_machine_code_frontends_end() const;

        InstructionSetIterator targetable_instruction_sets_begin() const;

        InstructionSetIterator targetable_instruction_sets_end() const;

        TranslationLayerIterator outbound_translation_layers_begin() const;

        TranslationLayerIterator outbound_translation_layers_end() const;

        LanguageFrontendIterator targetable_language_frontends_begin() const;

        LanguageFrontendIterator targetable_language_frontends_end() const;

        LanguageIterator targetable_languages_begin() const;

        LanguageIterator targetable_languages_end() const;

        DisAssemblersIterator disassemblers_begin() const;

        DisAssemblersIterator disassemblers_end() const;

        MachineCodeFrontendIterator machine_code_frontends_capable_of_targeting_begin() const;

        MachineCodeFrontendIterator machine_code_frontends_capable_of_targeting_end() const;

        InstructionSetIterator instruction_sets_capable_of_targeting_begin() const;

        InstructionSetIterator instruction_sets_capable_of_targeting_end() const;

        TranslationLayerIterator inbound_translation_layers_begin() const;

        TranslationLayerIterator inbound_translation_layers_end() const;

        AssemblersIterator assemblers_begin() const;

        AssemblersIterator assemblers_end() const;

        LanguageFrontendIterator language_frontends_capable_of_targeting_begin() const;

        LanguageFrontendIterator language_frontends_capable_of_targeting_end() const;

        LanguageIterator languages_capable_of_targeting_begin() const;

        LanguageIterator languages_capable_of_targeting_end() const;

        ProcessorsIterator processors_begin() const;

        ProcessorsIterator processors_end() const;

        ProgramOptimizerIterator program_optimizers_begin() const;

        ProgramOptimizerIterator program_optimizers_end() const;

    private:
        void setup_machine_code_frontend();

        MachineCodeFrontendAlreadyRegisteredException
        generate_machine_code_frontend_already_registered_exception(const std::string& name);
    };
} // namespace SoftOmni::Harnesses::MachineCode::Commons

#endif // SOFTOMNISWIFT_MACHINE_CODE_FRONTEND_HH
