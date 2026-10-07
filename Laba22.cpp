#include "framework.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

// ---------------- Стан програми ----------------
static bool     imageVisible = false;      // чи показано малюнок
static double   scaleFactor = 1.0;        // поточний масштаб (зменшення)
static int      colorIndex = 0;          // індекс кольору стін
static const COLORREF palette[] = {
    RGB(255, 200, 100), RGB(255, 0, 0), RGB(0, 200, 0), RGB(80, 120, 255), RGB(180, 0, 180)
};
static const int paletteSize = sizeof(palette) / sizeof(palette[0]);

static const int    ANCHOR_X = 100, ANCHOR_Y = 100;   // точка, відносно якої зменшуємо
static const double SHRINK_STEP = 0.8;                // кожне натискання: -20 %
static const double MIN_SCALE = 0.1;

// Перерахунок координати з урахуванням масштабу
static int SX(int x) { return ANCHOR_X + (int)((x - ANCHOR_X) * scaleFactor); }
static int SY(int y) { return ANCHOR_Y + (int)((y - ANCHOR_Y) * scaleFactor); }

// ---------------- Малювання зображення (будинок + сонце) ----------------
void DrawImage(HDC hdc)
{
    if (!imageVisible) return;

    HPEN   pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HPEN   oldPen = (HPEN)SelectObject(hdc, pen);

    // Стіни
    HBRUSH wallBrush = CreateSolidBrush(palette[colorIndex]);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, wallBrush);
    Rectangle(hdc, SX(150), SY(250), SX(350), SY(400));

    // Дах
    HBRUSH roofBrush = CreateSolidBrush(RGB(150, 60, 30));
    SelectObject(hdc, roofBrush);
    POINT roof[3] = { { SX(130), SY(250) }, { SX(250), SY(150) }, { SX(370), SY(250) } };
    Polygon(hdc, roof, 3);

    // Двері
    HBRUSH doorBrush = CreateSolidBrush(RGB(100, 70, 40));
    SelectObject(hdc, doorBrush);
    Rectangle(hdc, SX(225), SY(320), SX(275), SY(400));

    // Сонце
    HBRUSH sunBrush = CreateSolidBrush(RGB(255, 230, 0));
    SelectObject(hdc, sunBrush);
    Ellipse(hdc, SX(420), SY(130), SX(500), SY(210));

    // Повернення старих об'єктів та видалення створених
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    DeleteObject(wallBrush);
    DeleteObject(roofBrush);
    DeleteObject(doorBrush);
    DeleteObject(sunBrush);
}

// ---------------- Стандартна частина шаблону ----------------
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABA22, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA22));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA22));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LABA22);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

// ---------------- Віконна процедура ----------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            // --- Зображення -> Рисунок... ---
        case ID_IMAGE_DRAW:
            imageVisible = true;
            scaleFactor = 1.0;          // повертаємо початковий розмір
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

            // --- Трансформації -> Зміна кольору ---
        case ID_TRANSFORM_COLOR:
            colorIndex = (colorIndex + 1) % paletteSize;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

            // --- Трансформації -> Зменшення (індивідуальна дія, варіант 18) ---
        case ID_TRANSFORM_SHRINK:
            if (!imageVisible)
            {
                MessageBoxW(hWnd, L"Спочатку виведіть малюнок (Зображення -> Рисунок...).",
                    L"Увага", MB_OK | MB_ICONINFORMATION);
            }
            else if (scaleFactor * SHRINK_STEP < MIN_SCALE)
            {
                MessageBoxW(hWnd, L"Досягнуто мінімального розміру.",
                    L"Увага", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                scaleFactor *= SHRINK_STEP;
                InvalidateRect(hWnd, nullptr, TRUE);
            }
            break;

            // --- Інформація -> Про програму / Вихід ---
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawImage(hdc);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ---------------- Діалог "Про програму" ----------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

