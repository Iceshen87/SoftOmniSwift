
#ifndef SOFTOMNISWIFT_LANGUAGE_RUNTIME_HH
#define SOFTOMNISWIFT_LANGUAGE_RUNTIME_HH
#include <memory>

#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

namespace SoftOmni::Execution::Commons
{
    class LanguageRuntime
    {
        std::string name_;

        std::string display_name_;

        Parsing::Commons::Kernels::LanguageFrontend* frontend_;

        static std::unordered_map<std::string, LanguageRuntime*> language_runtimes_;

    protected:
        explicit LanguageRuntime(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& frontend);

        explicit LanguageRuntime(std::string name, std::string display_name,
                                 Parsing::Commons::Kernels::LanguageFrontend& frontend);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(std::string_view new_display_name);

        [[nodiscard]] const Parsing::Commons::Kernels::LanguageFrontend& frontend() const;

        virtual void execute() = 0;

        virtual ~LanguageRuntime();

        friend class ::SoftOmni::Parsing::Commons::Kernels::LanguageFrontend;

        class LanguageRuntimeAlreadyRegisteredException : std::exception
        {
            std::string name_;

            std::string message_;

            explicit LanguageRuntimeAlreadyRegisteredException(
                const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const LanguageRuntime&
            already_registered_language_runtime() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class LanguageRuntime;

        private:
            std::string generate_message(const std::string& name,
                                         const Parsing::Commons::Kernels::LanguageFrontend& frontend) const;
        };

    private:
        void setup_frontend();

        static LanguageRuntimeAlreadyRegisteredException generate_language_runtime_already_registered_exception(
            const std::string& name, const Parsing::Commons::Kernels::LanguageFrontend& frontend);
    };
} // namespace SoftOmni::Execution::Commons

#endif // SOFTOMNISWIFT_LANGUAGE_RUNTIME_HH
