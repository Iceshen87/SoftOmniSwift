
#ifndef SOFTOMNISWIFT_OPTIMIZER_HH
#define SOFTOMNISWIFT_OPTIMIZER_HH
#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

namespace SoftOmni::Compilation::Commons
{
    class Optimizer
    {
        std::string name_;

        std::string display_name_;

        Parsing::Commons::Kernels::LanguageFrontend* frontend_;

        static std::unordered_map<std::string, Optimizer*> optimizers_;

    protected:
        explicit Optimizer(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& frontend);

        explicit Optimizer(std::string name, std::string display_name,
                           Parsing::Commons::Kernels::LanguageFrontend& frontend);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(const std::string_view& new_display_name);

        [[nodiscard]] const Parsing::Commons::Kernels::LanguageFrontend& frontend() const;

        virtual void optimize() = 0;

        virtual ~Optimizer();

        class OptimizerAlreadyRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit OptimizerAlreadyRegisteredException(const std::string& name,
                                                         const Parsing::Commons::Kernels::LanguageFrontend& frontend);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const Optimizer& already_existing_optimizer() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class Optimizer;

        private:
            std::string generate_message(const std::string& name,
                                         const Parsing::Commons::Kernels::LanguageFrontend& frontend) const;
        };

    private:
        void setup_frontend();

        static OptimizerAlreadyRegisteredException
        generate_optimizer_already_registered_exception(const std::string& name,
                                                        const Parsing::Commons::Kernels::LanguageFrontend& frontend);
    };
} // namespace SoftOmni::Compilation::Commons


#endif // SOFTOMNISWIFT_OPTIMIZER_HH
