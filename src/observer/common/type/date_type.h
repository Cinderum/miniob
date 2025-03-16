#pragma once

#include "common/type/data_type.h"

/**
 * @brief date型数据类型
 * @ingroup DataType
 */

class DateType : public DataType
{
public:
  DateType() : DataType(AttrType::DATES) {}
  virtual ~DateType() = default;

  int compare(const Value &left, const Value &right) const;

  // RC add(const Value &left, const Value &right, Value &result) const override;
  // RC subtract(const Value &left, const Value &right, Value &result) const override;
  // RC multiply(const Value &left, const Value &right, Value &result) const override;
  // RC divide(const Value &left, const Value &right, Value &result) const override;
  // RC negative(const Value &val, Value &result) const override;

  RC set_value_from_str(Value &val, const string &data) const override;

  RC to_string(const Value &val, string &result) const override;
  static inline bool is_leap_year(const int &year);
  static bool is_valid_date(int32_t date);
};