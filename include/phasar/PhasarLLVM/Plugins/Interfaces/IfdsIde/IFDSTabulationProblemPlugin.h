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

#ifndef SRC_ANALYSIS_PLUGINS_IFDSTABULATIONPROBLEMPLUGIN_H_
#define SRC_ANALYSIS_PLUGINS_IFDSTABULATIONPROBLEMPLUGIN_H_

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h>
#include <phasar/PhasarLLVM/IfdsIde/DefaultIFDSTabulationProblem.h>
#include <phasar/PhasarLLVM/IfdsIde/LLVMZeroValue.h>
#include <phasar/Utils/LLVMShorthands.h>
#include <string>
#include <vector>

namespace psr {

class IFDSTabulationProblemPlugin
    : public DefaultIFDSTabulationProblem<
          const llvm::Instruction *, const llvm::Value *,
          const llvm::Function *, LLVMBasedICFG &> {
protected:
  std::vector<std::string> EntryPoints;

public:
  IFDSTabulationProblemPlugin(LLVMBasedICFG &ICFG,
                              std::vector<std::string> EntryPoints = {"main"})
      : DefaultIFDSTabulationProblem<const llvm::Instruction *,
                                     const llvm::Value *,
                                     const llvm::Function *, LLVMBasedICFG &>(
            ICFG),
        EntryPoints(EntryPoints) {
    DefaultIFDSTabulationProblem::zerovalue = createZeroValue();
  }
  ~IFDSTabulationProblemPlugin() = default;

  const llvm::Value *createZeroValue() override {
    // create a special value to represent the zero value!
    return LLVMZeroValue::getInstance();
  }

  bool isZeroValue(const llvm::Value *d) const override {
    return isLLVMZeroValue(d);
  }

  std::string DtoString(const llvm::Value *d) const override {
    return llvmIRToString(d);
  }

  std::string NtoString(const llvm::Instruction *n) const override {
    return llvmIRToString(n);
  }

  std::string MtoString(const llvm::Function *m) const override {
    return llvmIRToString(m);
  }
};

extern std::map<std::string,
                std::unique_ptr<IFDSTabulationProblemPlugin> (*)(
                    LLVMBasedICFG &I, std::vector<std::string> EntryPoints)>
    IFDSTabulationProblemPluginFactory;

} // namespace psr

#endif /* SRC_ANALYSIS_PLUGINS_IFDSTABULATIONPROBLEMPLUGIN_HH_ */
