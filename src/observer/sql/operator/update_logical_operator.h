#pragma once

#include <vector>

#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"
#include "storage/table/table.h"

/**
 * @brief 更新逻辑算子
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, Value values, std::string attribute_name);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  std::string attribute_name() { return attribute_name_; }

  Table      *table() const { return table_; }
  const Value value() const { return values_; }
  Value       value() { return values_; }

private:
  Table      *table_ = nullptr;
  Value       values_;
  std::string attribute_name_;
};