//
// Created by tongj on 24-11-13.
//

#ifndef TBS_TOOL_LIB_BASE_INCLUDE_STRING_UTILS_H_
#define TBS_TOOL_LIB_BASE_INCLUDE_STRING_UTILS_H_

#include <string>
#include <tbs/defs.h>

/**
 * 检查给定的C风格字符串是否为空
 * @param str 待检查的C风格字符串
 * @return 如果字符串为空则返回true，否则返回false
 */
bool isEmpty(CONST char *str);

/**
 * 检查给定的std::string对象是否为空
 * @param str 待检查的字符串对象
 * @return 如果字符串为空则返回true，否则返回false
 */
bool isEmpty(CONST std::string &str);

/**
 * 检查给定的std::wstring对象是否为空
 * @param str 待检查的宽字符串对象
 * @return 如果字符串为空则返回true，否则返回false
 */
bool isEmpty(CONST std::wstring &str);

/**
 * 检查给定的宽字符C风格字符串是否为空
 * @param str 待检查的宽字符C风格字符串
 * @return 如果字符串为空则返回true，否则返回false
 */
bool isEmpty(CONST wchar_t *str);

/**
 * 检查给定的C风格字符串是否只包含数字字符
 * @param str 待检查的C风格字符串
 * @return 如果字符串只包含数字字符则返回true，否则返回false
 */
bool isNumber(CONST char *str);

/**
 * 检查给定的std::string对象是否只包含数字字符
 * @param str 待检查的字符串对象
 * @return 如果字符串只包含数字字符则返回true，否则返回false
 */
bool isNumber(CONST std::string &str);

/**
 * 检查给定的std::wstring对象是否只包含数字字符
 * @param str 待检查的宽字符串对象
 * @return 如果字符串只包含数字字符则返回true，否则返回false
 */
bool isNumber(CONST std::wstring &str);

/**
 * 检查给定的宽字符C风格字符串是否只包含数字字符
 * @param str 待检查的宽字符C风格字符串
 * @return 如果字符串只包含数字字符则返回true，否则返回false
 */
bool isNumber(CONST wchar_t *str);

/**
 * 检查给定的字符是否为数字字符
 * @param c 待检查的字符
 * @return 如果字符是数字字符则返回true，否则返回false
 */
bool isNumber(CONST char c);

/**
 * 检查给定的宽字符是否为数字字符
 * @param c 待检查的宽字符
 * @return 如果字符是数字字符则返回true，否则返回false
 */
bool isNumber(CONST wchar_t c);

/**
 * 检查给定的C风格字符串的指定范围是否只包含数字字符
 * @param str 待检查的C风格字符串
 * @param start 检查的起始位置
 * @param end 检查的结束位置
 * @return 如果指定范围内的字符只包含数字字符则返回true，否则返回false
 */
bool isNumber(CONST char *str, int start, int end);

/**
 * 检查给定的C风格字符串是否只包含字母字符
 * @param str 待检查的C风格字符串
 * @return 如果字符串只包含字母字符则返回true，否则返回false
 */
bool isCharacters(CONST char *str);

/**
 * 检查给定的std::string对象是否只包含字母字符
 * @param str 待检查的字符串对象
 * @return 如果字符串只包含字母字符则返回true，否则返回false
 */
bool isCharacters(CONST std::string &str);

/**
 * 检查给定的std::wstring对象是否只包含字母字符
 * @param str 待检查的宽字符串对象
 * @return 如果字符串只包含字母字符则返回true，否则返回false
 */
bool isCharacters(CONST std::wstring &str);

/**
 * 检查给定的宽字符C风格字符串是否只包含字母字符
 * @param str 待检查的宽字符C风格字符串
 * @return 如果字符串只包含字母字符则返回true，否则返回false
 */
bool isCharacters(CONST wchar_t *str);

/**
 * 检查给定的字符是否为字母字符
 * @param c 待检查的字符
 * @return 如果字符是字母字符则返回true，否则返回false
 */
bool isCharacters(CONST char c);

/**
 * 检查给定的宽字符是否为字母字符
 * @param c 待检查的宽字符
 * @return 如果字符是字母字符则返回true，否则返回false
 */
bool isCharacters(CONST wchar_t c);

/**
 * 检查给定的C风格字符串的指定范围是否只包含字母字符
 * @param str 待检查的C风格字符串
 * @param start 检查的起始位置
 * @param end 检查的结束位置
 * @return 如果指定范围内的字符只包含字母字符则返回true，否则返回false
 */
bool isCharacters(CONST char *str, int start, int end);

/**
 * 计算给定的C风格字符串的长度
 * @param str 待计算长度的C风格字符串
 * @return 字符串的长度
 */
size_t strLength(CONST char *str);

/**
 * 计算给定的C风格字符串的长度
 * @param str 待计算长度的C风格字符串
 * @return 字符串的长度
 */
size_t strLength(CONST wchar_t *str);

/**
 * @brief 将给定的 C 风格字符串转换为小写。
 *
 * @param str 要转换的 C 风格字符串。
 * @return std::string 转换后的小写字符串。
 */
std::string toLower(CONST char *str);

/**
 * @brief 将给定的 `std::string` 转换为小写。
 *
 * @param str 要转换的 `std::string`。
 * @return std::string 转换后的小写字符串。
 */
std::string toLower(CONST std::string &str);

/**
 * @brief 将给定的宽字符 C 风格字符串转换为小写。
 *
 * @param str 要转换的宽字符 C 风格字符串。
 * @return std::wstring 转换后的小写字符串。
 */
std::wstring toLower(CONST wchar_t *str);

/**
 * @brief 将给定的 `std::wstring` 转换为小写。
 *
 * @param str 要转换的 `std::wstring`。
 * @return std::wstring 转换后的小写字符串。
 */
std::wstring toLower(CONST std::wstring &str);

/**
 * @brief 将给定的 C 风格字符串转换为大写。
 *
 * @param str 要转换的 C 风格字符串。
 * @return std::string 转换后的大写字符串。
 */
std::string toUpper(CONST char *str);

/**
 * @brief 将给定的 `std::string` 转换为大写。
 *
 * @param str 要转换的 `std::string`。
 * @return std::string 转换后的大写字符串。
 */
std::string toUpper(CONST std::string &str);

/**
 * @brief 将给定的宽字符 C 风格字符串转换为大写。
 *
 * @param str 要转换的宽字符 C 风格字符串。
 * @return std::wstring 转换后的大写字符串。
 */
std::wstring toUpper(CONST wchar_t *str);

/**
 * @brief 将给定的 `std::wstring` 转换为大写。
 *
 * @param str 要转换的 `std::wstring`。
 * @return std::wstring 转换后的大写字符串。
 */
std::wstring toUpper(CONST std::wstring &str);

/**
 * @brief 从左侧移除指定字符集中的字符。
 *
 * @param str 要处理的 C 风格字符串。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::string 处理后的字符串。
 */
std::string trimLeft(CONST char *str, CONST char *chars = " \t\n\r");

/**
 * @brief 从左侧移除指定字符集中的字符。
 *
 * @param str 要处理的 `std::string`。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::string 处理后的字符串。
 */
std::string trimLeft(CONST std::string &str, CONST char *chars = " \t\n\r");

/**
 * @brief 从左侧移除指定字符集中的字符。
 *
 * @param str 要处理的宽字符 C 风格字符串。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::wstring 处理后的字符串。
 */
std::wstring trimLeft(CONST wchar_t *str, CONST wchar_t *chars = L" \t\n\r");

/**
 * @brief 从左侧移除指定字符集中的字符。
 *
 * @param str 要处理的 `std::wstring`。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::wstring 处理后的字符串。
 */
std::wstring trimLeft(CONST std::wstring &str, CONST wchar_t *chars = L" \t\n\r");

/**
 * @brief 从右侧移除指定字符集中的字符。
 *
 * @param str 要处理的 C 风格字符串。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::string 处理后的字符串。
 */
std::string trimRight(CONST char *str, CONST char *chars = " \t\n\r");

/**
 * @brief 从右侧移除指定字符集中的字符。
 *
 * @param str 要处理的 `std::string`。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::string 处理后的字符串。
 */
std::string trimRight(CONST std::string &str, CONST char *chars = " \t\n\r");

/**
 * @brief 从右侧移除指定字符集中的字符。
 *
 * @param str 要处理的宽字符 C 风格字符串。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::wstring 处理后的字符串。
 */
std::wstring trimRight(CONST wchar_t *str, CONST wchar_t *chars = L" \t\n\r");

/**
 * @brief 从右侧移除指定字符集中的字符。
 *
 * @param str 要处理的 `std::wstring`。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::wstring 处理后的字符串。
 */
std::wstring trimRight(CONST std::wstring &str, CONST wchar_t *chars = L" \t\n\r");

/**
 * @brief 从两侧移除指定字符集中的字符。
 *
 * @param str 要处理的 C 风格字符串。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::string 处理后的字符串。
 */
std::string trim(CONST char *str, CONST char *chars = " \t\n\r");

/**
 * @brief 从两侧移除指定字符集中的字符。
 *
 * @param str 要处理的 `std::string`。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::string 处理后的字符串。
 */
std::string trim(CONST std::string &str, CONST char *chars = " \t\n\r");

/**
 * @brief 从两侧移除指定字符集中的字符。
 *
 * @param str 要处理的宽字符 C 风格字符串。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::wstring 处理后的字符串。
 */
std::wstring trim(CONST wchar_t *str, CONST wchar_t *chars = L" \t\n\r");

/**
 * @brief 从两侧移除指定字符集中的字符。
 *
 * @param str 要处理的 `std::wstring`。
 * @param chars 要移除的字符集，默认为空白字符。
 * @return std::wstring 处理后的字符串。
 */
std::wstring trim(CONST std::wstring &str, CONST wchar_t *chars = L" \t\n\r");

/**
 * @brief 在字符串中查找子字符串的位置。
 *
 * @param str 要搜索的字符串。
 * @param subStr 要查找的子字符串。
 * @param found 指示是否找到子字符串的布尔值。
 * @return size_t 子字符串的位置，如果未找到则返回字符串长度。
 */
size_t find(CONST char *str, CONST char *subStr, bool &found);

/**
 * @brief 在字符串中查找子字符串的位置。
 *
 * @param str 要搜索的 `std::string`。
 * @param subStr 要查找的子字符串。
 * @param found 指示是否找到子字符串的布尔值。
 * @return size_t 子字符串的位置，如果未找到则返回字符串长度。
 */
size_t find(CONST std::string &str, CONST char *subStr, bool &found);

/**
 * @brief 在宽字符字符串中查找子字符串的位置。
 *
 * @param str 要搜索的宽字符字符串。
 * @param subStr 要查找的子字符串。
 * @param found 指示是否找到子字符串的布尔值。
 * @return size_t 子字符串的位置，如果未找到则返回字符串长度。
 */
size_t find(CONST wchar_t *str, CONST wchar_t *subStr, bool &found);

/**
 * @brief 在宽字符字符串中查找子字符串的位置。
 *
 * @param str 要搜索的 `std::wstring`。
 * @param subStr 要查找的子字符串。
 * @param found 指示是否找到子字符串的布尔值。
 * @return size_t 子字符串的位置，如果未找到则返回字符串长度。
 */
size_t find(CONST std::wstring &str, CONST wchar_t *subStr, bool &found);

/**
 * @brief 替换字符串中的子字符串。
 *
 * @param str 原始字符串。
 * @param oldStr 要替换的子字符串。
 * @param newStr 新的子字符串。
 * @return std::string 替换后的字符串。
 */
std::string replace(CONST char *str, CONST char *oldStr, CONST char *newStr);

/**
 * @brief 替换字符串中的子字符串。
 *
 * @param str 原始 `std::string`。
 * @param oldStr 要替换的子字符串。
 * @param newStr 新的子字符串。
 * @return std::string 替换后的字符串。
 */
std::string replace(CONST std::string &str, CONST char *oldStr, CONST char *newStr);

/**
 * @brief 替换宽字符字符串中的子字符串。
 *
 * @param str 原始宽字符字符串。
 * @param oldStr 要替换的子字符串。
 * @param newStr 新的子字符串。
 * @return std::wstring 替换后的字符串。
 */
std::wstring replace(CONST wchar_t *str, CONST wchar_t *oldStr, CONST wchar_t *newStr);

/**
 * @brief 替换宽字符字符串中的子字符串。
 *
 * @param str 原始 `std::wstring`。
 * @param oldStr 要替换的子字符串。
 * @param newStr 新的子字符串。
 * @return std::wstring 替换后的字符串。
 */
std::wstring replace(CONST std::wstring &str, CONST wchar_t *oldStr, CONST wchar_t *newStr);

/**
 * @brief 替换字符串中的子字符串。
 *
 * @param str 原始字符串。
 * @param oldStr 要替换的子字符串。
 * @param newStr 新的子字符串。
 * @return std::string 替换后的字符串。
 */
std::string replace(CONST char *str, CONST char *oldStr, CONST std::string &newStr);

#endif //TBS_TOOL_LIB_BASE_INCLUDE_STRING_UTILS_H_
