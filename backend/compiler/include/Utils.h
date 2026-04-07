#ifndef UTILS_H
#define UTILS_H

#include <string>

#ifdef _WIN32
#include <windows.h>

// Converts a wide string (UTF-16 on Windows) to a standard UTF-8 string.
inline std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

// Converts a standard UTF-8 string to a wide string (UTF-16 on Windows).
inline std::wstring stringToWstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstrTo[0], size_needed);
    return wstrTo;
}
#else
#include <cstdlib>
#include <vector>

// Converts a wide string to a standard UTF-8 string using standard C library features.
inline std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    std::vector<char> buffer(wstr.length() * 4 + 1);
    std::wcstombs(buffer.data(), wstr.c_str(), buffer.size());
    return std::string(buffer.data());
}

// Converts a standard UTF-8 string to a wide string using standard C library features.
inline std::wstring stringToWstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    std::vector<wchar_t> buffer(str.length() + 1);
    std::mbstowcs(buffer.data(), str.c_str(), buffer.size());
    return std::wstring(buffer.data());
}
#endif

#endif
