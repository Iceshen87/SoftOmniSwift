#ifndef SOFTOMNISWIFT_LANGUAGE_FRONTEND_HH
#define SOFTOMNISWIFT_LANGUAGE_FRONTEND_HH

#include <cstddef>
#include <exception>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include <Include/Compilation/Commons/assembler.hh>
#include <Include/Compilation/Commons/disassembler.hh>
#include <Include/Compilation/Commons/language_compiler.hh>
#include <Include/Compilation/Commons/optimizer.hh>
#include <Include/Harnesses/MachineCode/Commons/machine_code_frontend.hh>
#include <Include/Parsing/Commons/Kernels/language.hh>

namespace SoftOmni::Execution::Commons
{
    class LanguageRuntime;
}

namespace SoftOmni::Compilation::Commons
{
    class LanguageCompiler;
    class Optimizer;
    class Assembler;
    class Disassembler;
} // namespace SoftOmni::Compilation::Commons

namespace SoftOmni::Harnesses::MachineCode::Commons
{
    class MachineCodeFrontend;
}

namespace SoftOmni::Parsing::Commons::Kernels
{
    class LanguageFrontend
    {
        std::string name_;

        std::string display_name_;

        Language* language_;

        std::unordered_map<const Harnesses::MachineCode::Commons::MachineCodeFrontend*,
                           std::unordered_set<Compilation::Commons::Assembler*>>
            capable_machine_code_frontend_targets_;

        std::unordered_map<const Harnesses::MachineCode::Commons::InstructionSet*,
                           std::unordered_set<Compilation::Commons::Assembler*>>
            capable_instruction_set_targets_;

        std::unordered_map<std::string, Compilation::Commons::Assembler*> targeting_assemblers_;

        std::unordered_set<Compilation::Commons::Assembler*> assemblers_;

        std::unordered_map<const LanguageFrontend*, std::unordered_set<Compilation::Commons::LanguageCompiler*>>
            capable_language_frontend_targets_;

        std::unordered_map<const Language*, std::unordered_set<Compilation::Commons::LanguageCompiler*>>
            capable_language_targets_;

        std::unordered_map<std::string, Compilation::Commons::LanguageCompiler*> targeting_language_compilers_;

        std::unordered_set<Compilation::Commons::LanguageCompiler*> inbound_language_compilers_;

        std::unordered_map<const Harnesses::MachineCode::Commons::MachineCodeFrontend*,
                           std::unordered_set<Compilation::Commons::Disassembler*>>
            machine_code_frontends_capable_of_targeting_;

        std::unordered_map<const Harnesses::MachineCode::Commons::InstructionSet*,
                           std::unordered_set<Compilation::Commons::Disassembler*>>
            instruction_sets_capable_of_targeting_;

        std::unordered_map<std::string, Compilation::Commons::Disassembler*> source_disassemblers_;

        std::unordered_set<Compilation::Commons::Disassembler*> disassemblers_;

        std::unordered_map<const LanguageFrontend*, std::unordered_set<Compilation::Commons::LanguageCompiler*>>
            language_frontends_capable_of_targeting_;

        std::unordered_map<const Language*, std::unordered_set<Compilation::Commons::LanguageCompiler*>>
            languages_capable_of_targeting_;

        std::unordered_map<std::string, Compilation::Commons::LanguageCompiler*> source_language_compilers_;

        std::unordered_set<Compilation::Commons::LanguageCompiler*> outbound_language_compilers_;

        std::unordered_map<std::string, Execution::Commons::LanguageRuntime*> runtimes_;

        std::unordered_set<const Execution::Commons::LanguageRuntime*> runtimes_set_;

        std::unordered_map<std::string, Compilation::Commons::Optimizer*> optimizers_;

        std::unordered_set<const Compilation::Commons::Optimizer*> optimizers_set_;

        static std::unordered_map<Language*, std::unordered_map<std::string, LanguageFrontend*>> language_frontends_;

    protected:
        explicit LanguageFrontend(const std::string& name, const std::string& language);

        explicit LanguageFrontend(const std::string& name, Language& language);

        LanguageFrontend(std::string name, std::string display_name, const std::string& language);

        LanguageFrontend(std::string name, std::string display_name, Language& language);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(const std::string_view& new_display_name);

        [[nodiscard]] const Language& language() const;

        std::size_t number_of_machine_code_frontend_targets() const;

        std::size_t number_of_instruction_set_targets() const;

        std::size_t number_of_language_frontend_targets() const;

        std::size_t number_of_language_targets() const;

        std::size_t number_of_targets() const;

        std::size_t number_of_supported_assemblers() const;

        std::size_t number_of_supported_assemblers(Harnesses::MachineCode::Commons::MachineCodeFrontend& target) const;

        std::size_t number_of_supported_assemblers(Harnesses::MachineCode::Commons::InstructionSet& target) const;

        std::size_t number_of_supported_outbound_language_compilers() const;

        std::size_t number_of_supported_outbound_language_compilers(LanguageFrontend& target) const;

        std::size_t number_of_supported_outbound_language_compilers(Language& target) const;

        std::size_t number_of_machine_code_frontend_sources() const;

        std::size_t number_of_instruction_set_sources() const;

        std::size_t number_of_language_frontend_sources() const;

        std::size_t number_of_language_sources() const;

        std::size_t number_of_sources() const;

        std::size_t number_of_supported_disassemblers() const;

        std::size_t
        number_of_supported_disassemblers(Harnesses::MachineCode::Commons::MachineCodeFrontend& source) const;

        std::size_t number_of_supported_disassemblers(Harnesses::MachineCode::Commons::InstructionSet& source) const;

        std::size_t number_of_inbound_language_compilers() const;

        std::size_t number_of_inbound_language_compilers(LanguageFrontend& source) const;

        std::size_t number_of_inbound_language_compilers(Language& source) const;

        std::size_t number_of_languages_which_can_target() const;

        std::size_t number_of_machine_code_frontends_which_can_target() const;

        std::size_t number_of_instruction_sets_which_can_target() const;

        std::size_t number_of_runtimes() const;

        std::size_t number_of_optimizers() const;


        static std::optional<std::reference_wrapper<LanguageFrontend>> get(const std::string& language_name,
                                                                           const std::string& language_frontend_name);

        std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
        machine_code_frontend_target(const std::string& instruction_set_name,
                                     const std::string& machine_code_frontend_name) const;

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
        assemblers_capable_of_targeting_machine_code_frontend(const std::string& instruction_set_name,
                                                              const std::string& machine_code_frontend_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
        assemblers_capable_of_targeting_machine_code_frontend(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
        assemblers_capable_of_targeting(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
        assemblers_capable_of_targeting_instruction_set(const std::string& instruction_set_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
        assemblers_capable_of_targeting_instruction_set(
            const Harnesses::MachineCode::Commons::InstructionSet& instruction_set);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
        assemblers_capable_of_targeting(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set);

        std::optional<std::reference_wrapper<LanguageFrontend>>
        language_frontend_target(const std::string& language_name, const std::string& language_frontend_name) const;

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_language_frontend(const std::string& language_name,
                                                                  const std::string& language_frontend_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_language_frontend(const LanguageFrontend& language_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting(const LanguageFrontend& language_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_language(const std::string& language_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_language(const Language& language);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting(const Language& language);

        std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
        machine_code_frontend_source(const std::string& instruction_set_name,
                                     const std::string& machine_code_frontend_name) const;

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
        disassemblers_capable_of_targeting_from_machine_code_frontend(const std::string& instruction_set_name,
                                                                      const std::string& machine_code_frontend_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
        disassemblers_capable_of_targeting_from_machine_code_frontend(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
        disassemblers_capable_of_targeting_from(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
        disassemblers_capable_of_targeting_from_instruction_set(const std::string& instruction_set_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
        disassemblers_capable_of_targeting_from_instruction_set(
            const Harnesses::MachineCode::Commons::InstructionSet& instruction_set);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
        disassemblers_capable_of_targeting_from(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set);

        std::optional<std::reference_wrapper<LanguageFrontend>>
        language_frontend_source(const std::string& language_name, const std::string& language_frontend_name) const;

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_from_language_frontend(const std::string& language_name,
                                                                       const std::string& language_frontend_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_from_language_frontend(const LanguageFrontend& language_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_from(const LanguageFrontend& language_frontend);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_from_language(const std::string& language_name);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_from_language(const Language& language);

        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
        language_compilers_capable_of_targeting_from(const Language& language);

        std::optional<std::reference_wrapper<Execution::Commons::LanguageRuntime>>
        runtime(const std::string& runtime_name) const;

        std::optional<std::reference_wrapper<Compilation::Commons::Optimizer>>
        optimizer(const std::string& optimizer_name) const;

        bool can_target_machine_code_frontend(const std::string& instruction_set_name,
                                              const std::string& machine_code_frontend_name) const;

        bool can_target_machine_code_frontend(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const;

        bool can_target(const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const;

        bool can_target_instruction_set(const std::string& instruction_set_name) const;

        bool can_target_instruction_set(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const;

        bool can_target(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const;

        bool can_target_language_frontend(const std::string& language_name,
                                          const std::string& language_frontend_name) const;

        bool can_target_language_frontend(const LanguageFrontend& language_frontend) const;

        bool can_target(const LanguageFrontend& language_frontend) const;

        bool can_target_language(const std::string& language_name) const;

        bool can_target_language(const Language& language) const;

        bool can_target(const Language& language) const;

        bool can_be_targeted_by_machine_code_frontend(const std::string& instruction_set_name,
                                                      const std::string& machine_code_frontend_name) const;

        bool can_be_targeted_by_machine_code_frontend(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const;

        bool
        can_be_targeted_by(const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const;

        bool can_be_targeted_by_instruction_set(const std::string& instruction_set_name) const;

        bool can_be_targeted_by_instruction_set(
            const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const;

        bool can_be_targeted_by(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const;

        bool can_be_targeted_by_language_frontend(const std::string& language_name,
                                                  const std::string& language_frontend_name) const;

        bool can_be_targeted_by_language_frontend(const LanguageFrontend& language_frontend) const;

        bool can_be_targeted_by(const LanguageFrontend& language_frontend) const;

        bool can_be_targeted_by_language(const std::string& language_name) const;

        bool can_be_targeted_by_language(const Language& language) const;

        bool can_be_targeted_by(const Language& language) const;

        bool runtime_supports_frontend(const std::string& runtime_name) const;

        bool runtime_supports_frontend(const Execution::Commons::LanguageRuntime& runtime) const;

        bool optimizer_supports_frontend(const std::string& optimizer_name) const;

        bool optimizer_supports_frontend(const Compilation::Commons::Optimizer& optimizer) const;

        friend bool operator==(const LanguageFrontend& lhs, const LanguageFrontend& rhs)
        {
            return &lhs == &rhs;
        }

        friend bool operator==(const LanguageFrontend& lhs, const std::string& rhs)
        {
            return lhs.name_ == rhs;
        }

        friend bool operator==(const std::string& lhs, const LanguageFrontend& rhs)
        {
            return lhs == rhs.name_;
        }

        ~LanguageFrontend();

        friend class ::SoftOmni::Execution::Commons::LanguageRuntime;

        friend class ::SoftOmni::Compilation::Commons::LanguageCompiler;

        friend class ::SoftOmni::Compilation::Commons::Assembler;

        friend class ::SoftOmni::Compilation::Commons::Disassembler;

        friend class ::SoftOmni::Compilation::Commons::Optimizer;

        class LanguageFrontendAlreadyRegisteredException : std::exception
        {
            std::string name_;

            const Language* language_;

            std::string message_;

            explicit LanguageFrontendAlreadyRegisteredException(const std::string& name, const Language& language);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const LanguageFrontend& existing_language_frontend() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend LanguageFrontend;

        private:
            std::string generate_message(const std::string& name) const;
        };

        class LanguageNotRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit LanguageNotRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend LanguageFrontend;

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

            friend class LanguageFrontend;
        };

        template <typename key, typename return_value>
        class UnorderedSetIterator
        {
            std::unordered_set<key>* set_;

            std::unordered_set<key>::const_iterator iterator_;

            explicit UnorderedSetIterator(std::unordered_set<key>* set);

        public:
            return_value operator*() const;

            UnorderedSetIterator& operator++();

            UnorderedSetIterator operator++(int offset);

            friend bool operator==(const UnorderedSetIterator& lhs, const UnorderedSetIterator& rhs)
            {
                return lhs.iterator_ == rhs.iterator_;
            }

            friend class LanguageFrontend;
        };

        using MachineCodeFrontendIterator =
            UnorderedMapIterator<Harnesses::MachineCode::Commons::MachineCodeFrontend*,
                                 Compilation::Commons::Assembler*, const Compilation::Commons::Assembler&>;

        using InstructionSetIterator =
            UnorderedMapIterator<Harnesses::MachineCode::Commons::InstructionSet*, Compilation::Commons::Assembler*,
                                 const Compilation::Commons::Assembler&>;

        using AssemblersIterator =
            UnorderedSetIterator<Compilation::Commons::Assembler*, const Compilation::Commons::Assembler&>;

        using LanguageFrontendIterator =
            UnorderedMapIterator<LanguageFrontend*, Compilation::Commons::LanguageCompiler*,
                                 const Compilation::Commons::LanguageCompiler&>;

        using LanguageIterator = UnorderedMapIterator<Language*, Compilation::Commons::LanguageCompiler*,
                                                      const Compilation::Commons::LanguageCompiler&>;

        using LanguageCompilersIterator = UnorderedSetIterator<Compilation::Commons::LanguageCompiler*,
                                                               const Compilation::Commons::LanguageCompiler&>;

        using DisassemblersIterator =
            UnorderedSetIterator<Compilation::Commons::Disassembler*, const Compilation::Commons::Disassembler&>;

        using RuntimesIterator =
            UnorderedSetIterator<Execution::Commons::LanguageRuntime*, const Execution::Commons::LanguageRuntime&>;

        using OptimizersIterator =
            UnorderedSetIterator<Compilation::Commons::Optimizer*, const Compilation::Commons::Optimizer&>;

    public:
        MachineCodeFrontendIterator targetable_machine_code_frontends_begin() const;

        MachineCodeFrontendIterator targetable_machine_code_frontends_end() const;

        InstructionSetIterator targetable_instruction_sets_begin() const;

        InstructionSetIterator targetable_instruction_sets_end() const;

        AssemblersIterator assemblers_begin() const;

        AssemblersIterator assemblers_end() const;

        LanguageFrontendIterator targetable_language_frontends_begin() const;

        LanguageFrontendIterator targetable_language_frontends_end() const;

        LanguageIterator targetable_languages_begin() const;

        LanguageIterator targetable_languages_end() const;

        LanguageCompilersIterator language_compilers_begin() const;

        LanguageCompilersIterator language_compilers_end() const;

        MachineCodeFrontendIterator machine_code_frontends_capable_of_targeting_begin() const;

        MachineCodeFrontendIterator machine_code_frontends_capable_of_targeting_end() const;

        InstructionSetIterator instruction_sets_capable_of_targeting_begin() const;

        InstructionSetIterator instruction_sets_capable_of_targeting_end() const;

        DisassemblersIterator disassemblers_begin() const;

        DisassemblersIterator disassemblers_end() const;

        LanguageFrontendIterator language_frontends_capable_of_targeting_begin() const;

        LanguageFrontendIterator language_frontends_capable_of_targeting_end() const;

        LanguageIterator languages_capable_of_targeting_begin() const;

        LanguageIterator languages_capable_of_targeting_end() const;

        // Exec/opt
        RuntimesIterator runtimes_begin() const;

        RuntimesIterator runtimes_end() const;

        OptimizersIterator optimizers_begin() const;

        OptimizersIterator optimizers_end() const;

    private:
        void setup_language_frontend();

        static LanguageFrontendAlreadyRegisteredException
        generate_language_frontend_already_registered_exception(const std::string& name, const Language& language);

        static LanguageNotRegisteredException generate_language_not_registered_exception(const std::string& name);
    };
} // namespace SoftOmni::Parsing::Commons::Kernels

#endif // SOFTOMNISWIFT_LANGUAGE_FRONTEND_HH
