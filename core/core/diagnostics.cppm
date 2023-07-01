export module core.diagnostics;

namespace core
{

class Exception
{
public:
    using This = Exception;
        
    constexpr explicit
    Exception(char const* message) noexcept
        : m_message{message} {}

    constexpr
    Exception(This const&) noexcept = default;

    constexpr
    Exception(This&&) noexcept = default;

    constexpr
    auto operator=(This const&) noexcept -> This& = default;

    constexpr
    auto operator=(This&&) noexcept -> This& = default;

    constexpr
    ~Exception() noexcept = default;

    [[nodiscard]] constexpr
    auto message() const noexcept -> char const*
    { return m_message; }
        
private:
    char const* m_message = nullptr;
};

class Out_Of_Range
    : public Exception
{
public:
    using This = Out_Of_Range;
    using Base = Exception;
        
    Out_Of_Range(char const* message)
        : Base(message) {}
    
    constexpr
    Out_Of_Range(This const&) noexcept = default;

    constexpr
    Out_Of_Range(This&&) noexcept = default;

    auto operator=(This const&) noexcept -> This& = default;

    auto operator=(This&&) noexcept -> This& = default;
};

}
