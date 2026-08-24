// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"					/// 윈도우 버전과 관련된 헤더
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>					/// GUI 작성 또는 WIN32 앱 작성시
										/// 필수적으로 포함해야 하는 헤더
// C 런타임 헤더 파일입니다.
#include <stdlib.h>						/// srand, rand
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
