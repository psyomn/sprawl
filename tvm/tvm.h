#ifndef CACOPHAGY_TVM_TVM_H_
#define CACOPHAGY_TVM_TVM_H_

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

    std::string value_;
    size_t line_number_;
    Type type_;
    u16 address_;
  private:
    void Classify();
  };

  struct InstructionRow {
    InstructionRow() :
      label_(nullptr), op_(nullptr),
      operands_({nullptr}) {}

    inline int AssignLabel(std::vector<Token>::const_iterator& it,
                           std::vector<Token>::const_iterator end);

    inline int AssignOp(std::vector<Token>::const_iterator& it,
                        std::vector<Token>::const_iterator end);

    inline int AssignOperand(std::vector<Token>::const_iterator& it,
                             std::vector<Token>::const_iterator end,
                             u8 pos);

    const Token* label_;
    const Token* op_;
    const Token* operands_[3];

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

    friend std::ostream& operator<<(std::ostream& os, const InstructionSet& iset) {
      size_t address = 0;
      for (const auto& row : iset.rows_)
        os << std::hex << std::setw(4) << std::setfill('0') << 0x10 * address++
           << row << std::endl;
      return os;
    }
  };

  std::vector<Token> Tokenize(std::istream& is);
  InstructionSet Parse(const std::vector<Token>& tokens);
}
#endif
