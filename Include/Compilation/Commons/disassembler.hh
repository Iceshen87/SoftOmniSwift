
#ifndef SOFTOMNISWIFT_DISASSEMBLER_HH
#define SOFTOMNISWIFT_DISASSEMBLER_HH

#include <Include/Harnesses/MachineCode/Commons/machine_code_frontend.hh>
#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

namespace SoftOmni::Compilation::Commons
{
    class Disassembler
    {
        std::string name_;

        std::string display_name_;

        Harnesses::MachineCode::Commons::MachineCodeFrontend* source_machine_code_frontend_;

        Parsing::Commons::Kernels::LanguageFrontend* target_language_frontend_;

        static std::unordered_map<std::string, Disassembler*> disassemblers_;

    protected:
        Disassembler(const std::string& name,
                     Harnesses::MachineCode::Commons::MachineCodeFrontend& source_machine_code_frontend,
                     Parsing::Commons::Kernels::LanguageFrontend& target_language_frontend);

        Disassembler(std::string name, std::string display_name,
                     Harnesses::MachineCode::Commons::MachineCodeFrontend& source_machine_code_frontend,
                     Parsing::Commons::Kernels::LanguageFrontend& target_language_frontend);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(const std::string_view& new_display_name);

        const Harnesses::MachineCode::Commons::MachineCodeFrontend& source_frontend() const;

        const Parsing::Commons::Kernels::LanguageFrontend& target_frontend() const;

        virtual void disassemble() = 0;

        virtual ~Disassembler();

        class DisassemblerAlreadyRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit DisassemblerAlreadyRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const Disassembler& get_already_existing_disassembler() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class Disassembler;

        private:
            static std::string generate_message(const std::string& name);
        };

        struct DisassemblerRefHash
        {
            std::size_t operator()(const std::reference_wrapper<Disassembler>& assembler_reference) const noexcept;
        };

        struct DisassemblerRefEq
        {
            bool operator()(const std::reference_wrapper<Disassembler>& lhs,
                            const std::reference_wrapper<Disassembler>& rhs) const;
        };

    private:
        void setup_frontends();

        static DisassemblerAlreadyRegisteredException
        generate_disassembler_already_registered_exception(const std::string& name);
    };
} // namespace SoftOmni::Compilation::Commons

#endif // SOFTOMNISWIFT_DISASSEMBLER_HH
