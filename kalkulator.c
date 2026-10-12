#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ID_DISPLAY     100
#define ID_BTN_CLR     14
#define ID_BTN_EQ      15
#define ID_BTN_SIN     20
#define ID_BTN_COS     21
#define ID_BTN_TAN     22
#define ID_BTN_POW     23
#define ID_BTN_SQRT    24

HWND hDisplay;
char buffer[100] = "";
double angka1 = 0, angka2 = 0;
int op = 0;
BOOL statusBaru = TRUE;

HBRUSH hBrushPinkBg;
HBRUSH hBrushPinkDisplay;
HBRUSH hBrushPinkBtn;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            hBrushPinkDisplay = CreateSolidBrush(RGB(255, 182, 193));
            hBrushPinkBtn = CreateSolidBrush(RGB(255, 158, 181));

            hDisplay = CreateWindow("EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
                                    10, 10, 310, 35, hwnd, (HMENU)ID_DISPLAY, NULL, NULL);

            HFONT hFont = CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
                                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
                                     DEFAULT_PITCH | FF_SWISS, "Arial");
            SendMessage(hDisplay, WM_SETFONT, (WPARAM)hFont, TRUE);

            CreateWindow("BUTTON", "Sin", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 10, 55, 55, 40, hwnd, (HMENU)ID_BTN_SIN, NULL, NULL);
            CreateWindow("BUTTON", "Cos", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 75, 55, 55, 40, hwnd, (HMENU)ID_BTN_COS, NULL, NULL);
            CreateWindow("BUTTON", "Tan", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 140, 55, 55, 40, hwnd, (HMENU)ID_BTN_TAN, NULL, NULL);
            CreateWindow("BUTTON", "x^y", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 205, 55, 55, 40, hwnd, (HMENU)ID_BTN_POW, NULL, NULL);
            CreateWindow("BUTTON", "Sqrt", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 270, 55, 50, 40, hwnd, (HMENU)ID_BTN_SQRT, NULL, NULL);

            CreateWindow("BUTTON", "7", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 10, 105, 55, 40, hwnd, (HMENU)7, NULL, NULL);
            CreateWindow("BUTTON", "8", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 75, 105, 55, 40, hwnd, (HMENU)8, NULL, NULL);
            CreateWindow("BUTTON", "9", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 140, 105, 55, 40, hwnd, (HMENU)9, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 205, 105, 55, 40, hwnd, (HMENU)13, NULL, NULL);
            CreateWindow("BUTTON", "C", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 270, 105, 50, 90, hwnd, (HMENU)ID_BTN_CLR, NULL, NULL);

            CreateWindow("BUTTON", "4", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 10, 155, 55, 40, hwnd, (HMENU)4, NULL, NULL);
            CreateWindow("BUTTON", "5", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 75, 155, 55, 40, hwnd, (HMENU)5, NULL, NULL);
            CreateWindow("BUTTON", "6", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 140, 155, 55, 40, hwnd, (HMENU)6, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 205, 155, 55, 40, hwnd, (HMENU)12, NULL, NULL);

            CreateWindow("BUTTON", "1", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 10, 205, 55, 40, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "2", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 75, 205, 55, 40, hwnd, (HMENU)2, NULL, NULL);
            CreateWindow("BUTTON", "3", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 140, 205, 55, 40, hwnd, (HMENU)3, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 205, 205, 55, 40, hwnd, (HMENU)11, NULL, NULL);
            CreateWindow("BUTTON", "=", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 270, 205, 50, 90, hwnd, (HMENU)ID_BTN_EQ, NULL, NULL);

            CreateWindow("BUTTON", "0", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 10, 255, 120, 40, hwnd, (HMENU)0, NULL, NULL);
            CreateWindow("BUTTON", ".", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 140, 255, 55, 40, hwnd, (HMENU)16, NULL, NULL);
            CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 205, 255, 55, 40, hwnd, (HMENU)10, NULL, NULL);
            break;

        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wp;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(255, 182, 193));
            return (INT_PTR)hBrushPinkDisplay;
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lp;
            if (pDIS->CtlType == ODT_BUTTON) {
                FillRect(pDIS->hDC, &pDIS->rcItem, hBrushPinkBtn);
                FrameRect(pDIS->hDC, &pDIS->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));
                char btnText[20];
                GetWindowText(pDIS->hwndItem, btnText, sizeof(btnText));
                SetTextColor(pDIS->hDC, RGB(255, 255, 255));
                SetBkMode(pDIS->hDC, TRANSPARENT);
                DrawText(pDIS->hDC, btnText, -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                return TRUE;
            }
            break;
        }

        case WM_COMMAND: {
            int id = LOWORD(wp);
            if (id >= 0 && id <= 9) {
                if (statusBaru) {
                    strcpy(buffer, "");
                    statusBaru = FALSE;
                }
                char temp[10];
                sprintf(temp, "%d", id);
                strcat(buffer, temp);
                SetWindowText(hDisplay, buffer);
            } 
            else if (id == 16) { 
                if (statusBaru) {
                    strcpy(buffer, "0.");
                    statusBaru = FALSE;
                } else if (strchr(buffer, '.') == NULL) {
                    strcat(buffer, ".");
                }
                SetWindowText(hDisplay, buffer);
            } 
            else if ((id >= 10 && id <= 13) || id == ID_BTN_POW) {
                angka1 = atof(buffer);
                op = id;
                statusBaru = TRUE;
            } 
            else if (id == ID_BTN_SIN || id == ID_BTN_COS || id == ID_BTN_TAN || id == ID_BTN_SQRT) {
                double val = atof(buffer);
                double hasil = 0;
                if (id == ID_BTN_SIN) hasil = sin(val * 3.14159265 / 180.0);
                else if (id == ID_BTN_COS) hasil = cos(val * 3.14159265 / 180.0);
                else if (id == ID_BTN_TAN) hasil = tan(val * 3.14159265 / 180.0);
                else if (id == ID_BTN_SQRT) {
                    if (val >= 0) hasil = sqrt(val);
                    else {
                        SetWindowText(hDisplay, "Error: Negatif Akar");
                        statusBaru = TRUE;
                        break;
                    }
                }
                sprintf(buffer, "%g", hasil);
                SetWindowText(hDisplay, buffer);
                statusBaru = TRUE;
            } 
            else if (id == ID_BTN_EQ) {
                angka2 = atof(buffer);
                double hasil = 0;
                BOOL isError = FALSE;
                switch (op) {
                    case 10: hasil = angka1 + angka2; break;
                    case 11: hasil = angka1 - angka2; break;
                    case 12: hasil = angka1 * angka2; break;
                    case 13: 
                        if (angka2 != 0) hasil = angka1 / angka2;
                        else isError = TRUE; 
                        break;
                    case ID_BTN_POW: hasil = pow(angka1, angka2); break;
                }
                if (isError) {
                    SetWindowText(hDisplay, "Error: Div by 0");
                    strcpy(buffer, ""); 
                } else {
                    sprintf(buffer, "%g", hasil);
                    SetWindowText(hDisplay, buffer);
                }
                statusBaru = TRUE;
            } 
            else if (id == ID_BTN_CLR) {
                strcpy(buffer, ""); 
                angka1 = 0;
                angka2 = 0;
                op = 0;
                SetWindowText(hDisplay, "0");
                statusBaru = TRUE;
            }
            break;
        }

        case WM_DESTROY:
            DeleteObject(hBrushPinkBg);
            DeleteObject(hBrushPinkDisplay);
            DeleteObject(hBrushPinkBtn);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    hBrushPinkBg = CreateSolidBrush(RGB(255, 218, 224));
    
    WNDCLASS wc = {0};
    wc.hbrBackground = hBrushPinkBg;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "KalkulatorScientificWin32";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) return -1;

    HWND hwndMain = CreateWindow("KalkulatorScientificWin32", "Scientific Calculator NAFISAH", 
                                 (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), 
                                 CW_USEDEFAULT, CW_USEDEFAULT, 345, 350, 
                                 NULL, NULL, hInst, NULL);

    ShowWindow(hwndMain, ncmdshow);
    UpdateWindow(hwndMain);

    // Message Loop dasar
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}