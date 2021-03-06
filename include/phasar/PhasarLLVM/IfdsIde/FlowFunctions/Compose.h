/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * Compose.h
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_FLOW_FUNC_COMPOSE_H_
#define ANALYSIS_IFDS_IDE_FLOW_FUNC_COMPOSE_H_

#include <phasar/PhasarLLVM/IfdsIde/FlowFunction.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/Identity.h>
#include <set>
#include <vector>

namespace psr {

template <typename D> class Compose : FlowFunction<D> {
private:
  const std::vector<FlowFunction<D>> funcs;

public:
  Compose(const std::vector<FlowFunction<D>> &funcs) : funcs(funcs) {}

  virtual ~Compose() = default;

  std::set<D> computeTargets(const D &source) override {
    std::set<D> current(source);
    for (const FlowFunction<D> &func : funcs) {
      std::set<D> next;
      for (const D &d : current) {
        std::set<D> target = func.computeTargets(d);
        next.insert(target.begin(), target.end());
      }
      current = next;
    }
    return current;
  }

  static std::shared_ptr<FlowFunction<D>>
  compose(const std::vector<FlowFunction<D>> &funcs) {
    std::vector<FlowFunction<D>> vec;
    for (const FlowFunction<D> &func : funcs)
      if (func != Identity<D>::getInstance())
        vec.insert(func);
    if (vec.size == 1)
      return vec[0];
    else if (vec.empty())
      return Identity<D>::getInstance();
    return std::make_shared<Compose>(vec);
  }
};

} // namespace psr

#endif /* ANALYSIS_IFDS_IDE_FLOW_FUNC_COMPOSE_HH_ */
