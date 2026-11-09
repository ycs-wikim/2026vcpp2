// 1109.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1109.h"

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
    /// 단일 프로세스 동작을 허용하는 코드
    /// OS 레벨에 등록되어 있는 특정 이름이 존재하는지 확인
    HANDLE mux = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"상우기");

    if (NULL == mux)
    {
        /// 참이라면(mux == NULL), 이름이 없는 상태
        //MessageBox(NULL, L"없습니다.", L"이름이", MB_OK);
        CreateMutex(NULL, FALSE, L"상우기");
    }
    else
    {
        //MessageBox(NULL, L"있다니까요~ㅁㄴ라ㅓㅗㅁㄴ라ㅣㅗㅁㄴㅇ라ㅣㅗㅁ노ㅑ", L"이름이", MB_OK);
        return 0;
    }

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1109, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1109));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1109));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1109);
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

/// 공유 자원 선언
int g_x;
/// 첫번째 OS 객체 선언
CRITICAL_SECTION g_cs;
/// 두번째 OS 객체 선언 : 세마포어
HANDLE g_sem;
/// 세번재 OS 객체 선언 : 뮤텍스
HANDLE g_mux;

/// 전달할 정보 구조체 선언
typedef struct thread_info
{
    HWND            hWnd;
    LPARAM          lParam;
} TINFO, *PTINFO;

/// Semaphore를 위한 위치 좌표 선언
int g_pos = 15;

/// 공유 자원에 접근하는 스레드 작성 : Semaphore
DWORD WINAPI pig_sem(LPVOID param)
{
    PTINFO ptr = (PTINFO)param;
    int pos = g_pos;        /// 0 = 15;
    g_pos += 15;            /// 15 + 15 => 30
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);
    WCHAR buf[128] = { 0, };

    HDC hdc = GetDC(ptr->hWnd);

    /// INFINITE : 무한
    WaitForSingleObject(g_sem, INFINITE);
    for (int i = 0; i < y; i++)
    {
        /*
        memset(buf, 0x00, 128);
        wsprintf(buf, L"%d%% : ", i + 1);
        for (int j = 0; j < i; j++)
        {
            lstrcat(buf, L"|");
        }
        TextOut(hdc, 10, pos, buf, lstrlen(buf));
*/
        /////////////////////////// 임계 영역 시작
        g_x = x;    /// 공유 자원의 사용 시작
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);

        Sleep(30);          /// CPU 양보 => 문맥 교환
    }
    /////////////////////////// 임계 영역 끝
    ReleaseSemaphore(g_sem, 1, NULL);

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);      /// 스레드의 자원을 해제
    return 0;           /// 함수의 반환 값
}


/// 공유 자원에 접근하는 스레드 작성 : CRITICAL_SECTION
DWORD WINAPI pig_cs(LPVOID param)
{
    PTINFO ptr = (PTINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        EnterCriticalSection(&g_cs);
        /////////////////////////// 임계 영역 시작
        g_x = x;    /// 공유 자원의 사용 시작
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /////////////////////////// 임계 영역 끝
        LeaveCriticalSection(&g_cs);
        Sleep(30);          /// CPU 양보 => 문맥 교환
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);      /// 스레드의 자원을 해제
    return 0;           /// 함수의 반환 값
}

/// 뮤텍스용 스레드 함수
DWORD WINAPI pig_mux(LPVOID param)
{
    PTINFO ptr = (PTINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        /// 뮤텍스 객체를 대기
        WaitForSingleObject(g_mux, INFINITE);
        /////////////////////////// 임계 영역 시작
        g_x = x;    /// 공유 자원의 사용 시작
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /////////////////////////// 임계 영역 끝
        ReleaseMutex(g_mux);
        Sleep(30);          /// CPU 양보 => 문맥 교환
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);      /// 스레드의 자원을 해제
    return 0;           /// 함수의 반환 값
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        TINFO ti = { 0, };
        ti.hWnd = hWnd;
        ti.lParam = lParam;

        //CreateThread(NULL, 0, pig_cs, (LPVOID)&ti, 0, NULL);
        CreateThread(NULL, 0, pig_sem, (LPVOID)&ti, 0, NULL);
        //CreateThread(NULL, 0, pig_mux, (LPVOID)&ti, 0, NULL);
    }
        break;

    case WM_CREATE:
    {
        /// CRITICAL_SECTION 자료형 초기화
        InitializeCriticalSection(&g_cs);
        /// Semaphore 자료형 초기화
        /// 1번째 인수 : 보안 속성
        /// 2번째 인수 : 초기 값 설정
        ///     - 초기에 사용할 수 있는 자원의 개수
        /// 3번째 인수 : 최대 값 설정
        /// 4번째 인수 : 이름 지정
        ///     - 이름의 확인 범위가 내 프로그램 내부
        g_sem = CreateSemaphore(NULL, 5, 5, NULL);
        /// Mutex 객체 생성
        /// 1번째 인수 : 보안 속성
        /// 2번째 인수 : 소유주의 여부
        /// 3번째 인수 : 이름 지정
        ///     - 이름의 확인 범위가 내 OS 전체
        g_mux = CreateMutex(NULL, FALSE, NULL);
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
    {
        /// CRITICAL_SECTION 자료형 해제 => OS자원 해제
        DeleteCriticalSection(&g_cs);
        /// Semaphore 자료형 해제
        CloseHandle(g_sem);
        /// Mutex 자료형 해제
        CloseHandle(g_mux);

        PostQuitMessage(0);
    }
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
