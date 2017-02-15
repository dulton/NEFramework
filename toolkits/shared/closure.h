#pragma once
//#include "util.h"
#include "base/callback/callback.h"
#include "cef/main_message_loop.h"

//Ͷ������UI�߳�
void Post2UI(const base::Closure& closure);

void Post2UI(const StdClosure& closure);

//Ͷ������ȫ��Misc�߳�
void Post2GlobalMisc(const StdClosure &closure);

//Ͷ������DB�߳�
void Post2Database(const StdClosure &closure);

//Ͷ������CURL�߳�
void Post2Curl(const StdClosure &closure);