// 0997.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0997.h"

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
    LoadStringW(hInstance, IDC_MY0997, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0997));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0997));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0997);
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


/// 이전 마우스의 위치 값을 보관하기 위한 변수들을 선언
int px = 0, py = 0;

/// 플래그 변수
/// 프로그래밍에서 특정 상태나 조건이 만족되었는지를 기록하고 판별하기 위해 사용하는 변수
/// BOOL 자료형을 int로 사용하는 이유 : 패딩(Padding)
int g_flag = 0;     /// 1이면 마우스를 누른 상태, 0이면 누르지 않은 상태

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 마우스 오른쪽 클릭 시에 면의 색상을 변경하는 코드와 주석
    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);

        /// 면 색상 변경을 위한 객체 선언
        HBRUSH myBrush, osBrush;

        /// 1. 브러시를 생성한다.
        myBrush = CreateSolidBrush(RGB(120, 180, 110));
        /// 2. 교체
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);
        /// 3. 그리기
        Rectangle(hdc, 10, 10, x, y);
        /// 4. 교체 -> 복원
        SelectObject(hdc, osBrush);
        /// 5. 해제
        DeleteObject(myBrush);

        ReleaseDC(hWnd, hdc);
    }
        break;

    /// 마우스 왼쪽 클릭 시에 선의 색상을 변경하는 코드와 주석 예제
    case WM_LBUTTONDOWN:
    {
        /// 마우스 왼쪽 버튼을 눌렀다! 라는 플래그를 설정
        g_flag = 1;
        px = LOWORD(lParam);
        py = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
    {
        /// 마우스 왼쪽 버튼을 뗐다! 라는 플래그를 설정
        g_flag = 0;

    }
        break;

    case WM_MOUSEMOVE:
    {
        /// 마우스가 눌린 상태가 아니라면 할 일 없음
        if (0 == g_flag)
            break;
        /// 마우스의 x, y 좌표 획득
        /// A, B, C, D, ... , X, Y, Z
        /// 1, 2, 3, 4, ... , 24, 25, 26
        /// 24 < 25 작다. 즉, x가 y보다 순서 값이 작다 --> LOW
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        /// 화면에 그리기를 수행하기 위해 반드시 필요한 핸들
        /// 요청에 의해 핸들이 생성되면! -> OS 내부에 동적으로 생성
        HDC hdc = GetDC(hWnd);

        /// 선 색상 변경을 위한 펜을 생성
        /// PS_SOLID : 실선
        /// 8 : 선의 두께
        /// RGB(0~255, 0~255, 0~255) : 색상 조합
        HPEN myPen = CreatePen(PS_SOLID, 8, RGB(0, 0, 255));

        /// OS의 펜과 내가 만든 펜을 교체
        /// HGDIOBJ는 HPEN의 상위(부모) 클래스
        /// 반환되는 HGDIOBJ를 형변환하여 HPEN 형태로 교체 후 보관
        HPEN osPen = (HPEN)SelectObject(hdc, myPen);

        /// 선 그리기
        //MoveToEx(hdc, 10, 10, NULL);
        //LineTo(hdc, x, y);
        /// 현재 마우스의 위치가 기준으로 설정
        MoveToEx(hdc, x, y, NULL);
        /// 이전 마우스 위치로 선을 그려라~
        LineTo(hdc, px, py);

        /// 그리기 끝난 상태에서 현재 위치를 이전 위치로 교체
        px = x;
        py = y;

        /// 원래 OS가 가지고 있었던 펜으로 다시 교체
        SelectObject(hdc, osPen);

        /// 생성한 펜 객체의 해제를 요청
        DeleteObject(myPen);

        /*
        /// 1. 새로운 선 객체를 생성
        myPen = CreatePen(PS_SOLID, 8, RGB(0, 255, 0));
        /// 2. 현재 OS의 펜과 교체한다.
        osPen = (HPEN)SelectObject(hdc, myPen);

        /// 3. HDC를 이용하여 그리기를 한다.
        MoveToEx(hdc, 900, 900, NULL);
        LineTo(hdc, x, y);
        /// 4. OS의 원래 펜으로 복원
        SelectObject(hdc, osPen);
        /// 5. 사용 완료된 객체를 해제
        DeleteObject(myPen);
        */
        /// HDC 핸들을 해제 요청
        ReleaseDC(hWnd, hdc);
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
