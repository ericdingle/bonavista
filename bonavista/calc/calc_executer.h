#ifndef CALC_CALC_EXECUTER_H_
#define CALC_CALC_EXECUTER_H_

#include "executer/executer.h"

class CalcExecuter : public Executer {
 public:
  using Executer::Executer;
  CalcExecuter(const CalcExecuter&) = delete;
  CalcExecuter& operator=(const CalcExecuter&) = delete;
  ~CalcExecuter() override = default;

 protected:
  absl::StatusOr<std::any> ExecuteNode(const Node& node) override;
};

#endif  // CALC_CALC_EXECUTER_H_
