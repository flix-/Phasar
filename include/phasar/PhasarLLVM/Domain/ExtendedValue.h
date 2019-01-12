#ifndef EXTENDEDVALUE_H
#define EXTENDEDVALUE_H

#include <cassert>
#include <cstdint>
#include <functional>

namespace llvm {
class Value;
} // namespace llvm

namespace psr {

class ExtendedValue {
public:
  ExtendedValue() {}
  ExtendedValue(const llvm::Value* _value,
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
    return std::less<const llvm::Value*>()(value, rhs.value) ||
           std::less<const llvm::Value*>()(patchedMemLocationFrame, rhs.patchedMemLocationFrame);
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

