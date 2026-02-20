
#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

#include <algorithm>

#include <Include/Execution/Commons/language_runtime.hh>

namespace SoftOmni::Parsing::Commons::Kernels
{
    LanguageFrontend::LanguageFrontend(const std::string& name, const std::string& language)
    {
        const std::optional<std::reference_wrapper<Language>> language_object = Language::get(language);

        if (!language_object.has_value())
        {
            throw generate_language_not_registered_exception(language);
        }

        const std::reference_wrapper<Language> language_reference = language_object.value();

        if (const std::optional<std::reference_wrapper<LanguageFrontend>>& language_frontend_object =
                language_reference.get()[name];
            language_frontend_object.has_value())
        {
            throw generate_language_frontend_already_registered_exception(name, language_reference.get());
        }

        name_ = name;
        display_name_ = name;

        language_ = &language_reference.get();
        language_frontends_[language_].try_emplace(name_, this);
        language_->language_frontends_.try_emplace(name_, this);
    }

    LanguageFrontend::LanguageFrontend(const std::string& name, Language& language) : language_(&language)
    {
        if (language.language_frontends_.contains(name))
        {
            throw generate_language_frontend_already_registered_exception(name, language);
        }

        name_ = name;
        display_name_ = name;

        language_frontends_[&language].try_emplace(name_, this);
        language.language_frontends_.try_emplace(name_, this);
    }

    LanguageFrontend::LanguageFrontend(std::string name, std::string display_name, const std::string& language)
    {
        const std::optional<std::reference_wrapper<Language>> language_object = Language::get(language);

        if (!language_object.has_value())
        {
            throw generate_language_not_registered_exception(language);
        }

        const std::reference_wrapper<Language> language_reference = language_object.value();

        if (const std::optional<std::reference_wrapper<LanguageFrontend>>& language_frontend_object =
                language_reference.get()[name];
            language_frontend_object.has_value())
        {
            throw generate_language_frontend_already_registered_exception(name, language_reference.get());
        }

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        language_ = &language_reference.get();
        language_frontends_[language_].try_emplace(name_, this);
        language_->language_frontends_.try_emplace(name_, this);
    }

    LanguageFrontend::LanguageFrontend(std::string name, std::string display_name, Language& language) :
        language_(&language)
    {
        if (language.language_frontends_.contains(name))
        {
            throw generate_language_frontend_already_registered_exception(name, language);
        }

        name_ = std::move(name);
        display_name_ = std::move(display_name);

        language_frontends_[&language].try_emplace(name_, this);
        language.language_frontends_.try_emplace(name_, this);
    }

    const std::string& LanguageFrontend::name() const
    {
        return name_;
    }

    const std::string& LanguageFrontend::display_name() const
    {
        return display_name_;
    }

    void LanguageFrontend::set_display_name(const std::string_view& new_display_name)
    {
        display_name_ = new_display_name;
    }

    const Language& LanguageFrontend::language() const
    {
        return *language_;
    }

    std::size_t LanguageFrontend::number_of_machine_code_frontend_targets() const
    {
        return capable_machine_code_frontend_targets_.size();
    }

    std::size_t LanguageFrontend::number_of_instruction_set_targets() const
    {
        return capable_instruction_set_targets_.size();
    }

    std::size_t LanguageFrontend::number_of_language_frontend_targets() const
    {
        return capable_language_frontend_targets_.size();
    }

    std::size_t LanguageFrontend::number_of_language_targets() const
    {
        return capable_language_targets_.size();
    }

    std::size_t LanguageFrontend::number_of_targets() const
    {
        return number_of_machine_code_frontend_targets() + number_of_language_frontend_targets();
    }

    std::size_t LanguageFrontend::number_of_supported_assemblers() const
    {
        return assemblers_.size();
    }

    std::size_t
    LanguageFrontend::number_of_supported_assemblers(Harnesses::MachineCode::Commons::MachineCodeFrontend& target) const
    {
        if (!capable_machine_code_frontend_targets_.contains(&target))
        {
            return 0;
        }

        return capable_machine_code_frontend_targets_.at(&target).size();
    }

    std::size_t
    LanguageFrontend::number_of_supported_assemblers(Harnesses::MachineCode::Commons::InstructionSet& target) const
    {
        if (!capable_instruction_set_targets_.contains(&target))
        {
            return 0;
        }

        return capable_instruction_set_targets_.at(&target).size();
    }

    std::size_t LanguageFrontend::number_of_supported_outbound_language_compilers() const
    {
        return targeting_language_compilers_.size();
    }

    std::size_t LanguageFrontend::number_of_supported_outbound_language_compilers(LanguageFrontend& target) const
    {
        if (!capable_language_frontend_targets_.contains(&target))
        {
            return 0;
        }

        return capable_language_frontend_targets_.at(&target).size();
    }

    std::size_t LanguageFrontend::number_of_supported_outbound_language_compilers(Language& target) const
    {
        if (!capable_language_targets_.contains(&target))
        {
            return 0;
        }

        return capable_language_targets_.at(&target).size();
    }

    std::size_t LanguageFrontend::number_of_machine_code_frontend_sources() const
    {
        return machine_code_frontends_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_instruction_set_sources() const
    {
        return instruction_sets_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_language_frontend_sources() const
    {
        return language_frontends_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_language_sources() const
    {
        return languages_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_sources() const
    {
        return number_of_machine_code_frontend_sources() + number_of_language_frontend_sources();
    }

    std::size_t LanguageFrontend::number_of_supported_disassemblers() const
    {
        return disassemblers_.size();
    }

    std::size_t LanguageFrontend::number_of_supported_disassemblers(
        Harnesses::MachineCode::Commons::MachineCodeFrontend& source) const
    {
        if (!machine_code_frontends_capable_of_targeting_.contains(&source))
        {
            return 0;
        }

        return machine_code_frontends_capable_of_targeting_.at(&source).size();
    }

    std::size_t
    LanguageFrontend::number_of_supported_disassemblers(Harnesses::MachineCode::Commons::InstructionSet& source) const
    {
        if (!instruction_sets_capable_of_targeting_.contains(&source))
        {
            return 0;
        }

        return instruction_sets_capable_of_targeting_.at(&source).size();
    }

    std::size_t LanguageFrontend::number_of_inbound_language_compilers() const
    {
        return source_language_compilers_.size();
    }

    std::size_t LanguageFrontend::number_of_inbound_language_compilers(LanguageFrontend& source) const
    {
        if (!language_frontends_capable_of_targeting_.contains(&source))
        {
            return 0;
        }

        return language_frontends_capable_of_targeting_.at(&source).size();
    }

    std::size_t LanguageFrontend::number_of_inbound_language_compilers(Language& source) const
    {
        if (!languages_capable_of_targeting_.contains(&source))
        {
            return 0;
        }

        return languages_capable_of_targeting_.at(&source).size();
    }

    std::size_t LanguageFrontend::number_of_languages_which_can_target() const
    {
        return languages_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_machine_code_frontends_which_can_target() const
    {
        return machine_code_frontends_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_instruction_sets_which_can_target() const
    {
        return instruction_sets_capable_of_targeting_.size();
    }

    std::size_t LanguageFrontend::number_of_runtimes() const
    {
        return runtimes_.size();
    }

    std::size_t LanguageFrontend::number_of_optimizers() const
    {
        return optimizers_.size();
    }

    std::optional<std::reference_wrapper<LanguageFrontend>>
    LanguageFrontend::get(const std::string& language_name, const std::string& language_frontend_name)
    {
        const std::optional<std::reference_wrapper<Language>> potential_language = Language::get(language_name);

        if (!potential_language.has_value())
        {
            return std::nullopt;
        }

        const std::reference_wrapper<Language> language_reference = potential_language.value();
        const std::optional<std::reference_wrapper<LanguageFrontend>>& language_frontend_object =
            language_reference.get()[language_frontend_name];

        return language_frontend_object;
    }

    std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
    machine_code_frontend_helper(const std::string& instruction_set_name, const std::string& machine_code_frontend_name)
    {
        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet>>
            potential_instruction_set = Harnesses::MachineCode::Commons::InstructionSet::get(instruction_set_name);

        if (!potential_instruction_set.has_value())
        {
            return std::nullopt;
        }

        const std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet> instruction_set_reference =
            potential_instruction_set.value();

        return instruction_set_reference.get().operator[](machine_code_frontend_name);
    }

    std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
    LanguageFrontend::machine_code_frontend_target(const std::string& instruction_set_name,
                                                   const std::string& machine_code_frontend_name) const
    {
        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
            potential_machine_code_frontend =
                machine_code_frontend_helper(instruction_set_name, machine_code_frontend_name);

        if (!potential_machine_code_frontend.has_value())
        {
            return std::nullopt;
        }

        if (!capable_machine_code_frontend_targets_.contains(&potential_machine_code_frontend.value().get()))
        {
            return std::nullopt;
        }

        return potential_machine_code_frontend;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                       Compilation::Commons::Assembler::AssemblerRefHash,
                       Compilation::Commons::Assembler::AssemblerRefEq>
    LanguageFrontend::assemblers_capable_of_targeting_machine_code_frontend(
        const std::string& instruction_set_name, const std::string& machine_code_frontend_name)
    {
        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
            potential_machine_code_frontend =
                machine_code_frontend_target(instruction_set_name, machine_code_frontend_name);

        if (!potential_machine_code_frontend.has_value())
        {
            return std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                                      Compilation::Commons::Assembler::AssemblerRefHash,
                                      Compilation::Commons::Assembler::AssemblerRefEq>();
        }

        const std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend> machine_code_frontend =
            potential_machine_code_frontend.value();
        return assemblers_capable_of_targeting_machine_code_frontend(machine_code_frontend);
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                       Compilation::Commons::Assembler::AssemblerRefHash,
                       Compilation::Commons::Assembler::AssemblerRefEq>
    LanguageFrontend::assemblers_capable_of_targeting_machine_code_frontend(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
            assemblers;

        if (!capable_machine_code_frontend_targets_.contains(&machine_code_frontend))
        {
            return assemblers;
        }

        for (Compilation::Commons::Assembler* assembler :
             capable_machine_code_frontend_targets_[&machine_code_frontend])
        {
            assemblers.emplace(std::ref(*assembler));
        }

        return assemblers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                       Compilation::Commons::Assembler::AssemblerRefHash,
                       Compilation::Commons::Assembler::AssemblerRefEq>
    LanguageFrontend::assemblers_capable_of_targeting(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        return assemblers_capable_of_targeting_machine_code_frontend(machine_code_frontend);
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                       Compilation::Commons::Assembler::AssemblerRefHash,
                       Compilation::Commons::Assembler::AssemblerRefEq>
    LanguageFrontend::assemblers_capable_of_targeting_instruction_set(const std::string& instruction_set_name)
    {
        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet>>
            instruction_set_object = Harnesses::MachineCode::Commons::InstructionSet::get(instruction_set_name);

        if (!instruction_set_object.has_value())
        {
            return std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                                      Compilation::Commons::Assembler::AssemblerRefHash,
                                      Compilation::Commons::Assembler::AssemblerRefEq>();
        }

        const std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet> instruction_set_reference =
            instruction_set_object.value();

        return assemblers_capable_of_targeting_instruction_set(instruction_set_reference.get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                       Compilation::Commons::Assembler::AssemblerRefHash,
                       Compilation::Commons::Assembler::AssemblerRefEq>
    LanguageFrontend::assemblers_capable_of_targeting_instruction_set(
        const Harnesses::MachineCode::Commons::InstructionSet& instruction_set)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                           Compilation::Commons::Assembler::AssemblerRefHash,
                           Compilation::Commons::Assembler::AssemblerRefEq>
            assemblers;

        if (!capable_instruction_set_targets_.contains(&instruction_set))
        {
            return assemblers;
        }

        for (Compilation::Commons::Assembler* assembler : capable_instruction_set_targets_[&instruction_set])
        {
            assemblers.emplace(std::ref(*assembler));
        }

        return assemblers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Assembler>,
                       Compilation::Commons::Assembler::AssemblerRefHash,
                       Compilation::Commons::Assembler::AssemblerRefEq>
    LanguageFrontend::assemblers_capable_of_targeting(
        const Harnesses::MachineCode::Commons::InstructionSet& instruction_set)
    {
        return assemblers_capable_of_targeting_instruction_set(instruction_set);
    }

    std::optional<std::reference_wrapper<LanguageFrontend>>
    LanguageFrontend::language_frontend_target(const std::string& language_name,
                                               const std::string& language_frontend_name) const
    {
        const std::optional<std::reference_wrapper<Language>> language_object = Language::get(language_name);

        if (!language_object.has_value())
        {
            return std::nullopt;
        }

        const std::reference_wrapper<Language> language_reference = language_object.value();

        const std::optional<std::reference_wrapper<LanguageFrontend>> frontend_object =
            language_reference.get().operator[](language_frontend_name);
        if (!frontend_object.has_value())
        {
            return std::nullopt;
        }

        const std::reference_wrapper<LanguageFrontend> frontend_reference = frontend_object.value();
        if (!capable_language_frontend_targets_.contains(&frontend_reference.get()))
        {
            return std::nullopt;
        }

        return frontend_reference;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_language_frontend(
        const std::string& language_name, const std::string& language_frontend_name)
    {
        const std::optional<std::reference_wrapper<LanguageFrontend>> potential_frontend =
            language_frontend_target(language_name, language_frontend_name);

        if (!potential_frontend.has_value())
        {
            std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
                compilers;
            return compilers;
        }

        return language_compilers_capable_of_targeting_language_frontend(potential_frontend.value().get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_language_frontend(
        const LanguageFrontend& language_frontend)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
            compilers;

        if (!capable_language_frontend_targets_.contains(&language_frontend))
        {
            return compilers;
        }

        for (Compilation::Commons::LanguageCompiler* compiler : capable_language_frontend_targets_[&language_frontend])
        {
            compilers.emplace(std::ref(*compiler));
        }

        return compilers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting(const LanguageFrontend& language_frontend)
    {
        return language_compilers_capable_of_targeting_language_frontend(language_frontend);
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_language(const std::string& language_name)
    {

        const std::optional<std::reference_wrapper<Language>> potential_language = Language::get(language_name);

        if (!potential_language.has_value())
        {
            std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
                compilers;
            return compilers;
        }

        return language_compilers_capable_of_targeting_language(potential_language.value().get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_language(const Language& language)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
            compilers;

        if (!capable_language_targets_.contains(&language))
        {
            return compilers;
        }

        for (Compilation::Commons::LanguageCompiler* compiler : capable_language_targets_[&language])
        {
            compilers.emplace(std::ref(*compiler));
        }

        return compilers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting(const Language& language)
    {
        return language_compilers_capable_of_targeting_language(language);
    }

    std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
    LanguageFrontend::machine_code_frontend_source(const std::string& instruction_set_name,
                                                   const std::string& machine_code_frontend_name) const
    {
        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
            machine_code_frontend_reference =
                machine_code_frontend_helper(instruction_set_name, machine_code_frontend_name);

        if (!machine_code_frontend_reference.has_value())
        {
            return std::nullopt;
        }

        if (!machine_code_frontends_capable_of_targeting_.contains(&machine_code_frontend_reference.value().get()))
        {
            return std::nullopt;
        }

        return machine_code_frontend_reference;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                       Compilation::Commons::Disassembler::DisassemblerRefHash,
                       Compilation::Commons::Disassembler::DisassemblerRefEq>
    LanguageFrontend::disassemblers_capable_of_targeting_from_machine_code_frontend(
        const std::string& instruction_set_name, const std::string& machine_code_frontend_name)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
            disassemblers;

        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::MachineCodeFrontend>>
            potential_machine_code_frontend =
                machine_code_frontend_source(instruction_set_name, machine_code_frontend_name);

        if (!potential_machine_code_frontend.has_value())
        {
            return disassemblers;
        }

        return disassemblers_capable_of_targeting_from_machine_code_frontend(
            potential_machine_code_frontend.value().get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                       Compilation::Commons::Disassembler::DisassemblerRefHash,
                       Compilation::Commons::Disassembler::DisassemblerRefEq>
    LanguageFrontend::disassemblers_capable_of_targeting_from_machine_code_frontend(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
            disassemblers;

        if (!machine_code_frontends_capable_of_targeting_.contains(&machine_code_frontend))
        {
            return disassemblers;
        }

        for (Compilation::Commons::Disassembler* disassembler :
             machine_code_frontends_capable_of_targeting_[&machine_code_frontend])
        {
            disassemblers.emplace(std::ref(*disassembler));
        }

        return disassemblers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                       Compilation::Commons::Disassembler::DisassemblerRefHash,
                       Compilation::Commons::Disassembler::DisassemblerRefEq>
    LanguageFrontend::disassemblers_capable_of_targeting_from(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend)
    {
        return disassemblers_capable_of_targeting_from_machine_code_frontend(machine_code_frontend);
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                       Compilation::Commons::Disassembler::DisassemblerRefHash,
                       Compilation::Commons::Disassembler::DisassemblerRefEq>
    LanguageFrontend::disassemblers_capable_of_targeting_from_instruction_set(const std::string& instruction_set_name)
    {

        std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet>> instruction_set =
            Harnesses::MachineCode::Commons::InstructionSet::get(instruction_set_name);

        if (!instruction_set.has_value())
        {
            std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                               Compilation::Commons::Disassembler::DisassemblerRefHash,
                               Compilation::Commons::Disassembler::DisassemblerRefEq>
                disassemblers;
            return disassemblers;
        }

        return disassemblers_capable_of_targeting_from_instruction_set(instruction_set.value().get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                       Compilation::Commons::Disassembler::DisassemblerRefHash,
                       Compilation::Commons::Disassembler::DisassemblerRefEq>
    LanguageFrontend::disassemblers_capable_of_targeting_from_instruction_set(
        const Harnesses::MachineCode::Commons::InstructionSet& instruction_set)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                           Compilation::Commons::Disassembler::DisassemblerRefHash,
                           Compilation::Commons::Disassembler::DisassemblerRefEq>
            disassemblers;

        if (!instruction_sets_capable_of_targeting_.contains(&instruction_set))
        {
            return disassemblers;
        }

        for (Compilation::Commons::Disassembler* disassembler :
             instruction_sets_capable_of_targeting_[&instruction_set])
        {
            disassemblers.emplace(std::ref(*disassembler));
        }

        return disassemblers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::Disassembler>,
                       Compilation::Commons::Disassembler::DisassemblerRefHash,
                       Compilation::Commons::Disassembler::DisassemblerRefEq>
    LanguageFrontend::disassemblers_capable_of_targeting_from(
        const Harnesses::MachineCode::Commons::InstructionSet& instruction_set)
    {
        return disassemblers_capable_of_targeting_from_instruction_set(instruction_set);
    }

    std::optional<std::reference_wrapper<LanguageFrontend>>
    LanguageFrontend::language_frontend_source(const std::string& language_name,
                                               const std::string& language_frontend_name) const
    {
        const std::optional<std::reference_wrapper<Language>> potential_language = Language::get(language_name);

        if (!potential_language.has_value())
        {
            return std::nullopt;
        }

        const std::optional<std::reference_wrapper<LanguageFrontend>> potential_language_frontend =
            potential_language.value().get()[language_frontend_name];

        if (!potential_language_frontend.has_value())
        {
            return std::nullopt;
        }

        if (!language_frontends_capable_of_targeting_.contains(&potential_language_frontend.value().get()))
        {
            return std::nullopt;
        }

        return potential_language_frontend;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_from_language_frontend(
        const std::string& language_name, const std::string& language_frontend_name)
    {
        const std::optional<std::reference_wrapper<LanguageFrontend>> potential_language_frontend =
            language_frontend_source(language_name, language_frontend_name);

        if (!potential_language_frontend.has_value())
        {
            std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
                compilers;
            return compilers;
        }

        return language_compilers_capable_of_targeting_from_language_frontend(
            potential_language_frontend.value().get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_from_language_frontend(
        const LanguageFrontend& language_frontend)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
            compilers;

        if (!language_frontends_capable_of_targeting_.contains(&language_frontend))
        {
            return compilers;
        }

        for (Compilation::Commons::LanguageCompiler* compiler :
             language_frontends_capable_of_targeting_[&language_frontend])
        {
            compilers.emplace(std::ref(*compiler));
        }

        return compilers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_from(const LanguageFrontend& language_frontend)
    {
        return language_compilers_capable_of_targeting_from_language_frontend(language_frontend);
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_from_language(const std::string& language_name)
    {
        const std::optional<std::reference_wrapper<Language>> potential_language = Language::get(language_name);

        if (!potential_language.has_value())
        {
            std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                               Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
                compilers;
            return compilers;
        }

        return language_compilers_capable_of_targeting_from_language(potential_language.value().get());
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_from_language(const Language& language)
    {
        std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                           Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
            compilers;

        if (!languages_capable_of_targeting_.contains(&language))
        {
            return compilers;
        }

        for (Compilation::Commons::LanguageCompiler* compiler : languages_capable_of_targeting_[&language])
        {
            compilers.emplace(std::ref(*compiler));
        }

        return compilers;
    }

    std::unordered_set<std::reference_wrapper<Compilation::Commons::LanguageCompiler>,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefHash,
                       Compilation::Commons::LanguageCompiler::LanguageCompilerRefEq>
    LanguageFrontend::language_compilers_capable_of_targeting_from(const Language& language)
    {
        return language_compilers_capable_of_targeting_from_language(language);
    }

    std::optional<std::reference_wrapper<Execution::Commons::LanguageRuntime>>
    LanguageFrontend::runtime(const std::string& runtime_name) const
    {
        if (!runtimes_.contains(runtime_name))
        {
            return std::nullopt;
        }

        return std::ref(*runtimes_.at(runtime_name));
    }

    std::optional<std::reference_wrapper<Compilation::Commons::Optimizer>>
    LanguageFrontend::optimizer(const std::string& optimizer_name) const
    {
        if (!optimizers_.contains(optimizer_name))
        {
            return std::nullopt;
        }

        return std::ref(*optimizers_.at(optimizer_name));
    }

    bool LanguageFrontend::can_target_machine_code_frontend(const std::string& instruction_set_name,
                                                            const std::string& machine_code_frontend_name) const
    {
        return machine_code_frontend_target(instruction_set_name, machine_code_frontend_name).has_value();
    }

    bool LanguageFrontend::can_target_machine_code_frontend(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const
    {
        return capable_machine_code_frontend_targets_.contains(&machine_code_frontend);
    }

    bool LanguageFrontend::can_target(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const
    {
        return can_target_machine_code_frontend(machine_code_frontend);
    }

    bool LanguageFrontend::can_target_instruction_set(const std::string& instruction_set_name) const
    {
        std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet>> instruction_set =
            Harnesses::MachineCode::Commons::InstructionSet::get(instruction_set_name);

        if (!instruction_set.has_value())
        {
            return false;
        }

        return capable_instruction_set_targets_.contains(&instruction_set.value().get());
    }

    bool LanguageFrontend::can_target_instruction_set(
        const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const
    {
        return capable_instruction_set_targets_.contains(&instruction_set);
    }

    bool LanguageFrontend::can_target(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const
    {
        return can_target_instruction_set(instruction_set);
    }

    bool LanguageFrontend::can_target_language_frontend(const std::string& language_name,
                                                        const std::string& language_frontend_name) const
    {
        return language_frontend_target(language_name, language_frontend_name).has_value();
    }

    bool LanguageFrontend::can_target_language_frontend(const LanguageFrontend& language_frontend) const
    {
        return capable_language_frontend_targets_.contains(&language_frontend);
    }

    bool LanguageFrontend::can_target(const LanguageFrontend& language_frontend) const
    {
        return can_target_language_frontend(language_frontend);
    }

    bool LanguageFrontend::can_target_language(const std::string& language_name) const
    {
        const std::optional<std::reference_wrapper<Language>> potential_language = Language::get(language_name);

        if (!potential_language.has_value())
        {
            return false;
        }

        return capable_language_targets_.contains(&potential_language.value().get());
    }

    bool LanguageFrontend::can_target_language(const Language& language) const
    {
        return capable_language_targets_.contains(&language);
    }

    bool LanguageFrontend::can_target(const Language& language) const
    {
        return can_target_language(language);
    }

    bool LanguageFrontend::can_be_targeted_by_machine_code_frontend(const std::string& instruction_set_name,
                                                                    const std::string& machine_code_frontend_name) const
    {
        return machine_code_frontend_source(instruction_set_name, machine_code_frontend_name).has_value();
    }

    bool LanguageFrontend::can_be_targeted_by_machine_code_frontend(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const
    {
        return machine_code_frontends_capable_of_targeting_.contains(&machine_code_frontend);
    }

    bool LanguageFrontend::can_be_targeted_by(
        const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const
    {
        return can_be_targeted_by_machine_code_frontend(machine_code_frontend);
    }

    bool LanguageFrontend::can_be_targeted_by_instruction_set(const std::string& instruction_set_name) const
    {
        const std::optional<std::reference_wrapper<Harnesses::MachineCode::Commons::InstructionSet>>
            potential_instruction_set = Harnesses::MachineCode::Commons::InstructionSet::get(instruction_set_name);

        if (potential_instruction_set.has_value())
        {
            return false;
        }

        return instruction_sets_capable_of_targeting_.contains(&potential_instruction_set.value().get());
    }

    bool LanguageFrontend::can_be_targeted_by_instruction_set(
        const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const
    {
        return instruction_sets_capable_of_targeting_.contains(&instruction_set);
    }

    bool
    LanguageFrontend::can_be_targeted_by(const Harnesses::MachineCode::Commons::InstructionSet& instruction_set) const
    {
        return can_be_targeted_by_instruction_set(instruction_set);
    }

    bool LanguageFrontend::can_be_targeted_by_language_frontend(const std::string& language_name,
                                                                const std::string& language_frontend_name) const
    {
        return language_frontend_source(language_name, language_frontend_name).has_value();
    }

    bool LanguageFrontend::can_be_targeted_by_language_frontend(const LanguageFrontend& language_frontend) const
    {
        return language_frontends_capable_of_targeting_.contains(&language_frontend);
    }

    bool LanguageFrontend::can_be_targeted_by(const LanguageFrontend& language_frontend) const
    {
        return can_be_targeted_by_language_frontend(language_frontend);
    }

    bool LanguageFrontend::can_be_targeted_by_language(const std::string& language_name) const
    {
        const std::optional<std::reference_wrapper<Language>> potential_language = Language::get(language_name);

        if (!potential_language.has_value())
        {
            return false;
        }

        return languages_capable_of_targeting_.contains(&potential_language.value().get());
    }

    bool LanguageFrontend::can_be_targeted_by_language(const Language& language) const
    {
        return languages_capable_of_targeting_.contains(&language);
    }

    bool LanguageFrontend::can_be_targeted_by(const Language& language) const
    {
        return can_be_targeted_by_language(language);
    }

    bool LanguageFrontend::runtime_supports_frontend(const std::string& runtime_name) const
    {
        return runtimes_.contains(runtime_name);
    }

    bool LanguageFrontend::runtime_supports_frontend(const Execution::Commons::LanguageRuntime& runtime) const
    {
        return runtimes_set_.contains(&runtime);
    }

    bool LanguageFrontend::optimizer_supports_frontend(const std::string& optimizer_name) const
    {
        return optimizers_.contains(optimizer_name);
    }

    bool LanguageFrontend::optimizer_supports_frontend(const Compilation::Commons::Optimizer& optimizer) const
    {
        return optimizers_set_.contains(&optimizer);
    }

    LanguageFrontend::~LanguageFrontend()
    {
        language_frontends_[language_].erase(name_);
        language_->language_frontends_.erase(name_); // TODO: associated elements must also deallocate
    }

    LanguageFrontend::LanguageFrontendAlreadyRegisteredException::LanguageFrontendAlreadyRegisteredException(
        const std::string& name, const Language& language) :
        name_(name), language_(&language), message_(generate_message(name))
    {
    }

    const std::string& LanguageFrontend::LanguageFrontendAlreadyRegisteredException::name() const
    {
        return name_;
    }

    const std::string& LanguageFrontend::LanguageFrontendAlreadyRegisteredException::message() const
    {
        return message_;
    }

    const LanguageFrontend&
    LanguageFrontend::LanguageFrontendAlreadyRegisteredException::existing_language_frontend() const
    {
        return *language_->language_frontends_.at(name_);
    }

    const char* LanguageFrontend::LanguageFrontendAlreadyRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string
    LanguageFrontend::LanguageFrontendAlreadyRegisteredException::generate_message(const std::string& name) const
    {
        return "ERROR: The language frontend '" + name + "' is already registered for language '" + language_->name() +
            "'.\nLanguage frontends must have a unique name for the language they target." +
            " Two language frontends can have the same name if they are for 2 different languages.\n" +
            "Two language frontends can always have the same display name if desired";
    }

    LanguageFrontend::LanguageNotRegisteredException::LanguageNotRegisteredException(const std::string& name) :
        name_(name), message_(generate_message(name))
    {
    }

    const std::string& LanguageFrontend::LanguageNotRegisteredException::name() const
    {
        return name_;
    }

    const std::string& LanguageFrontend::LanguageNotRegisteredException::message() const
    {
        return message_;
    }

    const char* LanguageFrontend::LanguageNotRegisteredException::what() const noexcept
    {
        return message_.c_str();
    }

    std::string LanguageFrontend::LanguageNotRegisteredException::generate_message(const std::string& name)
    {
        return "The language '" + name + "' is not registered";
    }

    template <typename key, typename value, typename return_value>
    LanguageFrontend::UnorderedMapIterator<key, value, return_value>::UnorderedMapIterator(
        std::unordered_map<key, value>* map) : map_(map), iterator_(map->cbegin())
    {
    }

    template <typename key, typename value, typename return_value>
    return_value LanguageFrontend::UnorderedMapIterator<key, value, return_value>::operator*() const
    {
        return *iterator_;
    }

    template <typename key, typename value, typename return_value>
    LanguageFrontend::UnorderedMapIterator<key, value, return_value>&
    LanguageFrontend::UnorderedMapIterator<key, value, return_value>::operator++()
    {
        ++iterator_;
        return this;
    }

    template <typename key, typename value, typename return_value>
    LanguageFrontend::UnorderedMapIterator<key, value, return_value>
    LanguageFrontend::UnorderedMapIterator<key, value, return_value>::operator++(int offset)
    {
        iterator_.operator++(offset);
        return this;
    }

    template <typename key, typename return_value>
    LanguageFrontend::UnorderedSetIterator<key, return_value>::UnorderedSetIterator(std::unordered_set<key>* set)
        : set_(set), iterator_(set->cbegin())
    {
    }

    template <typename key, typename return_value>
    return_value LanguageFrontend::UnorderedSetIterator<key, return_value>::operator*() const
    {
        return *iterator_;
    }

    template <typename key, typename return_value>
    LanguageFrontend::UnorderedSetIterator<key, return_value>&
    LanguageFrontend::UnorderedSetIterator<key, return_value>::operator++()
    {
        ++iterator_;
        return this;
    }

    template <typename key, typename return_value>
    LanguageFrontend::UnorderedSetIterator<key, return_value>
    LanguageFrontend::UnorderedSetIterator<key, return_value>::operator++(int offset)
    {
        iterator_.operator++(offset);
        return this;
    }

    /*LanguageFrontend::MachineCodeFrontendIterator LanguageFrontend::targetable_machine_code_frontends_begin() const
    {

    }

    LanguageFrontend::MachineCodeFrontendIterator LanguageFrontend::targetable_machine_code_frontends_end() const {}

    LanguageFrontend::InstructionSetIterator LanguageFrontend::targetable_instruction_sets_begin() const {}

    LanguageFrontend::InstructionSetIterator LanguageFrontend::targetable_instruction_sets_end() const {}

    LanguageFrontend::AssemblersIterator LanguageFrontend::assemblers_begin() const {}

    LanguageFrontend::AssemblersIterator LanguageFrontend::assemblers_end() const {}

    LanguageFrontend::LanguageFrontendIterator LanguageFrontend::targetable_language_frontends_begin() const {}

    LanguageFrontend::LanguageFrontendIterator LanguageFrontend::targetable_language_frontends_end() const {}

    LanguageFrontend::LanguageIterator LanguageFrontend::targetable_languages_begin() const {}

    LanguageFrontend::LanguageIterator LanguageFrontend::targetable_languages_end() const {}

    LanguageFrontend::LanguageCompilersIterator LanguageFrontend::language_compilers_begin() const {}

    LanguageFrontend::LanguageCompilersIterator LanguageFrontend::language_compilers_end() const {}

    LanguageFrontend::MachineCodeFrontendIterator
    LanguageFrontend::machine_code_frontends_capable_of_targeting_begin() const
    {
        auto var = MachineCodeFrontendIterator(capable_machine_code_frontend_targets_);
    }

    LanguageFrontend::MachineCodeFrontendIterator
    LanguageFrontend::machine_code_frontends_capable_of_targeting_end() const
    {
    }

    LanguageFrontend::InstructionSetIterator LanguageFrontend::instruction_sets_capable_of_targeting_begin() const {}

    LanguageFrontend::InstructionSetIterator LanguageFrontend::instruction_sets_capable_of_targeting_end() const {}

    LanguageFrontend::DisassemblersIterator LanguageFrontend::disassemblers_begin() const {}

    LanguageFrontend::DisassemblersIterator LanguageFrontend::disassemblers_end() const {}

    LanguageFrontend::LanguageFrontendIterator LanguageFrontend::language_frontends_capable_of_targeting_begin() const
    {
    }

    LanguageFrontend::LanguageFrontendIterator LanguageFrontend::language_frontends_capable_of_targeting_end() const {}

    LanguageFrontend::LanguageIterator LanguageFrontend::languages_capable_of_targeting_begin() const {}

    LanguageFrontend::LanguageIterator LanguageFrontend::languages_capable_of_targeting_end() const {}

    LanguageFrontend::RuntimesIterator LanguageFrontend::runtimes_begin() const {}

    LanguageFrontend::RuntimesIterator LanguageFrontend::runtimes_end() const {}

    LanguageFrontend::OptimizersIterator LanguageFrontend::optimizers_begin() const {}

    LanguageFrontend::OptimizersIterator LanguageFrontend::optimizers_end() const {}

    void LanguageFrontend::setup_language_frontend() {}

    LanguageFrontend::LanguageFrontendAlreadyRegisteredException
    LanguageFrontend::generate_language_frontend_already_registered_exception(const std::string& name,
                                                                              const Language& language)
    {
    }

    LanguageFrontend::LanguageNotRegisteredException
    LanguageFrontend::generate_language_not_registered_exception(const std::string& name)
    {
    }*/

} // namespace SoftOmni::Parsing::Commons::Kernels
