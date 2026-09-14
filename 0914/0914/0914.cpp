// 0914.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0914.h"

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
    LoadStringW(hInstance, IDC_MY0914, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0914));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0914));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0914);
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

/// 간단한 그림판 만들기

/// 이전 좌표(WM_LBUTTONDOWN) 보관 전역 변수
int g_x, g_y;
/// 그리기 메뉴를 선택한 정보를 보관할 전역 플래그 변수를 선언
int g_draw = 0;     /// 0-그리지않음, 1-선, 2-사각형, 3-타원, 4-자유선
/// 선의 굵기 메뉴의 선택 정보를 보관할 전역 플래그 변수 선언
int g_line_px;

/// 내 객체의 위치 좌표를 보관할 RECT 자료형
RECT g_me;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    /// 추가 윈도우 메시지
    case WM_CREATE:
    {
        /// WM_CREATE는 클래스의 생성자와 동일한 역할 수행
        /// 생성자의 동작 : 클래스가 인스턴스화 될때, 자동으로 불리는 함수
        ///     생성자에서는 주로 초기화와 동적 메모리 할당 등의 작업 수행
        /// 좌상단의 x, y 좌표 값 설정
        g_me.left = 10;     /// 좌상단의 x 좌표
        g_me.top = 10;      /// 좌상단의 y 좌표
        g_me.right = 160;    /// 우하단의 x 좌표
        g_me.bottom = 160;   /// 우하단의 y 좌표
    }
        break;

    /// 내 객체의 위치를 변경할 윈도우 메시지
    case WM_KEYDOWN:
    {
        /// 내부에서 좌표 값만을 수정한 상태 => 다시 그려라~가 없는 상태
        if (wParam == VK_RIGHT)
        {
            g_me.left += 10;
        }
    }
        break;

    case WM_LBUTTONDOWN:
    {
        /// 마우스가 눌린 시점의 좌표 값을 획득
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
    {
        /// 마우스의 왼쪽 버튼을 떼는 시점
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        /// 마우스를 누른 좌표 위치로 현재 위치에서 선을 그린다!
        HDC hdc = GetDC(hWnd);

        /// 선의 굵기를 지정하는 위치
        HPEN myPen, osPen;
        myPen = CreatePen(PS_SOLID, g_line_px, RGB(0, 0, 0));
        osPen = (HPEN)SelectObject(hdc, myPen);

        /// 그리기 종류에 따른 그리기 실행
        if (1 == g_draw)
        {
            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);

            MoveToEx(hdc, x, g_y, NULL);
            LineTo(hdc, g_x, y);
        }
        else if (2 == g_draw)
        {
            Rectangle(hdc, g_x, g_y, x, y);
        }
        else if (3 == g_draw)
        {
            Ellipse(hdc, g_x, g_y, x, y);
        }
        
        SelectObject(hdc, osPen);
        DeleteObject(myPen);

        ReleaseDC(hWnd, hdc);
    }
        break;

    /// 메뉴를 클릭했을때 전달되는 윈도우 메시지
    case WM_COMMAND:
        {
            /// wmId 값에 우리가 설정한 ID_DRAW_LINE의 값이 전달
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            /// visual studio 버그로 오류로 보이지만 실행된다.
            /// 선 그리기 메뉴를 선택한 경우의 처리 시작
            /// 선 그리기의 종류이기 때문에 별도로 설정
            case ID_DRAW_LINE:
                /// 메뉴 선택에 대한 정보만! 보관
                g_draw = 1;     /// 플래그 변수 설정
                //MessageBox(hWnd, L"선 그리기!", L"메뉴 선택", MB_OK);
                break;
            case ID_DRAW_RECTANGLE:
                g_draw = 2;
                break;
            case ID_DRAW_ELLIPSE:
                g_draw = 3;
                break;
            case ID_DRAW_FREELINE:
                g_draw = 4;
                break;
            /// 선 그리기 메뉴를 선택한 경우의 처리 끝

            /// 선 굵기 메뉴를 선택한 경우의 처리 시작
            /// 선 굵기의 실제 크기 값!을 보관
            case ID_LINE_1PX:
                g_line_px = 1;
                break;
            case ID_LINE_5PX:
                g_line_px = 5;
                break;
            case ID_LINE_10PX:
                g_line_px = 10;
                break;
            /// 선 굵기 메뉴를 선택한 경우의 처리 끝

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

            /// 나의 사각형을 그리자~
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        /// 클래스의 소멸자와 같은 역할을 담당하는 윈도우 메시지
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
