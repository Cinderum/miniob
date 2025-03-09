/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

/**
 * @brief 属性的类型
 * @details AttrType 枚举列出了属性的各种数据类型。
 */
enum class AttrType
{
  UNDEFINED,
  CHARS,     ///< 字符串类型
  INTS,      ///< 整数类型(4字节)
  FLOATS,    ///< 浮点数类型(4字节)
  DATES,     ///< 日期类型
  VECTORS,   ///< 向量类型
  BOOLEANS,  ///< boolean类型，当前不是由parser解析出来的，是程序内部使用的
  MAXTYPE,   ///< 请在 UNDEFINED 与 MAXTYPE 之间增加新类型
};

/* enum class 
  一种强类型的枚举
  枚举值不会泄露到外部作用域，必须通过AttrType::UNDEFINED的方式访问，而不是直接使用
  enum class 不能隐式转换为整数或其他类型，必须显式转换(传统的enum可以隐式转换为整数)*/

const char *attr_type_to_string(AttrType type); // 返回有效的type对应的小写字符串，对于无效类型，返回"unknown"
AttrType    attr_type_from_string(const char *s); // 从输入字符串判断属于哪个AttrType
