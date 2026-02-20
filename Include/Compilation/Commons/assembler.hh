
#ifndef SOFTOMNISWIFT_ASSEMBLER_HH
#define SOFTOMNISWIFT_ASSEMBLER_HH

#include <Include/Harnesses/MachineCode/Commons/machine_code_frontend.hh>
#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

namespace SoftOmni::Compilation::Commons
{
    class Assembler
    {
        std::string name_;

        std::string display_name_;

        Parsing::Commons::Kernels::LanguageFrontend* source_language_frontend_;

        Harnesses::MachineCode::Commons::MachineCodeFrontend* target_machine_code_frontend_;

        static std::unordered_map<std::string, Assembler*> assemblers_;

    protected:
        Assembler(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& frontend,
                  Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        Assembler(std::string name, std::string display_name, Parsing::Commons::Kernels::LanguageFrontend& frontend,
                  Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

    public:

        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(const std::string& new_display_name);

        [[nodiscard]] const Parsing::Commons::Kernels::LanguageFrontend& source_frontend() const;

        [[nodiscard]] const Harnesses::MachineCode::Commons::MachineCodeFrontend& target_frontend() const;

        virtual void assemble() = 0;

        virtual ~Assembler();

        class AssemblerAlreadyRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit AssemblerAlreadyRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const Assembler& get_already_existing_assembler() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class Assembler;

        private:
            static std::string generate_message(const std::string& name);
        };

        struct AssemblerRefHash
        {
            std::size_t operator()(const std::reference_wrapper<Assembler>& assembler_reference) const noexcept;
        };

        struct AssemblerRefEq
        {
            bool operator()(const std::reference_wrapper<Assembler>& lhs,
                            const std::reference_wrapper<Assembler>& rhs) const;
        };

    private:
        void setup_frontends();

        static AssemblerAlreadyRegisteredException
        generate_assembler_already_registered_exception(const std::string& name);
    };
} // namespace SoftOmni::Compilation::Commons

#endif // SOFTOMNISWIFT_ASSEMBLER_HH
