#ifndef UI_H
#define UI_H
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>

// UI Element resource boundaries representing interactive widgets.
#define IDC_CODE_INPUT   101
#define IDC_COMPILE_BTN  102
#define IDC_ERROR_OUTPUT 103
#define IDC_CCODE_OUTPUT 104

// Encapsulates the visual presentation logic handling user compilation operations.
class CompilerUI {
public:
    // Ties the UI interface payload.
    CompilerUI(HINSTANCE hInstance);

    // Activates message loop listening pushing events onto the handler.
    int run();

    // Sets failure string streams onto a designated visual region.
    void setErrorOutput(const std::vector<std::string>& errors);

    // Presents resulting text buffers into the respective view field.
    void setCCodeOutput(const std::string& ccode);

    // Queries text area fetching current coding block written directly by users.
    std::string getCodeInput() const;

    // Processes structural widget actions natively mapping messages.
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Globally pointing handle simplifying single-instance interactions.
    static CompilerUI* instance;

    // Trigger boundary representing 'Compile' action invocation by user clicks.
    void onCompile();

private:
    HWND hwndMain;          // Principal surrounding view window context
    HWND hwndCodeInput;     // Native textual handle recording typed codes
    HWND hwndCompileBtn;    // Switch emitting direct command operations
    HWND hwndErrorOutput;   // Secondary textual section targeting faults
    HWND hwndCCodeOutput;   // Separate read-only dump view

    // Universal message wrapper enabling callback hooks properly bound under Windows.
    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Scaffolds default bounds and internal logic wiring all individual elements.
    void createControls(HWND hwnd);

    // Records history retaining unrecoverable states for visualization comparisons.
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
