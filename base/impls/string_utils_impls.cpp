//
// Created by abstergo on 24-11-11.
//
#include <tbs/string_utils.h>

bool isEmpty(const char *str) {
  return str == nullptr || str[0] == '\0';
}
bool isEmpty(const std::string &str) {
  return str.empty();
}
bool isEmpty(const std::wstring &str) {
  return str.empty();
}
bool isEmpty(const wchar_t *str) {
  return str == nullptr || str[0] == '\0';
}
bool isNumber(const char *str) {
  if (isEmpty(str)) {
    return false;
  }
  while (*str++)
    if (!isNumber(*str))
      return false;
  return true;
}
bool isNumber(const std::string &str) {
  return isNumber(str.c_str());
}
bool isNumber(const std::wstring &str) {
  return isNumber(str.c_str());
}
bool isNumber(const wchar_t *str) {
  while (*str++)
    if (!isNumber(*str))
      return false;
  return true;
}
bool isNumber(const char c) {
  return c >= '0' && c <= '9';
}
bool isNumber(const wchar_t c) {
  return c >= L'0' && c <= L'9';
}
bool isNumber(const char *str, int start, int end) {
  if (isEmpty(str)) {
    return false;
  }
  for (int i = start; i < end; i++)
    if (!isNumber(str[i]))
      return false;
  return true;
}
bool isCharacters(const char *str) {
  while (*str++)
    if (!isCharacters(*str))
      return false;
  return true;
}
bool isCharacters(const std::string &str) {
  return isCharacters(str.c_str());
}
bool isCharacters(const std::wstring &str) {
  return isCharacters(str.c_str());
}
bool isCharacters(const wchar_t *str) {
  if (isEmpty(str)) {
    return false;
  }
  while (*str++) {
    if (!isCharacters(*str))
      return false;
  }
  return true;
}
bool isCharacters(const char c) {
  return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}
bool isCharacters(const wchar_t c) {
  return c >= L'a' && c <= L'z' || c >= L'A' && c <= L'Z';
}
bool isCharacters(const char *str, int start, int end) {
  if (isEmpty(str)) {
    return false;
  }
  for (int i = start; i < end; i++)
    if (!isCharacters(str[i]))
      return false;
  return true;
}
size_t strLength(const char *str) {
  size_t len = 0;
  while (*str++)
    len++;
  return len;
}
std::string toLower(const char *str) {
  std::string res(str);
  for (char &c : res) {
    if (c >= 'A' && c <= 'Z') {
      c += 32;
    }
  }
  return res;
}
std::string toLower(const std::string &str) {
  return toLower(str.c_str());
}
std::wstring toLower(const wchar_t *str) {
  std::wstring res(str);
  for (wchar_t &c : res) {
    if (c >= L'A' && c <= L'Z') {
      c += 32;
    }
  }
  return res;
}
std::wstring toLower(const std::wstring &str) {
  return toLower(str.c_str());
}
std::string toUpper(const char *str) {
  std::string res(str);
  for (char &c : res) {
    if (c >= 'a' && c <= 'z') {
      c -= 32;
    }
  }
  return res;
}
std::string toUpper(const std::string &str) {
  return toUpper(str.c_str());
}
std::wstring toUpper(const wchar_t *str) {
  std::wstring res(str);
  for (wchar_t &c : res) {
    if (c >= L'a' && c <= L'z') {
      c -= 32;
    }
  }
  return res;
}
std::wstring toUpper(const std::wstring &str) {
  return toUpper(str.c_str());
}
#include <unordered_set>
#include <cstring>
std::string trimLeft(const char *str, const char *chars) {
  std::string res;
  std::unordered_set<char> charsSet;
  while (*chars++)
    charsSet.insert(*chars);
  bool begin = true;
  for (int i = 0; str[i] != '\0'; i++) {
    if (begin) {
      if (charsSet.count(res[i]) > 0) {
        continue;
      } else {
        begin = false;
      }
    }
    res += str[i];
  }
  return res;
}
std::string trimLeft(const std::string &str, const char *chars) {
  return trimLeft(str.c_str(), chars);
}
std::wstring trimLeft(const wchar_t *str, const wchar_t *chars) {
  std::wstring res;
  std::unordered_set<wchar_t> charsSet;
  while (*chars++)
    charsSet.insert(*chars);
  bool begin = true;
  for (int i = 0; str[i] != L'\0'; i++) {
    if (begin) {
      if (charsSet.count(res[i]) > 0) {
        continue;
      } else {
        begin = false;
      }
    }
    res += str[i];
  }
  return res;
}
std::wstring trimLeft(const std::wstring &str, const wchar_t *chars) {
  return trimLeft(str.c_str(), chars);
}
std::string trimRight(const char *str, const char *chars) {
  return trimRight(std::string(str), chars);
}
std::string trimRight(const std::string &str, const char *chars) {
  std::string res(str);
  std::unordered_set<char> charsSet;
  while (*chars++)
    charsSet.insert(*chars);

  while (!res.empty() && charsSet.count(res.back()) > 0) {
    res.pop_back();
  }
  return res;
}
std::wstring trimRight(const wchar_t *str, const wchar_t *chars) {
  return trimRight(std::wstring(str), chars);
}
std::wstring trimRight(const std::wstring &str, const wchar_t *chars) {
  std::wstring res(str);
  std::unordered_set<wchar_t> charsSet;
  while (*chars++)
    charsSet.insert(*chars);

  while (!res.empty() && charsSet.count(res.back()) > 0) {
    res.pop_back();
  }
  return res;
}
std::string trim(const char *str, const char *chars) {
  std::string res = trimLeft(str, chars);
  res = trimRight(res, chars);
  return res;
}
std::string trim(const std::string &str, const char *chars) {
  return trim(str.c_str(), chars);
}
std::wstring trim(const wchar_t *str, const wchar_t *chars) {
  std::wstring res = trimLeft(str, chars);
  res = trimRight(res, chars);
  return res;
}
std::wstring trim(const std::wstring &str, const wchar_t *chars) {
  return trim(str.c_str(), chars);
}
std::string replace(const char *str, const char *oldStr, const char *newStr) {
  std::string res(str);
  size_t oldStrLen = strLength(oldStr);
  res.replace(res.find(oldStr), oldStrLen, newStr);
  return res;
}
std::string replace(const std::string &str, const char *oldStr, const char *newStr) {
  return replace(str.c_str(), oldStr, newStr);
}
std::wstring replace(const wchar_t *str, const wchar_t *oldStr, const wchar_t *newStr) {
  std::wstring res(str);
  size_t oldStrLen = strLength(oldStr);
  res.replace(res.find(oldStr), oldStrLen, newStr);
  return res;
}
std::wstring replace(const std::wstring &str, const wchar_t *oldStr, const wchar_t *newStr) {
  return replace(str.c_str(), oldStr, newStr);
}
std::string replace(const char *str, const char *oldStr, const std::string &newStr) {
  return replace(str, oldStr, newStr.c_str());
}
size_t find(const char *str, const char *subStr, bool &found) {
  found = false;
  if (isEmpty(str)) {
    return 0;
  }
  size_t subStrLen = strLength(subStr);
  if (subStrLen == 0) {
    return 0;
  }
  size_t strLen = strLength(str);
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != subStr[0]) {
      continue;
    }
    if (i + subStrLen > strLen) {
      return 0;
    }
    for (int j = 1; j < subStrLen; j++) {
      if (str[i + j] != subStr[j]) {
        break;
      }
      if (j == subStrLen - 1) {
        found = true;
        return i;
      }
    }
  }
  return 0;
}
size_t find(const std::string &str, const char *subStr, bool &found) {
  found = false;
  if (isEmpty(str)) {
    return 0;
  }
  size_t subStrLen = strLength(subStr);
  if (subStrLen == 0) {
    return 0;
  }
  size_t strLen = str.size();
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != subStr[0]) {
      continue;
    }
    if (i + subStrLen > strLen) {
      return 0;
    }
    for (int j = 1; j < subStrLen; j++) {
      if (str[i + j] != subStr[j]) {
        break;
      }
      if (j == subStrLen - 1) {
        found = true;
        return i;
      }
    }
  }
  return 0;
}
size_t find(const wchar_t *str, const wchar_t *subStr, bool &found) {
  found = false;
  if (isEmpty(str)) {
    return 0;
  }
  size_t subStrLen = strLength(subStr);
  if (subStrLen == 0) {
    return 0;
  }
  size_t strLen = strLength(str);
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != subStr[0]) {
      continue;
    }
    if (i + subStrLen > strLen) {
      return 0;
    }
    for (int j = 1; j < subStrLen; j++) {
      if (str[i + j] != subStr[j]) {
        break;
      }
      if (j == subStrLen - 1) {
        found = true;
        return i;
      }
    }
  }
  return 0;
}
size_t find(const std::wstring &str, const wchar_t *subStr, bool &found) {
  found = false;
  if (isEmpty(str)) {
    return 0;
  }
  size_t subStrLen = strLength(subStr);
  if (subStrLen == 0) {
    return 0;
  }
  size_t strLen = str.size();
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != subStr[0]) {
      continue;
    }
    if (i + subStrLen > strLen) {
      return 0;
    }
    for (int j = 1; j < subStrLen; j++) {
      if (str[i + j] != subStr[j]) {
        break;
      }
      if (j == subStrLen - 1) {
        found = true;
        return i;
      }
    }
  }
  return 0;
}
size_t strLength(const wchar_t *str) {
  size_t i = 0;
  while (str[i] != L'\0') {
    i++;
  }
  return i;
}
