
#ifndef SOFTOMNISWIFT_INSTRUCTION_SET_HH
#define SOFTOMNISWIFT_INSTRUCTION_SET_HH

#include <Include/Harnesses/MachineCode/Commons/machine_code_frontend.hh>

#include <string>

namespace SoftOmni::Harnesses::MachineCode::Commons
{
    class InstructionSet
    {
        std::string name_;

        std::string display_name_;

        std::unordered_map<std::string, MachineCodeFrontend*> machine_code_frontends_;

        std::optional<std::reference_wrapper<MachineCodeFrontend>> reference_machine_code_frontend_;

        static std::unordered_map<std::string, InstructionSet*> instruction_sets_;

    public:
        explicit InstructionSet(const std::string& name);

        explicit InstructionSet(const std::string& name, std::string display_name);

        [[nodiscard]] const std::string& name() const;

        [[nodiscard]] const std::string& display_name() const;

        void set_display_name(std::string new_display_name);

        std::optional<std::reference_wrapper<MachineCodeFrontend>> operator[](const std::string& name) const;

        static std::optional<std::reference_wrapper<InstructionSet>> get(const std::string& name);

        friend bool operator==(const InstructionSet& lhs, const InstructionSet& rhs)
        {
            return lhs.name_ == rhs.name_;
        }

        bool operator==(const std::string_view& rhs) const;

        friend std::size_t hash_value(const InstructionSet& instruction_set)
        {
            return std::hash<std::string>{}(instruction_set.name_);
        }

        friend std::ostream& operator<<(std::ostream& os, const InstructionSet& instruction_set)
        {
            os << instruction_set.name_;
            return os;
        }

        InstructionSet(const InstructionSet& other) = default;

        InstructionSet(InstructionSet&& other) noexcept;

        InstructionSet& operator=(const InstructionSet& other);

        InstructionSet& operator=(InstructionSet&& other) noexcept;

        ~InstructionSet();

        class InstructionSetAlreadyRegisteredException : std::exception
        {
            std::string name_;

            std::string message_;

            explicit InstructionSetAlreadyRegisteredException(const std::string& name);

        public:
            [[nodiscard]] const std::string& name() const;

            [[nodiscard]] const std::string& message() const;

            [[nodiscard]] const InstructionSet& get_already_existing_instruction_set() const;

            [[nodiscard]] const char* what() const noexcept override;

            friend class InstructionSet;

        private:
            static std::string generate_message(const std::string& name);
        };

    private:
        void setup_instruction_set(const std::string& name);

        static InstructionSetAlreadyRegisteredException
        generate_instruction_set_already_registered_exception(const std::string& name);
    };
} // namespace SoftOmni::Harnesses::MachineCode::Commons

#endif // SOFTOMNISWIFT_INSTRUCTION_SET_HH
