#include <windows.h>
#include <string>
#include <cmath>
#include <cwchar>

#define ID_EDIT_X        101
#define ID_EDIT_Y        102
#define ID_BUTTON        103
#define ID_STATIC_RESULT 104

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"Lab3Task1";
    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(0, L"Lab3Task1",
        L"Обчислення f(x, y) = [y / (x - 2)]",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        500, 300, 500, 300, nullptr, nullptr, hInstance, nullptr);
    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}

// Перетворення рядка в число з перевіркою коректності
static bool ParseDouble(const wchar_t* s, double& value)
{
    if (*s == L'\0') return false;
    wchar_t* end = nullptr;
    value = wcstod(s, &end);
    while (*end == L' ') ++end;         
    return *end == L'\0';
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEditX, hEditY, hButton, hResult;

    switch (message)
    {
    case WM_CREATE:
    {
        HINSTANCE hInst = reinterpret_cast<LPCREATESTRUCTW>(lParam)->hInstance;

        hEditX = CreateWindowExW(0, L"EDIT", L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            80, 60, 100, 25, hwnd, reinterpret_cast<HMENU>(ID_EDIT_X), hInst, nullptr);

        hEditY = CreateWindowExW(0, L"EDIT", L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            250, 60, 100, 25, hwnd, reinterpret_cast<HMENU>(ID_EDIT_Y), hInst, nullptr);

        hButton = CreateWindowExW(0, L"BUTTON", L"Розрахувати",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 110, 120, 30, hwnd, reinterpret_cast<HMENU>(ID_BUTTON), hInst, nullptr);

        hResult = CreateWindowExW(0, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE,
            160, 165, 300, 25, hwnd, reinterpret_cast<HMENU>(ID_STATIC_RESULT), hInst, nullptr);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON && HIWORD(wParam) == BN_CLICKED)
        {
            wchar_t bufX[64]{}, bufY[64]{};
            GetWindowTextW(hEditX, bufX, 64);
            GetWindowTextW(hEditY, bufY, 64);

            double x, y;
            if (!ParseDouble(bufX, x) || !ParseDouble(bufY, y))
            {
                SetWindowTextW(hResult, L"Помилка введення");
                break;
            }

            double d = x - 2.0;
            if (std::fabs(d) < 1e-12)
            {
                SetWindowTextW(hResult, L"Помилка: x = 2 (ділення на нуль)");
                break;
            }

            // [ ] - ціла частина (floor)
            long long r = static_cast<long long>(std::floor(y / d));
            SetWindowTextW(hResult, std::to_wstring(r).c_str());
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        TextOutW(hdc, 50, 30, L"Введіть x та y:", lstrlenW(L"Введіть x та y:"));
        TextOutW(hdc, 50, 63, L"x =", 3);
        TextOutW(hdc, 220, 63, L"y =", 3);
        TextOutW(hdc, 50, 168, L"Результат f(x, y):", lstrlenW(L"Результат f(x, y):"));
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}