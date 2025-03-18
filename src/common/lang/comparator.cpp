/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by wangyunlai on 2021/6/11.
//

#include "common/defs.h"
#include <string.h>

#include "common/lang/algorithm.h"
#include "common/log/log.h"

namespace common {

int compare_int(void *arg1, void *arg2)
{
  int v1 = *(int *)arg1;
  int v2 = *(int *)arg2;
  if (v1 > v2) {
    return 1;
  } else if (v1 < v2) {
    return -1;
  } else {
    return 0;
  }
}

int compare_float(void *arg1, void *arg2)
{
  float v1  = *(float *)arg1;
  float v2  = *(float *)arg2;
  float cmp = v1 - v2;
  if (cmp > EPSILON) {
    return 1;
  }
  if (cmp < -EPSILON) {
    return -1;
  }
  return 0;
}

// 模式匹配
int pattern_match(const char *str, const char *pattern) {
  if (*pattern == '\0') {
      return *str == '\0'; // 模式匹配到末尾
  }
  
  if (*pattern == '%') {
      // 跳过连续的 '%' 符号
      while (*(pattern + 1) == '%') {
          pattern++;
      }
      
      if (*(pattern + 1) == '\0') {
          return 1; // 模式以 '%' 结尾，必然匹配
      }
      
      // 递归尝试匹配剩余部分
      while (*str) {
          if (pattern_match(str, pattern + 1)) {
              return 1;
          }
          str++;
      }
      return 0;
  }
  
  if (*str && (*pattern == '_' || *str == *pattern)) {
      return pattern_match(str + 1, pattern + 1);
  }
  
  return 0;
}

int compare_string(void *arg1, int arg1_max_length, void *arg2, int arg2_max_length)
{
  const char *s1     = (const char *)arg1;
  const char *s2     = (const char *)arg2;
  
  LOG_DEBUG("左字符串为%s", s1);
  LOG_DEBUG("右字符串为%s", s2);
  int         maxlen = min(arg1_max_length, arg2_max_length);
  int         result = strncmp(s1, s2, maxlen); // 判断是否完全匹配
  if (0 != result) {
    LOG_DEBUG("字符串不相等");
    result = result < 0 ? -1 : 1; 
    // 模式匹配
    if (pattern_match(s1, s2 ))
    {
      LOG_DEBUG("字符串匹配成功");
      return 2;
    }
    else 
    {
      LOG_DEBUG("字符串匹配失败");

      if (arg1_max_length > maxlen) {
        return 1;
      }
    
      if (arg2_max_length > maxlen) {
        return -1;
      }

      return -2;
    }
  }
  LOG_DEBUG("字符串匹配成功");
  return 0;
}

int compare_date(void *arg1, void *arg2)
{
  const int32_t date1 = *(const int32_t *)arg1;
  const int32_t date2 = *(const int32_t *)arg2;
  if (date1 > date2) {
    return 1;
  } else if (date1 < date2) {
    return -1;
  } else {
    return 0;
  }
}

}  // namespace common
