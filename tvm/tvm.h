#ifndef SPRAWL_TVM_TVM_H_
#define SPRAWL_TVM_TVM_H_

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <istream>
#include <string>
#include <map>
#include <vector>

namespace psy::tvm {
  typedef std::uint8_t u8;
  typedef std::uint16_t u16;
  typedef std::uint32_t u32;
  typedef std::uint64_t u64;

  enum class OpCode { /* 4 bits -- c++14 */
    kAdd  = 0b0001'0000'00000000,
    kAnd  = 0b0101'0000'00000000,

    kBr   = 0b0000'000'000000000,
    kBrn  = 0b0000'100'000000000,
    kBrz  = 0b0000'010'000000000,
    kBrp  = 0b0000'001'000000000,

    kBrnp = kBr | kBrn | kBrp,
    kBrnz = kBr | kBrn | kBrz,
    kBrzp = kBr | kBrz | kBrp,
    kBrnzp = kBr | kBrn | kBrz | kBrp,

    kJmp  = 0b0100'0000'00000000,
    kLd   = 0b0010'000'000000000,
    kLdi  = 0b1010'0000'00000000,
    kLdr  = 0b0110'000'000000000,
    kLea  = 0b1110'000'000000000,
    kSt   = 0b0011'0000'00000000,
    kNot  = 0b1001'0000'00000000,

    kTrap      = 0b1111'0000'00000000,
    kTrapGetc  = kTrap | 0x20,
    kTrapOut   = kTrap | 0x21,
    kTrapPuts  = kTrap | 0x22,
    kTrapIn    = kTrap | 0x23,
    kTrapPutsp = kTrap | 0x24,
    kTrapHalt  = kTrap | 0x25,

    kRet = 0b1100'000'111'000000,

    kIllegal   = 0b1101'0000'00000000, /* ☠ */
  };

  OpCode StringToOpcode(const std::string& opstr);
  u16 StringToRegister(const std::string& regstr);

  constexpr u16 AsSR1(u16 sr1) { return sr1 << 6; }
  constexpr u16 AsDR(u16 dr)   { return dr << 9; }

  class Token {
  public:
    enum class Type {
      Segment, Instruction, Register,
      Label, NumberLiteral, HexNumber,
      Punct, String, Unknown };

    Token(std::string&& tok, size_t ln) :
      value_(std::move(tok)), line_number_(ln),
      type_(Type::Unknown), address_(0) {
      Classify();
    }

    std::string TypeString() const;
    u16 HexToU16() const;
    u16 LitValue() const;
    u16 U16Value() const;

    std::string value_;
    size_t line_number_;
    Type type_;
    OpCode opcode_;
    u16 address_;
  private:
    void Classify();
  };

  struct InstructionRow {
    InstructionRow() :
      label_(nullptr), op_(nullptr),
      operands_({nullptr}) {}

    inline int AssignLabel(std::vector<Token>::iterator& it,
                           std::vector<Token>::iterator end);

    inline int AssignOp(std::vector<Token>::iterator& it,
                        std::vector<Token>::iterator end);

    inline int AssignOperand(std::vector<Token>::iterator& it,
                             std::vector<Token>::iterator end,
                             u8 pos);

    Token* label_;
    Token* op_;
    Token* operands_[3];

    friend std::ostream& operator<<(std::ostream& os, const InstructionRow& row) {
      auto safe_print_fn = [](const Token* r) noexcept -> std::string {
        return r == nullptr ? ".." : r->value_;
      };

      os << std::setw(20) << std::setfill(' ') << safe_print_fn(row.label_)
         << std::setw(20) << std::setfill(' ') << safe_print_fn(row.op_)
         << std::setw(20) << std::setfill(' ') << safe_print_fn(row.operands_[0])
         << std::setw(20) << std::setfill(' ') << safe_print_fn(row.operands_[1])
         << std::setw(20) << std::setfill(' ') << safe_print_fn(row.operands_[2]);
      return os;
    }
  };

  struct InstructionSet {
    std::vector<InstructionRow> rows_;
    std::map<std::string, const Token*> symtab_;

    friend std::ostream& operator<<(std::ostream& os, const InstructionSet& iset) {
      size_t address = 0;

      os << "INSTA =============" << std::endl;
      for (const auto& row : iset.rows_)
        os << std::hex << std::setw(4) << std::setfill('0') << 0x10 * address++
           << row << std::endl;

      os << "SYMTAB ============" << std::endl;
      for (auto it = iset.symtab_.cbegin(); it != iset.symtab_.end(); ++it)
        os << it->first << "@" << std::hex << it->second->address_ << std::endl;

      return os;
    }
  };

  struct Image {
    Image() : data_({0}), len_(0) {}
    u16 data_[0xffff];
    size_t len_;
  };

  std::vector<Token> Tokenize(std::istream& is);
  void BuildSymtab(InstructionSet& inset);
  InstructionSet Parse(std::vector<Token>& tokens);
  Image Encode(InstructionSet& inset);
}
#endif
