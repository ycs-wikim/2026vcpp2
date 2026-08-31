// 0824.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"          /// 윈도우 GUI 작성을 위한 기초 헤더가 포함
#include "0824.h"               /// 프로젝트에서 사용하는 자원 정보 헤더

/// 상수 선언
#define MAX_LOADSTRING 100

/// 자료형
/// 자료형 이름의 구조
///     1. 모두 대문자로 구성
///     2. H로 시작하면 HANDLE(핸들)의 첫글자다.
///         핸들은 몸체가 OS 내부에 존재하고, 관리하기 위한 포인터
///     3. WND == Window
///     4. 함수 뒤에 W가 붙어 있다면 Wide-Char(Uni-Code) 자료형이다.
///     5. 함수 뒤에 EX, Ex(Extension)가 붙어 있다면, 확장되었다는 의미


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
                        /// 자기 자신이 메모리에 적재되었을 때의
                        /// 자신을 관리할 수 있는 핸들
                        /// 클래스 내에서 자신을 가리키는 포인터 : this
                        /// this->멤버 접근. VC++에서는 불가능
//char szT[100];        아래는 문자열 선언과 동일
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/// Window 프로그램의 main 함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,             
                    /// OS가 메모리에 잘 적재한 다음 해당 정보를
                    /// 관리하기 위한 핸들 값을 나에게 전달해준다.
                     _In_opt_ HINSTANCE hPrevInstance,
                    /// 이전에 실행된 나와 같은 인스턴스의 핸들
                    /// code/text영역이 읽기 전용이라 공유해서 사용하기 위해
                    /// 현재는 큰 의미가 없음. => OS가 알아서 해줌.
                     _In_ LPWSTR    lpCmdLine,
                    /// 명령줄에 입력된 정보. 실행 인수 값
                     _In_ int       nCmdShow
                    /// 실행 시에 화면 표시 여부를 결정
                    )
{
    /// 해당 인수가 사용되지 않거나, NULL이면 제거한다.
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0824, szWindowClass, MAX_LOADSTRING);
    /// 윈도우 클래스를 생성하고, 초기화
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    /// 단축키 로딩
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0824));

    /// 윈도우 GUI에서 가장 중요한 자료형
    /// 구조와 특징에 대해서 정확하게 이해해야 한다!
    MSG msg;
/*
    typedef struct tagMSG {
        HWND        hwnd;           /// 어떤 윈도우에 발생했느냐
        UINT        message;        /// 정수로 구성된 메시지 번호
                                    /// 사용자의 입력(키보드, 마우스) 정보
        WPARAM      wParam;         /// Word Parameter (== char == 1byte)
                                    /// 현재는 64bit int 값
                                    /// 사용자가 입력한 키보드 값이 전달
        LPARAM      lParam;         /// Long Parameter (== int == 2byte)
                                    /// 현재는 64bit int 값
                                    /// 사용자의 마우스 입력 정보가 전달
        DWORD       time;           /// 발생 시간
        POINT       pt;             /// 위치 정보
#ifdef _MAC
        DWORD       lPrivate;
#endif
    }
*/
    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            /// MSG 정보의 번역
            TranslateMessage(&msg);
            /// 실행 요청 => OS에게
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
    /// WND : 윈도우
    /// CLASS : 클래스     /// 이전에 윈도우 클래스가 이미 있었다.
    /// EX : 확장
    /// W : 유니코드를 지원하도록 구성
    /// 윈도우를 구성하는 유니코드를 지원하는 확장 클래스를 선언
    WNDCLASSEXW wcex;

    /// 클래스의 멤버에 값을 할당
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    /// Wnd : 윈도우
    /// Proc : procedure(함수) ==> 윈도우 함수
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0824));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0824);
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
            /// 전역 변수에 보관하면 어디서나 접근 가능

   /// H : 핸들           , WND : 윈도우
   /// Create Window => 윈도우 생성해줘
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   /// 오류 확인
   if (!hWnd)
   {
      return FALSE;
   }

   /// 화면에 표시해줘
   ShowWindow(hWnd, nCmdShow);
   /// 화면에 변경 사항이 있으면 최신 정보로 표시해줘
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
/// CALLBACK : 일반적으로 프로그래머(작성자)가 호출하는 함수 형식이 아니라
/// OS 또는 외부로부터 호출되는 함수를 지칭하는 용어
/// 인수 정보
/// 1. HWND : 메시지를 받을 대상 윈도우
/// 2. UINT : 사용자의 입력 종류
/// 3. WPARAM : 키보드로 입력된 정보 + 메뉴 선택 정보
/// 4. LPARAM : 마우스로 입력된 정보
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /// 사용자의 입력에 대한 처리 시작
    switch (message)
    {
    /// WM_ : Window Message
    case WM_COMMAND:        /// 메뉴 선택
        {
            /// LOWORD() 매크로 : 자료형을 반으로 나누어 하위 16(32)bit를
            ///             추출해서 숫자 값으로 반환해주는 매크로
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
                /// 정보 메뉴를 선택한 경우
            case IDM_ABOUT:
                /// 메시지 박스
                /// GUI 응용 프로그램에서 메시지 박스가 실행 중일때,
                /// 아래로 코드가 진행되지 않는 형태의 구조를 지칭하는 용어
                /// 모달 형태의 창 (Vs. 모달리스 - 메시지 박스와 관계없이 그냥 진행)
                MessageBox(hWnd, L"이사람들아... 초반이 중요해 지금 잘해야 해", L"학기초야!", MB_OK);

                /// 정보 창을 화면에 출력해줘
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                MessageBox(hWnd, L"프로그램 종료", L"학기초야!", MB_OK);
                /// 해당 윈도우를 파괴 ==> 종료해라
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    /// 항상 화면에 표시되어야 하는 내용을 입력하는 윈도우 메시지
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            /// DC - Device(모니터 화면) Context
            /// Context Switching : 문맥 교환
            /// 지금부터 그리기 시작!
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            /// 이제 그리기 종료
            EndPaint(hWnd, &ps);
        }
        break;

    /// 프로그램이 종료되는 시점에 전달되는 윈도우 메시지
    /// 프로그램이 완전히 종료된 상태는 아니다!
    /// 곧 종료될 예정이니, 정리할 수 있는 시간을 OS가 주는 윈도우 메시지
    case WM_DESTROY:
        //MessageBox(hWnd, L"프로그램 종료 WM_DESTROY", L"학기초야!", MB_OK);
        /// 프로그램에 종료 메시지를 직접 전달한다.
        PostQuitMessage(0);
        break;
    /// 처리할 메시지에 대한 코드가 존재하지 않는 경우
    default:
        /// 기본 윈도우 프로시저
        /// 내가 처리하도록 구성된 메시지가 아니기 때문에
        /// OS야 니가 알아서 처리해줘 ==> 사실상 메시지 정보 삭제
        /// OS 내부에 동적으로 정보가 만들어져 있기 때문에 반드시
        /// 전달해서 알려줘야 자원이 정리가 정확하게 이루어진다.
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
