#pragma once

#include "common/sys/rc.h"
#include "sql/stmt/stmt.h"
#include <string>

class Table;
class FilterStmt;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt 
{
public:
  UpdateStmt() = default;
  UpdateStmt(Table *table, FilterStmt *filter_stmt, Value value, std::string attribute_name);
  StmtType type() const override
  {
    return StmtType::UPDATE;
  }

  FilterStmt *filter_stmt() const
  {
    return filter_stmt_;
  }

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table *table() const
  {
    return table_;
  }
  const Value value() const
  {
    return value_;
  }
  std::string attribute_name() 
  {
    return attribute_name_;
  }

private:
  Table *table_ = nullptr;
  FilterStmt *filter_stmt_ = nullptr;
  Value value_;
  std::string attribute_name_;
};