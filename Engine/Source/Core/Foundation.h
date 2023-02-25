#pragma once

#define DEBUG_MODE 1
#define D3D11_DEVICE 1
#define VULKAN_DEVICE 0

#define CLIENT 1
#define SERVER 0


#if DEBUG_MODE
	#include <assert.h>
	#define HYBRID_CHECK(X, ...) assert((X))
	#define HYBRID_RUN(X, ...) X
	#define DEBUG_RUN(X) X
	#define DEBUG_CHECK(X, ...) assert((X))
	#define LOG(...) 
#else
	#define HYBRID_CHECK(X, ...) 
	#define HYBRID_RUN(X, ...)
	#define DEBUG_RUN(X)
	#define DEBUG_CHECK(X, ...)
	#define LOG(...) 
#endif

#define Unimplement(...) HYBRID_CHECK(0, ##__VA_ARGS__)
#define UNEXPECTED(X) (X)
#define SafeDestroy(X) if(X){ delete X; X=nullptr;}
#define SafeDestroyArray(X) if(X){delete[] X; X=nullptr;}
#define FORCEINLINE __forceinline
#define StaticName(Name) #Name


#define _NUM_ARGS(X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...)   N
#define NUM_ARGS(...) _NUM_ARGS(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define EXPAND(X)       X
#define FIRSTARG(X, ...)    (X)
#define RESTARGS(X, ...)    (__VA_ARGS__)
#define FOREACH(MACRO, LIST)    FOREACH_(NUM_ARGS LIST, MACRO, LIST)
#define FOREACH_(N, M, LIST)    FOREACH__(N, M, LIST)
#define FOREACH__(N, M, LIST)   FOREACH_##N(M, LIST)
#define FOREACH_1(M, LIST)  M LIST
#define FOREACH_2(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_1(M, RESTARGS LIST)
#define FOREACH_3(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_2(M, RESTARGS LIST)
#define FOREACH_4(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_3(M, RESTARGS LIST)
#define FOREACH_5(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_4(M, RESTARGS LIST)
#define STRINGIFY(X)    #X
#define STRINGIFY_MULTI(...) FOREACH(STRINGIFY, (__VA_ARGS__))