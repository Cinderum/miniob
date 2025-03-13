#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/date_type.h"
#include "common/value.h"
#include "common/lang/limits.h"
#include "common/value.h"

int DateType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
  ASSERT(right.attr_type() == AttrType::DATES, "right type is not date");
  int32_t left_val = left.get_date();
  int32_t right_val = right.get_date();
  return common::compare_date((void *)&left_val, (void *)&right_val);
}

// 日期不需要相加
// RC DateType::add(const Value &left, const Value &right, Value &result) const
// {
//   result.set_float(left.get_float() + right.get_float());
//   return RC::SUCCESS;
// }
// RC FloatType::subtract(const Value &left, const Value &right, Value &result) const
// {
//   result.set_float(left.get_float() - right.get_float());
//   return RC::SUCCESS;
// }
// RC FloatType::multiply(const Value &left, const Value &right, Value &result) const
// {
//   result.set_float(left.get_float() * right.get_float());
//   return RC::SUCCESS;
// }

// RC FloatType::divide(const Value &left, const Value &right, Value &result) const
// {
//   if (right.get_float() > -EPSILON && right.get_float() < EPSILON) {
//     // NOTE:
//     // 设置为浮点数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为浮点数最大值。
//     result.set_float(numeric_limits<float>::max());
//   } else {
//     result.set_float(left.get_float() / right.get_float());
//   }
//   return RC::SUCCESS;
// }

// RC FloatType::negative(const Value &val, Value &result) const
// {
//   result.set_float(-val.get_float());
//   return RC::SUCCESS;
// }

inline bool DateType::is_leap_year(const int &year) const
{
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

RC DateType::set_value_from_str(Value &val, const string &data) const
{
  RC                rc = RC::SUCCESS;
  int year, month, day;  // 分配内存，存储年、月、日
  // 使用 sscanf 解析字符串
  // LOG_INFO("set_value_from_str前!!!!!!!!%s!!!!!!!!", data);
  int ret = sscanf(data.c_str(), "%d-%d-%d", &year, &month, &day);
  if (ret != 3)
  {
    return rc = RC::INVALID_ARGUMENT;
  }

  if (year < 1970 || (year > 2038 && month > 2)
      || month < 1 || month > 12
      || day < 1 || day > 31)
  {
    return rc = RC::INVALID_ARGUMENT;
  }

  int max_day_in_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int max_day = max_day_in_month[month - 1];
  if (!is_leap_year(year))
  {
    max_day--;
  }

  if (day > max_day)
  {
    return rc = RC::INVALID_ARGUMENT;
  }

  int32_t date_values = year * 10000 + month * 100 + day;
  
  val.set_date(date_values);

  // LOG_INFO("set_value_from_str后!!!!!!!!%d!!!!!!!!", date_values);

  return rc;
}

RC DateType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  // LOG_INFO("to_string前!!!!!!!!%d!!!!!!!!", val.value_.date_value_);
  ss << common::date_to_str(val.value_.date_value_);
  result = ss.str();
  // LOG_INFO("to_string后!!!!!!!!%s!!!!!!!!", result);
  return RC::SUCCESS;
}
