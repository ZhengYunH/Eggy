#pragma once

#define DEBUG_MODE 1
#define D3D11_DEVICE 1
#define VULKAN_DEVICE 0


#if DEBUG_MODE
	#include <assert.h>
	#define HYBRID_CHECK(X, ...) assert((X))
	#define HYBRID_RUN(X, ...) X
	#define DEBUG_RUN(X) X
#else
	#define HYBRID_CHECK(X, ...) 
	#define HYBRID_RUN(X, ...)
	#define DEBUG_RUN(X)
#endif

#define Unimplement(...) HYBRID_CHECK(0, ##__VA_ARGS__)
#define UNEXPECTED(X) (X)
#define SafeDestroy(X) if(X){ delete X; X=nullptr;}
#define SafeDestroyArray(X) if(X){delete[] X; X=nullptr;}
#define FORCEINLINE __forceinline
