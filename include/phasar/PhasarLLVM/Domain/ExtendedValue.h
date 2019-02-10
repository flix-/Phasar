#ifndef EXTENDEDVALUE_H
#define EXTENDEDVALUE_H

#include <cassert>
#include <functional>
#include <set>
#include <string>
#include <vector>

namespace llvm {
class Value;
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

    bool isMemLocationSeqEqual = memLocationSeq == rhs.memLocationSeq;
    if (!isMemLocationSeqEqual) return false;

    bool isEndOfTaintedBlockLabelEqual = endOfTaintedBlockLabel == rhs.endOfTaintedBlockLabel;
    if (!isEndOfTaintedBlockLabelEqual) return false;

    bool isEndOfTaintedBlockSuccLabelsEqual = endOfTaintedBlockSuccLabels == rhs.endOfTaintedBlockSuccLabels;
    if (!isEndOfTaintedBlockSuccLabelsEqual) return false;

    bool isVarArgIndexEqual = varArgIndex == rhs.varArgIndex;
    if (!isVarArgIndexEqual) return false;

    bool isCurrentVarArgIndexEqual = currentVarArgIndex == rhs.currentVarArgIndex;
    if (!isCurrentVarArgIndexEqual) return false;

    return true;
  }

  bool operator<(const ExtendedValue& rhs) const {
    if (std::less<const llvm::Value*>{}(value, rhs.value)) return true;
    if (std::less<const llvm::Value*>{}(rhs.value, value)) return false;

    if (memLocationSeq < rhs.memLocationSeq) return true;
    if (rhs.memLocationSeq < memLocationSeq) return false;

    if (std::less<std::string>{}(endOfTaintedBlockLabel, rhs.endOfTaintedBlockLabel)) return true;
    if (std::less<std::string>{}(rhs.endOfTaintedBlockLabel, endOfTaintedBlockLabel)) return false;

    if (endOfTaintedBlockSuccLabels < rhs.endOfTaintedBlockSuccLabels) return true;
    if (rhs.endOfTaintedBlockSuccLabels < endOfTaintedBlockSuccLabels) return false;

    if (std::less<long>{}(varArgIndex, rhs.varArgIndex)) return true;
    if (std::less<long>{}(rhs.varArgIndex, varArgIndex)) return false;

    return std::less<long>{}(currentVarArgIndex, rhs.currentVarArgIndex);
  }

  const llvm::Value* getValue() const { return value; }

  const std::vector<const llvm::Value*> getMemLocationSeq() const { return memLocationSeq; }
  void setMemLocationSeq(std::vector<const llvm::Value*> _memLocationSeq) { memLocationSeq = _memLocationSeq; }

  const std::string getEndOfTaintedBlockLabel() const { return endOfTaintedBlockLabel; }
  const std::set<std::string> getEndOfTaintedBlockSuccLabels() const { return endOfTaintedBlockSuccLabels; }
  void setEndOfTaintedBlockLabels(std::string _endOfTaintedBlockLabel,
                                  std::set<std::string> _endOfTaintedBlockSuccLabels ) {

    endOfTaintedBlockLabel = _endOfTaintedBlockLabel;
    endOfTaintedBlockSuccLabels = _endOfTaintedBlockSuccLabels;
  }

  long getVarArgIndex() const { return varArgIndex; }
  void setVarArgIndex(long _varArgIndex) { varArgIndex = _varArgIndex; }
  void resetVarArgIndex() { varArgIndex = -1L; }
  long getCurrentVarArgIndex() const { return currentVarArgIndex; }
  void incrementCurrentVarArgIndex() { ++currentVarArgIndex; }
  bool isVarArg() const { return varArgIndex > -1L; }

private:
  const llvm::Value* value = nullptr;
  std::vector<const llvm::Value*> memLocationSeq;

  std::string endOfTaintedBlockLabel;
  std::set<std::string> endOfTaintedBlockSuccLabels;

  long varArgIndex = -1L;
  long currentVarArgIndex = -1L;
};

} // namespace psr

namespace std {

template<>
struct hash<psr::ExtendedValue> {

  std::size_t operator()(const psr::ExtendedValue& ev) const {
    std::size_t seed = 0x4711;

    seed ^= hash<const llvm::Value*>{}(ev.getValue()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    for (const auto& memLocationPart : ev.getMemLocationSeq()) {
      seed ^= hash<const llvm::Value*>{}(memLocationPart) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    seed ^= hash<string>{}(ev.getEndOfTaintedBlockLabel()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    for (const auto& succLabel : ev.getEndOfTaintedBlockSuccLabels()) {
      seed ^= hash<string>{}(succLabel) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    seed ^= hash<long>{}(ev.getVarArgIndex()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    seed ^= hash<long>{}(ev.getCurrentVarArgIndex()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
  }
};

} // namespace std

#endif // EXTENDEDVALUE_H

