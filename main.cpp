#define _USE_MATH_DEFINES
#include <windows.h>
#include <cstdio>
#include <math.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int ovalFindY(int x, int a, int b);
int circleFindY(int x, int a);
int OvalPrintText(HDC hdc, LPCTSTR str, RECT rect);
int CerclePrintText(HDC hdc, LPCTSTR str, RECT rect);
int RectanglePrintText(HDC hdc, LPCTSTR str, RECT rect);
void resizeFieldForOval(RECT *rect, RECT r);
void resizeFieldForCercl(RECT *rect, RECT r);
/*
 * figure = 1 --> Овал
 * figure = 2 --> Круг
 * figure = 3 --> прямоугольник
 * */
int figure = 1;
HDC hdc;
PAINTSTRUCT ps;
RECT field;
LOGFONT lf;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEX wcex; HWND hWnd; MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "HelloWorldClass";
    wcex.hIconSm = wcex.hIcon;
    RegisterClassEx(&wcex);

    hWnd = CreateWindow("HelloWorldClass", "Hello, World!", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    lf.lfHeight = 8;
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = FW_NORMAL;
    lf.lfItalic = 0;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfCharSet = ANSI_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = PROOF_QUALITY;
    lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
    strcpy_s(lf.lfFaceName, "Times New Roman");

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return  (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT rWnd;

    hdc = GetDC(hWnd);
    GetWindowRect(hWnd, &rWnd);
    switch(figure){
        case 1:
            resizeFieldForOval(&field, rWnd);
            break;
        case 2:
            resizeFieldForCercl(&field, rWnd);
            break;
        case 3:
            resizeFieldForOval(&field, rWnd);
            break;
    }
    RECT rect1;
    GetClientRect(hWnd, &rect1);

    switch (message)
    {
        case WM_PAINT:
            EndPaint(hWnd, &ps);
            switch(figure){
                case 1:
                    OvalPrintText(hdc, "If the function succeeds, the return value is nonzero.", field);
                    break;
                case 2:
                    CerclePrintText(hdc, "If the function succeeds, the return value is nonzero.", field);
                    break;
                case 3:
                    RectanglePrintText(hdc, "If the function succeeds, the return value is nonzero.", field);
                    break;
            }
            break;

        case WM_KEYDOWN:
            switch(wParam) {
                case VK_F1:
                    figure = 1;
                    break;
                case VK_F2:
                    figure = 2;
                    break;
                case VK_F3:
                    figure = 3;
                    break;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int OvalPrintText(HDC hdc, LPCTSTR str, RECT rect) {

    int h = (rect.right - rect.left) / strlen(str) * 2;
    int x0 = (rect.right + rect.left) / 2;
    int y0 = (rect.bottom + rect.top) / 2;
    int a = (rect.right - rect.left) / 2;
    int b = (rect.bottom - rect.top) / 2;
    int x = -a;
    for (int i = 0; i < static_cast<int>(strlen(str) / 2); i++) {
        int y = ovalFindY(x, a, b);
        TextOut(hdc, x0+x, y0-y, &str[i], 1);
        x += h;
    }

    for (int i = static_cast<int>(strlen(str) / 2); i < strlen(str); i++) {
        int y = ovalFindY(x, a, b);
        TextOut(hdc, x0-x, y0+y, &str[i], 1);
        x -= h;
    }
    return 0;
}

int CerclePrintText(HDC hdc, LPCTSTR str, RECT rect) {

    int h = (rect.right - rect.left) / strlen(str) * 2;
    int x0 = (rect.right + rect.left) / 2;
    int y0 = (rect.bottom + rect.top) / 2;
    int a = (rect.right - rect.left) / 2;
    int x = -a;
    for (int i = 0; i < static_cast<int>(strlen(str) / 2); i++) {
        int y = circleFindY(x, a);
        TextOut(hdc, x0+x, y0-y, &str[i], 1);
        x += h;
    }

    for (int i = static_cast<int>(strlen(str) / 2); i < strlen(str); i++) {
        int y = circleFindY(x, a);
        TextOut(hdc, x0-x, y0+y, &str[i], 1);
        x -= h;
    }
    return 0;
}


int RectanglePrintText(HDC hdc, LPCTSTR str, RECT rect) {
    int h1 = (rect.right - rect.left) / strlen(str) * 4;
    int h2 = (rect.bottom - rect.top) / strlen(str) * 4;

    int textLength = static_cast<int>(strlen(str) / 4);

    int x = rect.left;
    int y = rect.top;

    for (int i = 0; i < textLength; i++) {
        TextOut(hdc, x, y, &str[i], 1);
        x += h1;
    }

    for (int i = textLength; i < textLength*2; i++) {
        TextOut(hdc, x, y, &str[i], 1);
        y += h2;
    }

    x = rect.left;
    y = rect.top + h2;

    for (int i = textLength*2; i < textLength*3; i++) {
        TextOut(hdc, x, y, &str[i], 1);
        y += h2;
    }

    for (int i = textLength*3; i < strlen(str); i++) {
        TextOut(hdc, x, y, &str[i], 1);
        x += h1;
    }
    return 0;
}

int ovalFindY(int x, int a, int b) {
    return sqrt(pow(a, 2)*pow(b, 2) - pow(x, 2)*pow(b, 2)) / a;
}

int circleFindY(int x, int a) {
    return sqrt(pow(a, 2)*pow(a, 2) - pow(x, 2)*pow(a, 2)) / a;
}

void resizeFieldForOval(RECT *rect, RECT r) {
    rect->left = 10 + (r.right - r.left) / 20;
    rect->top = 10 + (r.bottom - r.top) / 20;
    rect->right = r.right - r.left - rect->left - 50;
    rect->bottom = r.bottom - r.top - rect->top - 50;
}

void resizeFieldForCercl(RECT *rect, RECT r) {
    int a = 100;
    int h = (r.bottom - r.top) - 2*a;
    rect->left = (r.right - r.left - h) / 2 ;
    rect->top = a;
    rect->right = rect->left + h;
    rect->bottom = r.top + h;
}