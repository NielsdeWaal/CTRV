// Type your code here, or load an example.

#include <array>
#include <functional>

enum class typeList {
    R = 0b0110011,
    I = 0b0010011,
};

enum class funct3 {
    ADD = 0b000,
    SUB = 0b000,
    SLL = 0b001,
    XOR = 0b100,
    SRI = 0b101,
    SRA = 0b101,
    OR  = 0b110,
    AND = 0b111,
};

enum class funct7 {
    SUB = 0b0100000,
    LRD = 0b0001000,
    SCD = 0b0001100,
};

enum class funct10 {
    ADD     = 0b00000000,
    SUB     = 0x100,
    SLL     = 0b00000001,
    SLT     = 0b00000010,
    SLTU    = 0b00000011,
    XOR     = 0b00000100,
    SRL     = 0b00000101,
    SRA     = 0x105,
    OR      = 0b00000110,
    AND     = 0b00000111,
    MUL     = 0b00001000,
    MULH    = 0b00001001,
    MULHSU  = 0xA,
    MULHU   = 0xB,
    DIV     = 0xC,
    DIVU    = 0xD,
    REM     = 0xE,
    REMU    = 0xF,
};

template<typename Value, typename Bit>[[nodiscard]] constexpr bool testBit(const Value val, const Bit bit) noexcept {
    return val & (static_cast<Value>(1) << bit);
}

template<typename Type, typename CRTP> struct StronglyTyped {
    [[nodiscard]] constexpr auto data() const noexcept { return mVal; }
    [[nodiscard]] constexpr auto operator&(const Type rhs) const noexcept { return mVal & rhs; }
    [[nodiscard]] constexpr bool testBit(const Type bit) const noexcept { return testBit(mVal, bit); }

    [[nodiscard]] friend constexpr auto operator&(const Type lhs, const CRTP rhs) noexcept { return lhs & rhs.mVal; }

    template<typename T>
    [[nodiscard]] constexpr T getFunct3() const noexcept { return static_cast<T>((mVal >> 12) & 0x7F); }
    [[nodiscard]] constexpr auto getRS1() const noexcept { return static_cast<uint32_t>((mVal >> 15) & 0b11111);}

    constexpr explicit StronglyTyped(const Type val) noexcept : mVal(val) {}

  protected:
    uint32_t mVal;
};

struct Instruction : StronglyTyped<uint32_t, Instruction> {    
    [[nodiscard]] constexpr typeList getType() const noexcept { return static_cast<typeList>(mVal & 0x7F); }

    friend struct RType;
};

struct RType : StronglyTyped<uint32_t, RType> {
    template<typename T>
    [[nodiscard]] constexpr T getFunct7() const noexcept { return static_cast<T>((mVal >> 25) & 0x7F); }
    [[nodiscard]] constexpr funct10 getFunct10() const noexcept { return static_cast<funct10>((getFunct7<uint32_t>() << 3 ) | getFunct3<uint32_t>()); }
    [[nodiscard]] constexpr auto getRS2() const noexcept { return ((mVal >> 20) & 0b11111); }
    [[nodiscard]] constexpr auto getRD() const noexcept { return ((mVal >> 7) & 0b11111); }

    constexpr RType(Instruction ins) noexcept : StronglyTyped{ins.mVal} {}
};

template<size_t RAMSIZE = 65535> struct System {
    // 31 registers, x[0] == 0, x[16] == PC
    std::array<uint32_t, 16> x{};
    std::array<uint32_t, RAMSIZE> ram{};

    [[nodiscard]] constexpr auto &PC() noexcept { return x[16]; }
    [[nodiscard]] constexpr const auto &PC() const noexcept { return x[16]; }

    constexpr void writeWord(uint16_t loc, uint32_t val) {

    }

    constexpr System() = default;

    template<size_t SIZE> constexpr System(const std::array<std::uint8_t, SIZE> &memory) noexcept {
        static_assert(SIZE <= RAMSIZE);

        x[0] = 0;

        for (size_t loc = 0; loc < SIZE; ++loc) {
            writeWord(static_cast<uint32_t>(loc), memory[loc]);
        }
    }

    constexpr void dataProcessing() noexcept {

    }

    constexpr void run() noexcept {

    }

    constexpr void processInstruction() noexcept {

    }

    /*
    template<typename T>
    constexpr void run(const T instr) noexcept {

    }

    template<typename ...T>
    constexpr void run(const T ... instr) {

    }
    */
};

int main() {
    constexpr System system{};

    return 0;
}
