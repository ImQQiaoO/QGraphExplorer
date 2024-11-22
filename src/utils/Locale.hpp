#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <io.h>
#include <locale>
#include <windows.h>

namespace utils {

    template <typename _ = void>
    inline bool starts_with(const std::string &str, const std::string &prefix) {
        return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
    }

    template <typename _ = void>
    inline std::string ansi_to_utf8(const std::string &ansi_str) {
        int wide_size = MultiByteToWideChar(CP_ACP, 0, ansi_str.c_str(), -1, nullptr, 0);
        if (wide_size <= 0) return {};
        std::wstring wide_str(wide_size, L'\0');
        MultiByteToWideChar(CP_ACP, 0, ansi_str.c_str(), -1, &wide_str[0], wide_size);
        int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (utf8_size <= 0) return {};
        std::string utf8_str(utf8_size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide_str.c_str(), -1, &utf8_str[0], utf8_size, nullptr, nullptr);
        utf8_str.resize(utf8_size - 1);

        return utf8_str;
    }

    template <typename _ = void>
    inline std::string utf8_to_ansi(const std::string &utf8_str) {
        int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
        if (wide_size <= 0) return {};
        std::wstring wide_str(wide_size, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, wide_str.data(), wide_size);
        int ansi_size = WideCharToMultiByte(CP_ACP, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (ansi_size <= 0) return {};
        std::string ansi_str(ansi_size, '\0');
        WideCharToMultiByte(CP_ACP, 0, wide_str.c_str(), -1, ansi_str.data(), ansi_size, nullptr, nullptr);
        ansi_str.resize(ansi_size - 1);

        return ansi_str;
    }

    template <typename _ = void>
    inline std::string utf8_to_unicode_escape(std::string_view utf8_str) {
        const char *src_str = utf8_str.data();
        int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, nullptr, 0);
        const std::size_t wstr_length = static_cast<std::size_t>(len) + 1U;
        auto wstr = new wchar_t[wstr_length];
        memset(wstr, 0, sizeof(wstr[0]) * wstr_length);
        MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wstr, len);

        std::string unicode_escape_str = {};
        constexpr static char hex_code[] = "0123456789abcdef";
        for (const wchar_t *pchr = wstr; *pchr; ++pchr) {
            const wchar_t &chr = *pchr;
            if (chr > 255) {
                unicode_escape_str += "\\u";
                unicode_escape_str.push_back(hex_code[chr >> 12]);
                unicode_escape_str.push_back(hex_code[(chr >> 8) & 15]);
                unicode_escape_str.push_back(hex_code[(chr >> 4) & 15]);
                unicode_escape_str.push_back(hex_code[chr & 15]);
            } else {
                unicode_escape_str.push_back(chr & 255);
            }
        }

        delete[] wstr;
        wstr = nullptr;

        return unicode_escape_str;
    }

    inline std::string load_file_without_bom(const std::filesystem::path &path) {
        std::ifstream ifs(path, std::ios::in);
        if (!ifs.is_open()) {
            return {};
        }
        std::stringstream iss;
        iss << ifs.rdbuf();
        ifs.close();
        std::string str = iss.str();

        if (starts_with(str, "\xEF\xBB\xBF")) {
            str.assign(str.begin() + 3, str.end());
        }

        return str;
    }

    class utf8_scope {
#ifdef _WIN32
        bool active;
        int thread_mode;
        std::string old_locale;

        static bool is_console_ostream(std::ostream &ofs) {
            if (&ofs == &std::cout) {
                return _isatty(_fileno(stdout));
            }
            if (&ofs == &std::cerr) {
                return _isatty(_fileno(stderr));
            }
            return false;
        }

    public:
        explicit utf8_scope(std::nullptr_t) {
            active = false;
            thread_mode = 0;
        }

        ~utf8_scope() {
            if (!active) {
                return;
            }
            if (!old_locale.empty()) {
                setlocale(LC_ALL, old_locale.c_str());
            }
            _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
        }

        explicit utf8_scope(std::ostream &ofs) {
            if (is_console_ostream(ofs)) {
                active = true;
                thread_mode = _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
                if (auto old_locale_ptr = setlocale(LC_ALL, ".UTF-8")) {
                    old_locale = old_locale_ptr;
                }
            } else {
                active = false;
            }
        }
#else
    public:
        utf8_scope(std::ostream &) {}
#endif
    };

} // namespace utils

#endif
