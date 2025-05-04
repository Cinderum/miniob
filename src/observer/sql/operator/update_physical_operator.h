#pragma once

#include <vector>
#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"

class Trx;
class UpdateStmt;

class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, Value &&values, std::string attribute_name) 
  : table_(table), value_(std::move(values)), attribute_name_(attribute_name) {}
 
  virtual ~UpdatePhysicalOperator() = default;
 
  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::UPDATE;
  }
  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
 
  Tuple *current_tuple() override { return nullptr; }
 
private:
  Table *table_ = nullptr;
  Trx *trx_ = nullptr;
  Value value_;
  std::string attribute_name_;
};

