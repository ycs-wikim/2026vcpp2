// 0921.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0921.h"

#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0921, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0921));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0921));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0921);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

/// 나의 객체, 적의 객체, 음식 객체 선언
RECT g_me, g_enemy, g_food;

void userInputProcess(WPARAM wParam)
{
    /// 사용자의 방향키 입력에 따른 좌표 변경
    switch (wParam)
    {
        /*
    case VK_F1:
    {
        /// 동적 함수 매핑
    }
        break;
        */

    case VK_LEFT:       /// 왼쪽 키를 누른 경우
    {
        g_me.left -= 10;
        g_me.right -= 10;
    }
    break;
    case VK_RIGHT:      /// 오른쪽 키를 누른 경우
    {
        g_me.left += 10;
        g_me.right += 10;
    }
    break;
    case VK_UP:         /// 위쪽 키를 누른 경우
    {
        g_me.top -= 10;
        g_me.bottom -= 10;
    }
    break;
    case VK_DOWN:       /// 아래쪽 키를 누른 경우
    {
        g_me.top += 10;
        g_me.bottom += 10;
    }
    break;
    }       /// switch( wParam ) 종료
    /// 함수가 종료되는 시점에 내 캐릭터의 좌표가 이동된 상태!
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        if (1 == wParam)    /// SetTimer에서 설정한 ID 값 확인
        {
            /// 적의 이동을 수행
            if (g_me.left < g_enemy.left)   /// 30 < 170
            {
                g_enemy.left -= 10;
                g_enemy.right -= 10;
            }
            else
            {
                g_enemy.left += 10;
                g_enemy.right += 10;
            }

            if (g_me.top < g_enemy.top)      /// 30 < 170
            {
                g_enemy.top -= 10;
                g_enemy.bottom -= 10;
            }
            else
            {
                g_enemy.top += 10;
                g_enemy.bottom += 10;
            }

            /// 적이 나를 겹침했는지에 대한 확인
            RECT abc;
            /*
            if (IntersectRect(&abc, &g_me, &g_enemy))
            {
                /// 타이머를 정지!
                KillTimer(hWnd, 1);

                /// 내가 적에게 붙들린 상황 : 잡혔다~
                MessageBox(hWnd, L"Game Over", L"잡혔다", MB_OK);
            }
            */
            /// 적의 이동이 완료되었으니 화면을 다시 그려라~
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
        break;

    case WM_CREATE:
    {
        /// 랜덤 시드 값을 현재 시간으로 초기화
        srand(time(NULL));

        /// 주기적인 호출을 요청
        /// hWnd : 주기적인 호출을 받을 대상
        /// 1 : 아이디 값. WM_TIMER에 도착 했을때 구분자로 사용
        /// 1000 : 1초. msec 단위로 시간을 설정
        /// NULL : 특정 함수를 지정할 수 있으나, 여기서는 사용하지 않음
        SetTimer(hWnd, 1, 100, NULL);

        /// 나의 좌표 초기화
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = 60;
        g_me.bottom = 60;

        /// 상대의 좌표 초기화
        g_enemy.left = 500;
        g_enemy.top = 500;
        g_enemy.right = g_enemy.left + 50;
        g_enemy.bottom = g_enemy.top + 50;

        /// 음식의 좌표 초기화
        g_food.left = 250;
        g_food.top = 100;
        g_food.right = g_food.left + 50;
        g_food.bottom = g_food.top + 50;
    }
        break;

    case WM_KEYDOWN:
    {
        /// 사용자의 방향키 입력을 처리할 함수를 작성
        userInputProcess(wParam);

        /// 사용자의 입력키가 F5 키라면
        if (wParam == VK_F5)
        {
            /// 게임을 다시 시작할 수 있도록 타이머 세팅
            SetTimer(hWnd, 1, 100, NULL);
        }

        /// 음식을 먹었는지 확인!
        /// IntersectRect( ) API
        /// 1. Dst : 겹침이 발생한 좌표 값을 반환
        /// 2, 3. Src : 겹침을 확인할 RECT 자료형
        /// 반환 값이 참이면 겹쳤다. 거짓이면 안 겹쳤다.
        RECT ir;
        if (IntersectRect(&ir, &g_me, &g_food))
        {
            /// 내가 음식을 먹은 것이 확인된 시점 -> 음식을 다른 위치로 변경
            g_food.left = rand() % 400;
            g_food.top = rand() % 400;
            g_food.right = g_food.left + 50;
            g_food.bottom = g_food.top + 50;

            /*  /// 실제로 겹침이 발생한 좌표에 사각형을 그려서 확인
            HDC hdc = GetDC(hWnd);

            Rectangle(hdc, ir.left, ir.top, ir.right, ir.bottom);

            ReleaseDC(hWnd, hdc);
            */
            ///MessageBox(hWnd, L"겹쳤다!", L"내가 해내써", MB_OK);
        }
        
        /// OS에게 화면을 다시 그려달라~ 라고 요청
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            /// 나의 좌표 값을 이용하여 내 캐릭터를 그리는 코드
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            /// 적의 좌표 값을 이용하여 적 캐릭터를 그리는 코드
            Rectangle(hdc, g_enemy.left, g_enemy.top, g_enemy.right, g_enemy.bottom);
            /// 음식의 좌표 값을 이용하여 음식 그리는 코드
            Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
