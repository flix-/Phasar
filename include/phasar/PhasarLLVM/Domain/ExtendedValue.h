#ifndef EXTENDEDVALUE_H
#define EXTENDEDVALUE_H

#include <cassert>
#include <cstdint>
#include <functional>
#include <tuple>

namespace llvm {
class Value;
} // namespace llvm

namespace psr {

class ExtendedValue {
public:
  ExtendedValue() {}
  explicit ExtendedValue(const llvm::Value* _value,
                         const llvm::Value* _patchedMemLocationFrame = nullptr)
    : value(_value),
      patchedMemLocationFrame(_patchedMemLocationFrame) {

    assert(value != nullptr && "ExtendedValue requires a Value object");
  }
  ~ExtendedValue() = default;

  bool operator==(const ExtendedValue& rhs) const {
    return value == rhs.value &&
           patchedMemLocationFrame == rhs.patchedMemLocationFrame;
  }
  bool operator<(const ExtendedValue& rhs) const {
    uintptr_t numValue = reinterpret_cast<uintptr_t>(value);
    uintptr_t numPatchedMemLocationFrame = reinterpret_cast<uintptr_t>(patchedMemLocationFrame);

    uintptr_t numRhsValue = reinterpret_cast<uintptr_t>(rhs.getValue());
    uintptr_t numRhsPatchedMemLocationFrame = reinterpret_cast<uintptr_t>(rhs.getPatchedMemLocationFrame());

    return std::tie(numValue, numPatchedMemLocationFrame) < std::tie(numRhsValue, numRhsPatchedMemLocationFrame);
  }

  const llvm::Value* getValue() const { return value; }
  const llvm::Value* getPatchedMemLocationFrame() const { return patchedMemLocationFrame; }
  void setPatchedMemLocationFrame(const llvm::Value* _patchedMemLocationFrame) { patchedMemLocationFrame = _patchedMemLocationFrame; }

private:
  const llvm::Value* value;
  const llvm::Value* patchedMemLocationFrame;
};

} // namespace psr

namespace std {

template<>
struct hash<psr::ExtendedValue> {

  std::size_t operator()(const psr::ExtendedValue& ev) const {
    uintptr_t value = reinterpret_cast<uintptr_t>(ev.getValue());
    uintptr_t patchedMemLocationFrame = reinterpret_cast<uintptr_t>(ev.getPatchedMemLocationFrame());

    return hash<uintptr_t>()(value) ^
           hash<uintptr_t>()(patchedMemLocationFrame);
  }
};

} // namespace std

#endif // EXTENDEDVALUE_H

