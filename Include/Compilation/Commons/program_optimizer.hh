
#ifndef SOFTOMNISWIFT_PROGRAM_OPTIMIZER_HH
#define SOFTOMNISWIFT_PROGRAM_OPTIMIZER_HH
#include <Include/Harnesses/MachineCode/Commons/machine_code_frontend.hh>
#include <string>

namespace SoftOmni::Compilation::Commons
{
    class ProgramOptimizer
    {
        std::string name_;

        std::string display_name_;

        Harnesses::MachineCode::Commons::MachineCodeFrontend* machine_code_frontend_;

        static std::unordered_map<std::string, ProgramOptimizer*> program_optimizers_;

    protected:
        explicit ProgramOptimizer(const std::string& name,
                                  Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        explicit ProgramOptimizer(std::string name, std::string display_name,
                                  Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(const std::string_view& new_display_name);

        [[nodiscard]] const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend() const;

        virtual void optimize() = 0;

        virtual ~ProgramOptimizer();

        class ProgramOptimizerAlreadyRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit ProgramOptimizerAlreadyRegisteredException(
                const std::string& name,
                const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const ProgramOptimizer& already_existing_program_optimizer() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class ProgramOptimizer;

        private:
            std::string
            generate_message(const std::string& name,
                             const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend) const;
        };

    private:
        void setup_frontend();

        static ProgramOptimizerAlreadyRegisteredException generate_programOptimizer_already_registered_exception(
            const std::string& name, const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);
    };
} // namespace SoftOmni::Compilation::Commons

#endif // SOFTOMNISWIFT_PROGRAM_OPTIMIZER_HH
