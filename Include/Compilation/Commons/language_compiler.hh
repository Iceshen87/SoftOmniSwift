
#ifndef SOFTOMNISWIFT_LANGUAGE_COMPILER_HH
#define SOFTOMNISWIFT_LANGUAGE_COMPILER_HH

#include <Include/Parsing/Commons/Kernels/language_frontend.hh>

namespace SoftOmni::Compilation::Commons
{
    class LanguageCompiler
    {
        std::string name_;

        std::string display_name_;

        Parsing::Commons::Kernels::LanguageFrontend* source_;

        Parsing::Commons::Kernels::LanguageFrontend* target_;

        static std::unordered_map<std::string, LanguageCompiler*> compilers_;

    protected:
        explicit LanguageCompiler(const std::string& name, Parsing::Commons::Kernels::LanguageFrontend& source,
                                  Parsing::Commons::Kernels::LanguageFrontend& target);

        explicit LanguageCompiler(std::string name, std::string display_name,
                                  Parsing::Commons::Kernels::LanguageFrontend& source,
                                  Parsing::Commons::Kernels::LanguageFrontend& target);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(const std::string_view& new_display_name);

        [[nodiscard]] const Parsing::Commons::Kernels::LanguageFrontend& source_frontend() const;

        [[nodiscard]] const Parsing::Commons::Kernels::LanguageFrontend& target_frontend() const;

        virtual void compile() = 0;

        virtual ~LanguageCompiler();

        class LanguageCompilerAlreadyRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit LanguageCompilerAlreadyRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const LanguageCompiler& get_already_existing_language_compiler() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class LanguageCompiler;

        private:
            static std::string generate_message(const std::string& name);
        };

        class LanguageCompilerForSameFrontendException final : std::exception
        {
            std::string message_;

            explicit LanguageCompilerForSameFrontendException(
                const Parsing::Commons::Kernels::LanguageFrontend& frontend);

        public:
            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class LanguageCompiler;

        private:
            static std::string generate_message(const Parsing::Commons::Kernels::LanguageFrontend& source);
        };

        struct LanguageCompilerRefHash
        {
            std::size_t operator()(const std::reference_wrapper<LanguageCompiler>& language_compiler_reference) const noexcept;
        };

        struct LanguageCompilerRefEq
        {
            bool operator()(const std::reference_wrapper<LanguageCompiler>& lhs,
                            const std::reference_wrapper<LanguageCompiler>& rhs) const;
        };

    private:
        void setup_frontends();

        static LanguageCompilerAlreadyRegisteredException
        generate_compiler_already_registered_exception(const std::string& name);

        static LanguageCompilerForSameFrontendException generate_language_compiler_for_same_frontend_exception(
            const Parsing::Commons::Kernels::LanguageFrontend& frontend);
    };
} // namespace SoftOmni::Compilation::Commons

#endif // SOFTOMNISWIFT_LANGUAGE_COMPILER_HH
