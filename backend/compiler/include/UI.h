#ifndef UI_H
#define UI_H
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>

#define IDC_CODE_INPUT   101
#define IDC_COMPILE_BTN  102
#define IDC_ERROR_OUTPUT 103
#define IDC_CCODE_OUTPUT 104

class CompilerUI {
public:
    CompilerUI(HINSTANCE hInstance);
    int run();

    void setErrorOutput(const std::vector<std::string>& errors);
    void setCCodeOutput(const std::string& ccode);

    std::string getCodeInput() const;

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static CompilerUI* instance;

    void onCompile();

private:
    HWND hwndMain;
    HWND hwndCodeInput;
    HWND hwndCompileBtn;
    HWND hwndErrorOutput;
    HWND hwndCCodeOutput;

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void createControls(HWND hwnd);

    std::vector<std::string> lastErrors;
    std::string lastCCode;
};

#else

using HINSTANCE = void*;
using HWND = void*;
using UINT = unsigned int;
using WPARAM = unsigned long long;
using LPARAM = long long;
using LRESULT = long long;

#ifndef CALLBACK
#define CALLBACK
#endif

class CompilerUI {
public:
    explicit CompilerUI(HINSTANCE) {}
    int run() { return 0; }

    void setErrorOutput(const std::vector<std::string>&) {}
    void setCCodeOutput(const std::string&) {}

    std::string getCodeInput() const { return std::string(); }

    LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM) { return 0; }

    static CompilerUI* instance;

    void onCompile() {}
};

#endif

#endif 
