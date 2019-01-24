#ifndef EXTENDEDVALUE_H
#define EXTENDEDVALUE_H

#include <cassert>
#include <functional>
#include <string>
#include <vector>

namespace llvm {
class Value;
class CallInst;
} // namespace llvm

namespace psr {

class ExtendedValue {
public:
  ExtendedValue() {}
  explicit ExtendedValue(const llvm::Value* _value) : value(_value) {
    assert(value != nullptr && "ExtendedValue requires a Value object");
  }
  ~ExtendedValue() = default;

  bool operator==(const ExtendedValue& rhs) const {
    bool isValueEqual = value == rhs.value;
    if (!isValueEqual) return false;

    bool isMemLocationSizeEqual = memLocationSeq.size() == rhs.memLocationSeq.size();
    if (!isMemLocationSizeEqual) return false;

    for (std::size_t i = 0; i < memLocationSeq.size(); ++i) {
      bool isMemLocationPartEqual = memLocationSeq[i] == rhs.memLocationSeq[i];
      if (!isMemLocationPartEqual) return false;
    }

    bool isEndOfTaintedBlockLabelEqual = endOfTaintedBlockLabel == rhs.endOfTaintedBlockLabel;
    if (!isEndOfTaintedBlockLabelEqual) return false;

    bool isCalleeEqual = callee == rhs.callee;
    if (!isCalleeEqual) return false;

    return true;
  }

  bool operator<(const ExtendedValue& rhs) const {
    if (std::less<const llvm::Value*>{}(value, rhs.value)) return true;
    if (std::less<const llvm::Value*>{}(rhs.value, value)) return false;

    if (std::less<std::size_t>{}(memLocationSeq.size(), rhs.memLocationSeq.size())) return true;
    if (std::less<std::size_t>{}(rhs.memLocationSeq.size(), memLocationSeq.size())) return false;

    for (std::size_t i = 0; i < memLocationSeq.size(); ++i) {
      if (std::less<const llvm::Value*>{}(memLocationSeq[i], rhs.memLocationSeq[i])) return true;
      if (std::less<const llvm::Value*>{}(rhs.memLocationSeq[i], memLocationSeq[i])) return false;
    }

    if (std::less<std::string>{}(endOfTaintedBlockLabel, rhs.endOfTaintedBlockLabel)) return true;
    if (std::less<std::string>{}(rhs.endOfTaintedBlockLabel, endOfTaintedBlockLabel)) return false;

    return std::less<const llvm::CallInst*>{}(callee, rhs.callee);
  }

  const llvm::Value* getValue() const { return value; }

  const std::vector<const llvm::Value*> getMemLocationSeq() const { return memLocationSeq; }
  void setMemLocationSeq(std::vector<const llvm::Value*> _memLocationSeq) { memLocationSeq = _memLocationSeq; }

  const llvm::Value* getMemLocationFrame() const { return memLocationSeq.empty() ? nullptr : memLocationSeq[0]; }
  void setMemLocationFrame(const llvm::Value* _memLocationFrame) { if (!memLocationSeq.empty()) memLocationSeq[0] = _memLocationFrame; }

  const std::string getEndOfTaintedBlockLabel() const { return endOfTaintedBlockLabel; }
  void setEndOfTaintedBlockLabel(std::string _endOfTaintedBlockLabel) { endOfTaintedBlockLabel = _endOfTaintedBlockLabel; }

  const llvm::CallInst* getCallee() const { return callee; }
  void setCallee(const llvm::CallInst* _callee) { callee = _callee; }
  void resetCallee() { callee = nullptr; }

private:
  const llvm::Value* value = nullptr;
  std::vector<const llvm::Value*> memLocationSeq;
  std::string endOfTaintedBlockLabel;
  const llvm::CallInst* callee = nullptr;
};

} // namespace psr

namespace std {

template<>
struct hash<psr::ExtendedValue> {

  std::size_t operator()(const psr::ExtendedValue& ev) const {
    std::size_t seed = 0x4711;

    seed ^= hash<const llvm::Value*>{}(ev.getValue()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    for (const auto memLocationPart : ev.getMemLocationSeq()) {
      seed ^= hash<const llvm::Value*>{}(memLocationPart) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    seed ^= hash<string>{}(ev.getEndOfTaintedBlockLabel()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    seed ^= hash<const llvm::CallInst*>{}(ev.getCallee()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
  }
};

} // namespace std

#endif // EXTENDEDVALUE_H

