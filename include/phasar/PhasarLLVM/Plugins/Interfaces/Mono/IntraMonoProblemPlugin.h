/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#ifndef PHASAR_PHASARLLVM_PLUGINS_INTERFACES_MONO_INTRAMONOPROBLEMPLUGIN_H_
#define PHASAR_PHASARLLVM_PLUGINS_INTERFACES_MONO_INTRAMONOPROBLEMPLUGIN_H_

#include <map>
#include <memory>
#include <string>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include <phasar/PhasarLLVM/Mono/IntraMonoProblem.h>
#include <phasar/Utils/LLVMShorthands.h>

namespace psr {

class LLVMBasedCFG;

class IntraMonoProblemPlugin
    : public IntraMonoProblem<const llvm::Instruction*,
                              const llvm::Value*,
                              const llvm::Function*,
                              LLVMBasedCFG&> {

public:
  IntraMonoProblemPlugin(LLVMBasedCFG& cfg, const llvm::Function* f)
    : IntraMonoProblem(cfg, f) {}
  virtual ~IntraMonoProblemPlugin() override = default;

  virtual MonoSet<const llvm::Value*> join(const MonoSet<const llvm::Value*> &Lhs, const MonoSet<const llvm::Value*> &Rhs) override = 0;
  virtual bool sqSubSetEqual(const MonoSet<const llvm::Value*> &Lhs, const MonoSet<const llvm::Value*> &Rhs) override = 0;
  virtual MonoSet<const llvm::Value*> flow(const llvm::Instruction* S, const MonoSet<const llvm::Value*> &In) override = 0;
  virtual MonoMap<const llvm::Instruction*, MonoSet<const llvm::Value*>> initialSeeds() override = 0;

  void printNode(std::ostream &os, const llvm::Instruction *n) const override {
    os << llvmIRToString(n);
  }

  void printDataFlowFact(std::ostream &os, const llvm::Value *d) const override {
    os << llvmIRToString(d);
  }

  void printMethod(std::ostream &os, const llvm::Function *m) const override {
    os << m->getName().str();
  }
};

extern std::map<std::string,
                std::unique_ptr<IntraMonoProblemPlugin> (*)(
                    LLVMBasedCFG& cfg,
                    const llvm::Function* f)>
    IntraMonoProblemPluginFactory;

} // namespace

#endif

