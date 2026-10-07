#include <windows.h>
#include <cmath>
#include <string>

const wchar_t CLASS_NAME[] = L"Lab2Variant18";

int clientWidth = 1000;
int clientHeight = 700;
int currentMode = 1;
int clickCount = 0;

double Function(double x)
{
    return x / ((x + 1) * (x + 1));
}

class Ship
{
public:
    void Draw(HDC dc, int x, int y)
    {
        POINT hull[] =
        {
            {x, y},
            {x + 260, y},
            {x + 220, y + 65},
            {x + 45, y + 65}
        };

        HBRUSH brush = CreateSolidBrush(RGB(130, 55, 30));
        HBRUSH oldBrush = (HBRUSH)SelectObject(dc, brush);

        HPEN pen = CreatePen(PS_SOLID, 2, RGB(50, 30, 20));
        HPEN oldPen = (HPEN)SelectObject(dc, pen);

        Polygon(dc, hull, 4);

        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);

        DeleteObject(brush);
        DeleteObject(pen);

        pen = CreatePen(PS_SOLID, 5, RGB(80, 50, 20));
        oldPen = (HPEN)SelectObject(dc, pen);

        MoveToEx(dc, x + 130, y, nullptr);
        LineTo(dc, x + 130, y - 170);

        SelectObject(dc, oldPen);
        DeleteObject(pen);

        POINT sail1[] =
        {
            {x + 120, y - 160},
            {x + 35, y - 20},
            {x + 120, y - 20}
        };

        brush = CreateSolidBrush(RGB(245, 245, 230));
        oldBrush = (HBRUSH)SelectObject(dc, brush);

        Polygon(dc, sail1, 3);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);

        POINT sail2[] =
        {
            {x + 140, y - 145},
            {x + 235, y - 20},
            {x + 140, y - 20}
        };

        brush = CreateSolidBrush(RGB(220, 230, 240));
        oldBrush = (HBRUSH)SelectObject(dc, brush);

        Polygon(dc, sail2, 3);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

class Bird
{
public:
    void Draw(HDC dc, int x, int y)
    {
        HPEN pen = CreatePen(PS_SOLID, 4, RGB(40, 40, 40));
        HPEN oldPen = (HPEN)SelectObject(dc, pen);

        MoveToEx(dc, x, y + 20, nullptr);
        LineTo(dc, x + 25, y);
        LineTo(dc, x + 50, y + 20);

        SelectObject(dc, oldPen);
        DeleteObject(pen);
    }
};

class Cloud
{
public:
    void Draw(HDC dc, int x, int y)
    {
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(dc, brush);

        Ellipse(dc, x, y + 20, x + 75, y + 70);
        Ellipse(dc, x + 35, y, x + 115, y + 70);
        Ellipse(dc, x + 75, y + 20, x + 150, y + 70);

        Rectangle(dc, x + 35, y + 35, x + 115, y + 70);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

class Sun
{
public:
    void Draw(HDC dc, int x, int y)
    {
        HBRUSH brush = CreateSolidBrush(RGB(255, 220, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(dc, brush);

        HPEN pen = CreatePen(PS_SOLID, 2, RGB(240, 160, 0));
        HPEN oldPen = (HPEN)SelectObject(dc, pen);

        Ellipse(dc, x, y, x + 100, y + 100);

        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);

        DeleteObject(brush);
        DeleteObject(pen);
    }
};

Ship ship;
Bird bird;
Cloud cloud;
Sun sun;

void DrawComposition(HDC dc)
{
    RECT r = { 0, 0, clientWidth, clientHeight };

    HBRUSH sky = CreateSolidBrush(RGB(135, 206, 235));
    FillRect(dc, &r, sky);
    DeleteObject(sky);

    sun.Draw(dc, clientWidth - 160, 50);

    cloud.Draw(dc, 100, 80);
    cloud.Draw(dc, 370, 120);
    cloud.Draw(dc, 600, 65);

    bird.Draw(dc, 300, 220);
    bird.Draw(dc, 390, 180);
    bird.Draw(dc, 470, 230);

    RECT sea =
    {
        0,
        clientHeight / 2,
        clientWidth,
        clientHeight
    };

    HBRUSH water = CreateSolidBrush(RGB(30, 130, 210));
    FillRect(dc, &sea, water);
    DeleteObject(water);

    HPEN wavePen = CreatePen(PS_SOLID, 2, RGB(180, 230, 255));
    HPEN oldPen = (HPEN)SelectObject(dc, wavePen);

    for (int y = clientHeight / 2 + 30;
        y < clientHeight - 20; y += 45)
    {
        for (int x = 20; x < clientWidth - 40; x += 100)
        {
            MoveToEx(dc, x, y, nullptr);
            LineTo(dc, x + 45, y);
        }
    }

    SelectObject(dc, oldPen);
    DeleteObject(wavePen);

    ship.Draw(dc, clientWidth / 2 - 130,
        clientHeight / 2 + 100);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(0, 0, 0));

    const wchar_t* title = L"Лабораторна робота №2. Варіант 18";
    TextOutW(dc, 20, 20, title, lstrlenW(title));

    const wchar_t* hint = L"Клавіша 1 - композиція; клавіша 2 - графік";
    TextOutW(dc, 20, 45, hint, lstrlenW(hint));

    wchar_t info[100];
    wsprintfW(info, L"Кількість натискань: %d", clickCount);
    TextOutW(dc, 20, 70, info, lstrlenW(info));
}

void DrawGraph(HDC dc)
{
    RECT r = { 0, 0, clientWidth, clientHeight };

    HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(dc, &r, white);

    int left = 100;
    int right = clientWidth - 70;
    int top = 100;
    int bottom = clientHeight - 100;

    int graphWidth = right - left;
    int graphHeight = bottom - top;

    auto PixelX = [&](double x)
        {
            return left + (int)(x / 4.0 * graphWidth);
        };

    auto PixelY = [&](double y)
        {
            return bottom - (int)(y / 0.30 * graphHeight);
        };

    HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(dc, axisPen);

    MoveToEx(dc, left, top, nullptr);
    LineTo(dc, left, bottom);
    LineTo(dc, right, bottom);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(0, 0, 0));

    for (int i = 0; i <= 4; i++)
    {
        int x = PixelX(i);

        MoveToEx(dc, x, bottom - 5, nullptr);
        LineTo(dc, x, bottom + 5);

        wchar_t label[20];
        wsprintfW(label, L"%d", i);

        TextOutW(dc, x - 5, bottom + 12, label, lstrlenW(label));
    }

    for (int i = 0; i <= 6; i++)
    {
        double y = i * 0.05;
        int yy = PixelY(y);

        MoveToEx(dc, left - 5, yy, nullptr);
        LineTo(dc, left + 5, yy);

        wchar_t label[20];
        swprintf_s(label, L"%.2f", y);

        TextOutW(dc, left - 55, yy - 8, label, lstrlenW(label));
    }

    TextOutW(dc, right + 10, bottom, L"X", 1);
    TextOutW(dc, left - 25, top - 20, L"Y", 1);

    const wchar_t* title = L"y = x / (x + 1)^2";
    TextOutW(dc, left + 150, 25, title, lstrlenW(title));

    HPEN graphPen = CreatePen(PS_SOLID, 3, RGB(0, 80, 220));
    SelectObject(dc, graphPen);

    bool firstPoint = true;

    for (double x = 0.0; x <= 4.001; x += 0.01)
    {
        double y = Function(x);

        int xx = PixelX(x);
        int yy = PixelY(y);

        if (firstPoint)
        {
            MoveToEx(dc, xx, yy, nullptr);
            firstPoint = false;
        }
        else
        {
            LineTo(dc, xx, yy);
        }
    }

    SelectObject(dc, oldPen);

    DeleteObject(graphPen);
    DeleteObject(axisPen);

    TextOutW(dc, 20, clientHeight - 35,
        L"Натисніть 1 для композиції", 30);

    TextOutW(dc, 300, clientHeight - 35,
        L"Натисніть 2 для графіка", 27);
}

LRESULT CALLBACK WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hwnd, &ps);

        if (currentMode == 1)
        {
            DrawComposition(dc);
        }
        else
        {
            DrawGraph(dc);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SIZE:
    {
        clientWidth = LOWORD(lParam);
        clientHeight = HIWORD(lParam);

        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        clickCount++;

        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == '1')
        {
            currentMode = 1;
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        else if (wParam == '2')
        {
            currentMode = 2;
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        else if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }

        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    WNDCLASSW wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Лабораторна робота №2 - Варіант 18",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1000,
        700,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
