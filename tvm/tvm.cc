#include "tvm.h"

#include <regex>
#include <iostream>

namespace psy::tvm {
  const std::regex kSegment("^\\.[A-Z][A-Z0-9]+$");
  const std::regex kInstruction("^("
                                "AND|ADD|"
                                "BR|BRn|BRzp|BRnz|BRp|BRz|BRnp|"
                                "HALT|"
                                "JMP|"
                                "LD|LDI|LEA|LDR|"
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

  OpCode StringToOpcode(const std::string& opstr) {
    if      (opstr == "AND")   return OpCode::kAnd;
    else if (opstr == "ADD")   return OpCode::kAdd;
    else if (opstr == "BR")    return OpCode::kBr;
    else if (opstr == "BRn")   return OpCode::kBrn;
    else if (opstr == "BRz")   return OpCode::kBrz;
    else if (opstr == "BRp")   return OpCode::kBrp;
    else if (opstr == "BRnp")  return OpCode::kBrnp;
    else if (opstr == "BRnz")  return OpCode::kBrnz;
    else if (opstr == "BRzp")  return OpCode::kBrzp;
    else if (opstr == "JMP")   return OpCode::kJmp;
    else if (opstr == "LD")    return OpCode::kLd;
    else if (opstr == "LDI")   return OpCode::kLdi;
    else if (opstr == "LDR")   return OpCode::kLdr;
    else if (opstr == "LEA")   return OpCode::kLea;
    else if (opstr == "ST")    return OpCode::kSt;
    else if (opstr == "NOT")   return OpCode::kNot;
    else if (opstr == "GETC")  return OpCode::kTrapGetc;
    else if (opstr == "OUT")   return OpCode::kTrapOut;
    else if (opstr == "PUTS")  return OpCode::kTrapPuts;
    else if (opstr == "IN")    return OpCode::kTrapIn;
    else if (opstr == "PUTSP") return OpCode::kTrapPutsp;
    else if (opstr == "HALT")  return OpCode::kTrapHalt;
    return OpCode::kIllegal;
  }

  u16 StringToRegister(const std::string& regstr) {
    if (regstr.size() < 2) abort();
    return regstr[1] - 0x30; /* will fail on non ascii */
  }

  void Token::Classify() {
    if (std::regex_match(value_, kSegment)) type_ = Type::Segment;
    else if (std::regex_match(value_, kInstruction)) type_ = Type::Instruction;
    else if (std::regex_match(value_, kRegister)) type_ = Type::Register;
    else if (std::regex_match(value_, kHexNumber)) type_ = Type::HexNumber;
    else if (std::regex_match(value_, kLabel)) type_ = Type::Label;
    else if (std::regex_match(value_, kNumberLiteral)) type_ = Type::NumberLiteral;
    else if (std::regex_match(value_, kPunct)) type_ = Type::Punct;
    else if (std::regex_match(value_, kString)) type_ = Type::String;

    if (type_ == Type::Instruction) opcode_ = StringToOpcode(value_);
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

  u16 Token::HexToU16() const {
    std::stringstream ss;
    ss << std::hex << value_.substr(1);
    u16 ret = 0;
    ss >> ret;
    return ret;
  }

  u16 Token::LitValue() const {
    std::stringstream ss;
    ss << value_.substr(1);
    // TODO: this might not be portable: two's complement works on my
    //   current architecture and we can piggyback on this when encoding
    //   such literals. Might not be the case on others (very unlikely
    //   but still possible)
    u16 ret = 0;
    ss >> ret;
    return ret;
  }

  u16 Token::U16Value() const {
    std::stringstream ss;
    ss << value_;
    u16 ret = 0;
    ss >> ret;
    return ret;
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

  int InstructionRow::AssignLabel(std::vector<Token>::iterator& it,
                                  std::vector<Token>::iterator end) {
    if (it == end) return -1;
    label_ = &*it;
    ++it;
    return 0;
  }

  int InstructionRow::AssignOp(std::vector<Token>::iterator& it,
                               std::vector<Token>::iterator end) {
    if (it == end) return -1;
    op_ = &*it;
    ++it;
    return 0;
  }

  int InstructionRow::AssignOperand(std::vector<Token>::iterator& it,
                                    std::vector<Token>::iterator end,
                                    u8 pos) {
    if (it == end) return -1;
    if (pos > 2) return -2;

    operands_[pos] = &*it;
    it++;

    return 0;
  }

  void BuildSymtab(InstructionSet& inset) {
    // build symbtab
    //
    // Allow for the following:
    //
    // NUMBER  .BLKW 1          (uninitialized integer)
    // ARRAY   .BLKW 10         (uninitialized array)
    // STR     .STRINGZ "hello" (null terminated a la C)
    // MONTHSA .STRINGZ "Jan"
    //         .STRINGZ "Feb"
    //         .STRINGZ "Mar"
    //         .STRINGZ "Apr"
    //         ...
    size_t offset = 0;
    for (auto& row : inset.rows_) {
      if (row.op_->value_ == ".ORIG") offset = row.operands_[0]->HexToU16();
      if (!(row.label_ && row.op_ && row.operands_[0])) {
        offset += 2;
        continue;
      }

      std::cout << std::hex << offset << std::endl;

      // variables should be huddled up together at the bottom of the assembly
      row.label_->address_ = offset;
      inset.symtab_[row.label_->value_] = row.label_;
      if (row.op_->value_ == ".STRINGZ"){
        offset += row.operands_[0]->value_.size();
      } else if (row.op_->value_ == ".BLKW") {
        std::cout << "try to do the lit value" << std::endl;
        std::cout << row.operands_[0]->U16Value() << std::endl;
        offset += row.operands_[0]->U16Value();
      }
    }
  }

  InstructionSet Parse(std::vector<Token>& tokens) {
    std::vector<Token>::iterator it = tokens.begin();
    std::vector<Token>::iterator end = tokens.end();

    InstructionSet iset;

    // TODO: use opcode_ instead of string matches here

    // All programs must begin with .ORIG x????
    while (it != tokens.end()) {
      InstructionRow row;

      switch (it->type_) {
      case Token::Type::Segment:
      parse_segment:
        if (it->value_ == ".END") {
          row.AssignOp(it, end);
        } else if (it->value_ == ".ORIG" || it->value_ == ".STRINGZ" ||
                   it->value_ == ".BLKW") {
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

  Image Encode(InstructionSet& inset) {
    Image img;

    for (auto const& row : inset.rows_) {
      if (row.op_ != nullptr) {
        if (row.op_->value_ == ".ORIG") {
          img.data_[img.len_++] = row.operands_[0]->HexToU16();
          continue;
        }

        /* AND, NOT */
        if (row.op_->value_ == "NOT") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kNot)
            | AsDR(StringToRegister(row.operands_[0]->value_))
            | AsSR1(StringToRegister(row.operands_[1]->value_))
            | 0b111111;
          continue;
        }

        if (row.op_->value_ == "AND")  {
          /*
             ri = regiser index (eg: r1 = index 1)

             AND R1, R2, R3
             [4bits opcode AND] [3bits ri] [3bits ri] [3bits 0] [3bits ri]

             AND R1, R2, #1
             [4bits opcode AND] [3bits ri] [3bits ri] [1bit 1] [5bit literal (2s complement)]
           */
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kAnd)
            | AsDR(StringToRegister(row.operands_[0]->value_))
            | AsSR1(StringToRegister(row.operands_[1]->value_))
            | ((row.operands_[2]->type_ == Token::Type::NumberLiteral) ?
              (0b1'00000 | (0b11111 & row.operands_[2]->LitValue()))
              : StringToRegister(row.operands_[2]->value_));
          continue;
        }

        if (row.op_->value_ == "ADD")  {
          /*
             ri = regiser index (eg: r1 = index 1)

             ADD R1, R2, R3
             [4bits opcode ADD] [3bits ri] [3bits ri] [3bits 0] [3bits ri]

             ADD R1, R2, #-1
             [4bits opcode ADD] [3bits ri] [3bits ri] [1bit 1] [5bit literal (2s complement)]
           */
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kAdd)
            | AsDR(StringToRegister(row.operands_[0]->value_))
            | AsSR1(StringToRegister(row.operands_[1]->value_))
            | ((row.operands_[2]->type_ == Token::Type::NumberLiteral) ?
              (0b1'00000 | (0b11111 & row.operands_[2]->LitValue()))
              : StringToRegister(row.operands_[2]->value_));
          continue;
        }

        // if (it->value_ == "LDR") {
        //   continue;
        // }

        /* HERE BE TRAPS! OH NO! TOO DANGEROUS! */

        if (row.op_->value_ == "GETC") {
          /* [4bits OP TRAP] [12 bits GETC 0x20]*/
          img.data_[img.len_++] = static_cast<u16>(OpCode::kTrapGetc);
          continue;
        }

        if (row.op_->value_ == "OUT") {
          /* [4bits OP TRAP] [12 bits OUT 0x21]*/
          img.data_[img.len_++] = static_cast<u16>(OpCode::kTrapOut);
          continue;
        }

        if (row.op_->value_ == "PUTS") {
          /* [4bits OP TRAP] [12 bits PUTS 0x22]*/
          img.data_[img.len_++] = static_cast<u16>(OpCode::kTrapPuts);
          continue;
        }

        if (row.op_->value_ == "IN") {
          /* [4bits OP TRAP] [12 bits PUTS 0x23]*/
          img.data_[img.len_++] = static_cast<u16>(OpCode::kTrapIn);
          continue;
        }

        if (row.op_->value_ == "PUTSP") {
          /* [4bits OP TRAP] [12 bits PUTSP 0x24]*/
          img.data_[img.len_++] = static_cast<u16>(OpCode::kTrapPutsp);
          continue;
        }

        if (row.op_->value_ == "HALT") {
          /* [4bits OP TRAP] [12 bits PUTSP 0x25]*/
          img.data_[img.len_++] = static_cast<u16>(OpCode::kTrapHalt);
          continue;
        }

        /* LD, LDI, LEA */

        if (row.op_->value_ == "LD") {
          /* [4bits OP LD] [3bits ri] [9bits PC offset] */
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kLd)
            | AsDR(StringToRegister(row.operands_[0]->value_))
            | (0b111111111 & row.operands_[1]->LitValue());
          continue;
        }

        if (row.op_->value_ == "LEA") {
          /* [4bits OP LEA] [3bits ri] [9bits PC offset] */
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kLea)
            | AsDR(StringToRegister(row.operands_[0]->value_))
            | (0b111111111 & row.operands_[1]->LitValue());
          continue;
        }

        if (row.op_->value_ == "LDI") {
          /* [4bits OP LDI] [3bits ri] [9bits PC offset] */
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kLdi)
            | AsDR(StringToRegister(row.operands_[0]->value_))
            | (0b111111111 & row.operands_[1]->LitValue());
          continue;
        }

        /*
            might be possible to be a little clever here, but keeping
            it simple for now.

           [4bits OP BR] [3bits nzp] [9bits PC offset]
        */

        if (row.op_->value_ == "BR") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBr)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRn") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrn)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRzp") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrzp)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRnz") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrnz)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRp") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrp)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRz") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrz)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRnp") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrnp)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "BRnzp") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kBrnzp)
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "ST") {
          img.data_[img.len_++] =
            static_cast<u16>(OpCode::kSt)
            | AsSR1(StringToRegister(row.operands_[0]->value_))
            | (0b111111111 & row.operands_[0]->LitValue());
          continue;
        }

        if (row.op_->value_ == "RET")
          img.data_[img.len_++] = static_cast<u16>(OpCode::kRet);

      } else if (row.label_ != nullptr) {
      }
    }
    return img;
  }
}
