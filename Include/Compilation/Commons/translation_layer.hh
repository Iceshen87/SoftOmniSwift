
#ifndef SOFTOMNISWIFT_TRANSLATION_LAYER_HH
#define SOFTOMNISWIFT_TRANSLATION_LAYER_HH
#include <string>

#include <Include/Harnesses/MachineCode/Commons/machine_code_frontend.hh>

namespace SoftOmni::Compilation::Commons
{
    class TranslationLayer
    {
        std::string name_;

        std::string display_name_;

        Harnesses::MachineCode::Commons::MachineCodeFrontend* source_;

        Harnesses::MachineCode::Commons::MachineCodeFrontend* target_;

        static std::unordered_map<std::string, TranslationLayer*> translation_layers_;

    protected:
        explicit TranslationLayer(const std::string& name, Harnesses::MachineCode::Commons::MachineCodeFrontend& source,
                                  Harnesses::MachineCode::Commons::MachineCodeFrontend& target);

        explicit TranslationLayer(std::string name, std::string display_name,
                                  Harnesses::MachineCode::Commons::MachineCodeFrontend& source,
                                  Harnesses::MachineCode::Commons::MachineCodeFrontend& target);

    public:
        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(std::string new_display_name);

        [[nodiscard]] const Harnesses::MachineCode::Commons::MachineCodeFrontend& source() const;

        [[nodiscard]] const Harnesses::MachineCode::Commons::MachineCodeFrontend& target() const;

        virtual void translate() = 0;

        virtual ~TranslationLayer();

        class TranslationLayerAlreadyRegisteredException final : std::exception
        {
            std::string name_;

            std::string message_;

            explicit TranslationLayerAlreadyRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const TranslationLayer& get_already_existing_translation_layer() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend TranslationLayer;

        private:
            static std::string generate_message(const std::string& name);
        };

        class TranslationLayerBetweenSameMachineCodeFrontendsException final : std::exception
        {
            std::string message_;

            explicit TranslationLayerBetweenSameMachineCodeFrontendsException(
                const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);

        public:
            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend TranslationLayer;

        private:
            static std::string
            generate_message(const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);
        };

    private:
        void setup_translation_layers_relationships();

        static TranslationLayerAlreadyRegisteredException
        generate_translation_unit_already_registered_exception(const std::string& name);

        static TranslationLayerBetweenSameMachineCodeFrontendsException
        generate_translation_layer_between_same_machine_code_frontends_exception(
            const Harnesses::MachineCode::Commons::MachineCodeFrontend& machine_code_frontend);
    };
} // namespace SoftOmni::Compilation::Commons

#endif // SOFTOMNISWIFT_TRANSLATION_LAYER_HH
