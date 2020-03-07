#pragma once
#include <stdio.h>
#include <android/log.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <stdbool.h>
#include <cstring>
#include <iostream>
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, "InlineHook", fmt, ##args);

void InlineHook(void *symbol, void*replace);