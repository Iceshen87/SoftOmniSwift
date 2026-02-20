
#ifndef SOFTOMNISWIFT_CPU_HH
#define SOFTOMNISWIFT_CPU_HH

#include <Include/Harnesses/MachineCode/Commons/instruction_set.hh>

namespace SoftOmni::Execution::Machines
{
    class Processor
    {
        std::string name_;

        std::string display_name_;

        Harnesses::MachineCode::Commons::MachineCodeFrontend* machine_code_frontend_;

        static std::unordered_map<std::string, Processor*> processors_;

    protected:
        explicit Processor(const std::string& name, Harnesses::MachineCode::Commons::MachineCodeFrontend& frontend);

        explicit Processor(std::string name, std::string display_name,
                           Harnesses::MachineCode::Commons::MachineCodeFrontend& frontend);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(std::string_view new_display_name);

        [[nodiscard]] const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend() const;

        virtual void execute() = 0;

        virtual ~Processor();

        friend class ::SoftOmni::Harnesses::MachineCode::Commons::MachineCodeFrontend;

        class ProcessorAlreadyRegisteredException : std::exception
        {
            std::string name_;

            std::string message_;

            explicit ProcessorAlreadyRegisteredException(
                const std::string& name,
                const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const Processor& already_registered_processor() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class Processor;

        private:
            std::string
            generate_message(const std::string& name,
                             const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const;
        };

    private:
        void setup_frontend();

        static ProcessorAlreadyRegisteredException generate_processor_already_registered_exception(
            const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);
    };
} // namespace SoftOmni::Execution::Machines

#endif // SOFTOMNISWIFT_CPU_HH
