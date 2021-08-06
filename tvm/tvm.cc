#include "tvm.h"

#include <regex>
#include <iostream>

namespace psy::tvm {
  const std::regex kSegment("^\\.[A-Z][A-Z0-9]+$");
  const std::regex kInstruction("^("
                                "AND|ADD|"
                                "BR|BRn|BRzp|BRnz|BRp|BRz|BRnp|"
                                "HALT|"
                                "LD|LEA|LDR|"
                                "PUTS|"
                                "ST|"
                                "NOT"
                                ")$");
  const std::regex kRegister("^R[0-3]$");
  const std::regex kLabel("^[A-QS-Z_][A-Z0-9_]+$");
  const std::regex kNumberLiteral("^#?[-]?(0|[1-9][0-9]*)$");
  const std::regex kHexNumber("^x[A-Fa-f0-9]+$");
  const std::regex kPunct("^[,.:;]$");
  const std::regex kString("^\"[A-Za-z0-9[:punct:][:blank:]]*\"$");

  constexpr std::uint16_t kMemMax = 0xffff;

  void Token::Classify() {
    if (std::regex_match(value_, kSegment)) type_ = Type::Segment;
    else if (std::regex_match(value_, kInstruction)) type_ = Type::Instruction;
    else if (std::regex_match(value_, kRegister)) type_ = Type::Register;
    else if (std::regex_match(value_, kHexNumber)) type_ = Type::HexNumber;
    else if (std::regex_match(value_, kLabel)) type_ = Type::Label;
    else if (std::regex_match(value_, kNumberLiteral)) type_ = Type::NumberLiteral;
    else if (std::regex_match(value_, kPunct)) type_ = Type::Punct;
    else if (std::regex_match(value_, kString)) type_ = Type::String;
  }

  std::string Token::TypeString() const {
    switch (type_) {
    case Type::Segment: return "segment";
    case Type::Instruction: return "instruction";
    case Type::Register: return "register";
    case Type::Label: return "label";
    case Type::NumberLiteral: return "number-lit";
    case Type::HexNumber: return "hexnumber-lit";
    case Type::Punct: return "punct";
    case Type::String: return "string-lit";
    case Type::Unknown: return "unknown"; /* this should signify an error */
    default: abort(); /* unreachable */
    }
  }

  std::vector<Token> Tokenize(std::istream& is) {
    std::vector<Token> tokens;

    std::string buffer; // consider changing to stringstream
    size_t line_number = 1;
    bool incr_line_number = false;
    bool colon = false;

    char c = 0;
    while (is.get(c)) {
      switch (c) {
      case ';':
        while (is.get(c) && c != '\n') {}
        if (c == '\n') line_number++;
        break;
      case '"':
        buffer += c;

        while (is.get(c) && c != '"' && c != '\n') buffer += c;

        if (c == '\n') {
          std::cout << "error at line" << line_number << std::endl;
          exit(1);
        }

        if (c == '"') buffer += c;
        goto flush_buffer;

        break;
      case ':':
        colon = true;
        goto flush_buffer;
      case '\n': /* token from buff, incr line */
        incr_line_number = true;
        [[fallthrough]];
      case ',':
      case ' ':
      case '\t':
      flush_buffer:
        if (buffer.size() > 0)  {
          tokens.push_back(Token(std::move(buffer), line_number));
          buffer.clear();
        }
        if (incr_line_number) {
          line_number++;
          incr_line_number = false;
        }
        if (colon) {
          colon = false;
          tokens.push_back(Token(":", line_number));
        }
        break;
      default:
        buffer += c;
        break;
      }
    }

    if (buffer.size()) tokens.push_back(Token(std::move(buffer), line_number));

    return tokens;
  }

  int InstructionRow::AssignLabel(std::vector<Token>::const_iterator& it,
                                  std::vector<Token>::const_iterator end) {
    if (it == end) return -1;
    label_ = &*it;
    ++it;
    return 0;
  }

  int InstructionRow::AssignOp(std::vector<Token>::const_iterator& it,
                               std::vector<Token>::const_iterator end) {
    if (it == end) return -1;
    op_ = &*it;
    ++it;
    return 0;
  }

  int InstructionRow::AssignOperand(std::vector<Token>::const_iterator& it,
                                    std::vector<Token>::const_iterator end,
                                    u8 pos) {
    if (it == end) return -1;
    if (pos > 2) return -2;

    operands_[pos] = &*it;
    it++;

    return 0;
  }

  void BuildSymtab(InstructionSet& inset) {
    for (auto& row : inset.rows_)
      if (row.label_ && row.op_ && row.operands_[0])
        inset.symtab_[row.label_->value_] = row.label_;
  }

  InstructionSet Parse(const std::vector<Token>& tokens) {
    std::vector<Token>::const_iterator it = tokens.cbegin();
    const std::vector<Token>::const_iterator end = tokens.end();

    InstructionSet iset;

    // All programs must begin with .ORIG x????
    while (it != tokens.end()) {
      InstructionRow row;

      switch (it->type_) {
      case Token::Type::Segment:
      parse_segment:
        if (it->value_ == ".END") {
          row.AssignOp(it, end);
        } else if (it->value_ == ".ORIG" || it->value_ == ".STRINGZ") {
          // .ORIG ADDRESS
          // .STRINGZ "value"
          row.AssignOp(it, end);
          row.AssignOperand(it, end, 0);
        } else {
          // LABEL OP
          row.AssignLabel(it, end);
          row.AssignOperand(it, end, 0);
        }
        break;
      case Token::Type::Label:
        row.AssignLabel(it, end);
        if (it == end) continue;
        if (it->type_ == Token::Type::Instruction) goto parse_instruction;
        if (it->type_ == Token::Type::Segment) goto parse_segment;
        break;
      case Token::Type::Instruction:
      parse_instruction:
        if (it->value_ == "AND" || it->value_ == "ADD" || it->value_ == "LDR") {
          // AND DR1 SR2 SR3
          // AND DR1 SR1 imm5        (imm5 = literal max 5 bits)
          // LDR DR1 SR1 SR2 offset6 (offset6 = 6 bit memory offset)
          const int result =
            row.AssignOp(it, end)
          | row.AssignOperand(it, end, 0)
          | row.AssignOperand(it, end, 1)
          | row.AssignOperand(it, end, 2);
        } else if (it->value_ == "HALT" || it->value_ == "PUTS") {
          // HALT
          row.AssignOp(it, end);
        } else if (it->value_ == "BR" || it->value_ == "BRn" || it->value_ == "BRzp" ||
                   it->value_ == "BRnz" || it->value_ == "BRp" || it->value_ == "BRz" ||
                   it->value_ == "BRnp") {
          // BR LABEL
          const int result =
            row.AssignOp(it, end)
          | row.AssignOperand(it, end, 0);
        } else if (it->value_ == "LD" || it->value_ == "LEA" || it->value_ == "NOT" ||
                   it->value_ == "ST") {
          // LD DR LABEL
          // ST SR1 LABEL
          // NOT DR SR1
          const int result =
            row.AssignOp(it, end)
          | row.AssignOperand(it, end, 0)
          | row.AssignOperand(it, end, 1);
        }
        break;
      case Token::Type::NumberLiteral:
      case Token::Type::HexNumber:
      case Token::Type::Punct:
      case Token::Type::String:
      case Token::Type::Register:
      case Token::Type::Unknown:
      default:
        abort();
      }

      iset.rows_.push_back(std::move(row));
    }

    BuildSymtab(iset);

    return iset;
  }
}
