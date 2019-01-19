/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * IFDSTabulationProblemPlugin.h
 *
 *  Created on: 14.06.2017
 *      Author: philipp
 */

#ifndef PHASAR_PHASARLLVM_PLUGINS_INTERFACES_IFDSIDE_IFDSTABULATIONPROBLEMPLUGINEXTENDEDVALUE_H_
#define PHASAR_PHASARLLVM_PLUGINS_INTERFACES_IFDSIDE_IFDSTABULATIONPROBLEMPLUGINEXTENDEDVALUE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Value.h>

#include <phasar/PhasarLLVM/IfdsIde/DefaultIFDSTabulationProblem.h>
#include <phasar/PhasarLLVM/IfdsIde/LLVMZeroValue.h>
#include <phasar/PhasarLLVM/Domain/ExtendedValue.h>
#include <phasar/Utils/LLVMShorthands.h>

namespace psr {

class LLVMBasedICFG;

class IFDSTabulationProblemPluginExtendedValue
    : public DefaultIFDSTabulationProblem<const llvm::Instruction*,
                                          ExtendedValue,
                                          const llvm::Function*,
                                          LLVMBasedICFG &> {
protected:
  std::vector<std::string> EntryPoints;

public:
  IFDSTabulationProblemPluginExtendedValue(LLVMBasedICFG &ICFG,
                                           std::vector<std::string> EntryPoints = {"main"})
      : DefaultIFDSTabulationProblem<const llvm::Instruction*,
                                     ExtendedValue,
                                     const llvm::Function*,
                                     LLVMBasedICFG &>(ICFG),
        EntryPoints(EntryPoints) {
    DefaultIFDSTabulationProblem::zerovalue = createZeroValue();
  }
  ~IFDSTabulationProblemPluginExtendedValue() = default;

  ExtendedValue createZeroValue() override {
    // create a special value to represent the zero value!
    return ExtendedValue(LLVMZeroValue::getInstance());
  }

  bool isZeroValue(ExtendedValue ev) const override {
    return isLLVMZeroValue(ev.getValue());
  }

  void printNode(std::ostream &os, const llvm::Instruction *n) const override {
    os << llvmIRToString(n);
  }

  void printDataFlowFact(std::ostream &os,
                         ExtendedValue ev) const override {
    os << llvmIRToString(ev.getValue()) << "\n";
    for (const auto memLocationPart : ev.getMemLocationSeq()) {
      os << "A:\t" << llvmIRToString(memLocationPart) << "\n";
    }
    if (!ev.getEndOfTaintedBlockLabel().empty()) {
        os << "L:\t" << ev.getEndOfTaintedBlockLabel() << "\n";
    }
  }

  void printMethod(std::ostream &os, const llvm::Function *m) const override {
    os << m->getName().str();
  }

  virtual void printReport() {}
};

extern std::map<std::string,
                std::unique_ptr<IFDSTabulationProblemPluginExtendedValue> (*)(
                    LLVMBasedICFG &I, std::vector<std::string> EntryPoints)>
    IFDSTabulationProblemPluginExtendedValueFactory;

} // namespace psr

#endif
