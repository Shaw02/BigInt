// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。

#include <stdio.h>
#include <tchar.h>

#include <iomanip>
#include <iostream>
#include <string>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

using namespace std;

#include "BigInt.h"
#include "montgomery.h"
