// 0928.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0928.h"

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
    LoadStringW(hInstance, IDC_MY0928, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0928));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0928));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0928);
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

/// 자료구조 선언 : 나, 적, 음식의 객체 선언
RECT g_me, g_you, g_food;       /// 겹침 확인이 편리
/// 사각형의 크기를 상수로 선언
#define RECT_SIZE       50
/// 점수를 보관하고 있을 변수를 선언
int g_score = 0;
/// 현재 게임의 상태를 보관하는 변수 선언
int g_in_game = 1;          /// 1 - 게임 중, 0 - 게임 종료 상태
/// 상대의 속도를 제어하기 위한 변수 선언
int g_speed = 1000;
/// 게임 시간을 제어하기 위한 변수 선언
int g_gametime = 10;
/// 나의 게임 라이프를 보관할 변수 선언
int g_life = 3;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        if (1 == wParam)
        {
            if (g_me.left < g_you.left)
            {
                /// 참이라면 나는 상대의 왼쪽에 위치
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else
            {
                /// 거짓이라면 나는 상대의 오른쪽에 위치
                g_you.left += 10;
                g_you.right += 10;
            }

            if (g_me.top < g_you.top)
            {
                /// 참이라면 나는 상대의 위쪽에 위치
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                /// 거짓이라면 나는 상대의 아래쪽에 위치
                g_you.top += 10;
                g_you.bottom += 10;
            }
            /// 상대의 좌표 값 이동이 완료된 상태

            /// 상대가 나를 잡았는지 여부를 판단
            RECT ir;
            if (IntersectRect(&ir, &g_me, &g_you))
            {
                /// 라이프를 감소시킨다.
                g_life--;
                if (0 == g_life)
                {
                    /// 참이라면 상대가 나를 잡은 상태 -> 상대를 멈춤
                    KillTimer(hWnd, 1);
                    KillTimer(hWnd, 2);
                    /// 게임의 상태도 변경
                    g_in_game = 0;
                }
                /// 상대방을 초기 위치로 변경
                g_you.left = 400;
                g_you.top = 400;
                g_you.right = g_you.left + RECT_SIZE;
                g_you.bottom = g_you.top + RECT_SIZE;
            }

            /// 화면을 다시 그려줘야 한다.
            InvalidateRect(hWnd, NULL, TRUE);
        }       /// Timer ID가 1인 경우의 처리
        else if (2 == wParam)
        {
            /// 게임의 시간을 조절
            g_gametime--;
            if (0 == g_gametime)
            {
                /// 게임 종료 조건
                g_in_game = 0;      /// 사용자 입력을 막는다.
                KillTimer(hWnd, 1); /// 상대의 움직임을 막는다.
                KillTimer(hWnd, 2); /// 게임 시간 타이머를 막는다.
                /// 화면을 다시 그려준다.
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }


    }
        break;

    /// 생성자와 동일하게 프로그램 시작 시 단 한번만 호출된다.
    case WM_CREATE:
    {
        /// 1. 랜덤 시드 값 변경
        srand(time(NULL));

        /// 2. 타이머 설정 - WM_TIMER가 지정 시간마다 호출
        ///  2.1 상대의 움직임
        SetTimer(hWnd, 1, g_speed, NULL);
        ///  2.2 게임 시간
        SetTimer(hWnd, 2, 1000, NULL);

        g_me.left = 10;
        g_me.top = 10;
        g_me.right = g_me.left + RECT_SIZE;
        g_me.bottom = g_me.top + RECT_SIZE;

        g_you.left = 400;
        g_you.top = 400;
        g_you.right = g_you.left + RECT_SIZE;
        g_you.bottom = g_you.top + RECT_SIZE;

        g_food.left = 150;
        g_food.top = 300;
        g_food.right = g_food.left + RECT_SIZE;
        g_food.bottom = g_food.top + RECT_SIZE;
    }
        break;

    case WM_KEYDOWN:
    {
        /// 게임 중인지 여부를 확인
        if (0 == g_in_game)
            break;

        switch (wParam)
        {
        case VK_LEFT:
        {
            g_me.left -= 10;
            g_me.right -= 10;
        }
            break;
        case VK_RIGHT:
        {
            g_me.left += 10;
            g_me.right += 10;
        }
            break;
        case VK_UP:
        {
            g_me.top -= 10;
            g_me.bottom -= 10;
        }
            break;
        case VK_DOWN:
        {
            g_me.top += 10;
            g_me.bottom += 10;
        }
            break;
        }   /// switch( wParam )
        /// 사용자의 키 입력에 따라 나의 객체 이동이 완료된 상태

        /// 내가 음식을 먹었는지 확인한다!
        RECT ir;
        if (IntersectRect(&ir, &g_me, &g_food))
        {
            /// 음식을 먹은 상태
            g_food.left = rand() % 400;
            g_food.top = rand() % 400;
            g_food.right = g_food.left + RECT_SIZE;
            g_food.bottom = g_food.top + RECT_SIZE;
            /// 점수 증가
            g_score += 100;
            /// 게임 시간을 변경
            g_gametime += 3;
            /// 상대의 속도를 변경
            KillTimer(hWnd, 1);
            /// 상대 속도를 줄인다.
            g_speed -= 100;
            /// 상대의 최대 속도를 지정
            if (g_speed <= 100) /// 0.1초보다 속도가 빨라졌는지 확인
            {
                /// 속도를 고정
                g_speed = 100;
            }

            SetTimer(hWnd, 1, g_speed, NULL);
        }

        /// 좌표값만! 변경되었기 때문에 화면을 다시 그려야 한다!
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
            /// 화면에 문자열을 출력하기 위한 버퍼
            WCHAR buf[128] = { 0, };
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            /// 0. 그라운드 출력
            Rectangle(hdc, 10, 10, 500, 500);

            HBRUSH myBrush, osBrush;
            /// 1. 나를 그린다.
            myBrush = CreateSolidBrush(RGB(0, 100, 250));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            TextOut(hdc, g_me.left + 20, g_me.top + 20, L"ME", 2);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);
            /// 2. 적을 그린다.
            myBrush = CreateSolidBrush(RGB(200, 100, 100));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);
            TextOut(hdc, g_you.left + 10, g_you.top + 20, L"YOU", 3);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);
            /// 3. 음식을 그린다.
            myBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);
            /// 4. 점수를 표시한다.
            /// buf에 printf( ) 형식의 출력 내용을 전달하는 함수
            wsprintf(buf, L"현재 점수는요 : %d", g_score);
            /// 화면에 직접 문자열을 출력하는 API
            TextOut(hdc, 600, 100, buf, lstrlen(buf));
            /// 5. 게임의 진행 중 여부를 출력
            if (1 == g_in_game)
            {
                /// 게임 중인 상태
                TextOut(hdc, 600, 20, L"게임 중", 4);
            }
            else
            {
                /// 게임이 종료된 상태
                TextOut(hdc, 600, 20, L"게임 오바", 5);
            }
            /// 6. 게임의 시간 정보 출력
            wsprintf(buf, L"남은 게임 시간 : %d", g_gametime);
            TextOut(hdc, 600, 40, buf, lstrlen(buf));
            Rectangle(hdc, 600, 60, 600 + (g_gametime * 20), 80);
            /// 7. 게임의 라이프를 출력
            wsprintf(buf, L"Life: %d", g_life);
            TextOut(hdc, 600, 80, buf, lstrlen(buf));
            

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
