//===========================================================
//
// SV_Types 
//
// Made by save_as, 2025.
// 
// A convienient header file to use across projects 
// with common typedefs, enums, structs, and macros. 
//===========================================================
#ifndef SV_TYPES

	#define SV_TYPES // only do this once
	
	// Typedefs
	
	typedef unsigned char u8;
	typedef signed char i8;
	typedef unsigned short u16;
	typedef signed short i16;
	typedef unsigned int u32;
	typedef signed int i32;
	typedef unsigned long u64;
	typedef signed long i64;
	
	typedef u8 bool;
	
	// For me, this makes it easier to tell when we're working with pointers or strings.
	typedef char* string;
	
	// Structs

	// Vector structs
	typedef struct {
		u32 x;
		u32 y;
	} Vector2;
	
	typedef struct {
		u32 x;
		u32 y;
		u32 z;
	} Vector3;
	
	typedef struct {
		u32 w;
		u32 x;
		u32 y;
		u32 z;
	} Vector4;
	
	// Color structs
	
	typedef struct {
		u8 r;
		u8 g;
		u8 b;
	} rgb8;
	
	typedef struct {
		u8 r;
		u8 g;
		u8 b;
		u8 a;
	} rgba8;
	
	typedef struct {
		u16 r;
		u16 g;
		u16 b;
	} rgb16;
	
	typedef struct {
		u16 r;
		u16 g;
		u16 b;
		u16 a;
	} rgba16;

	typedef struct {
		u32 r;
		u32 g;
		u32 b;
	} rgb32;
	
	typedef struct {
		u32 r;
		u32 g;
		u32 b;
		u32 a;
	} rgba32;
	
	
	// Enums
	
	typedef enum {
		RESULT_SUCCESS,              // Success!
		RESULT_ERROR,                // Error (generic)
		RESULT_BADDATA,              // Bad data passed
		RESULT_NODATA,               // Required data not passed
		RESULT_TOOMUCHDATA,          // Too much data passed
		RESULT_BUFFEROVERFLOW,       // Buffer overflow
		RESULT_STACKOVERFLOW,        // Stack overflow
		RESULT_BREAK,                // Break
		RESULT_NULLPTR,              // Tried to access a null pointer
		RESULT_ILLEGALINSTRUCTION,   // Illegal instruction
		RESULT_OVERFLOW,             // Overflow
		RESULT_BREAKPOINT,           // Breakpoint
		RESULT_DIVIDEBYZERO,         // Divide by zero
		RESULT_SEGMENTVIOLATION,     // Segment violation
		RESULT_CANTFINDDATA,         // Data passed does not exist
		RESULT_COULDNTINITDEPENDENCY // Something we need did not initialize properly
	} RESULT;
	
	// Macros and defines
	
	#define TRUE 1
	#define true 1
	#define FALSE 0
	#define false 0
	
	// There's a good chance NULL is defined.
	#ifndef NULL
		#define NULL (void*)0
	#endif
	
	#ifndef null
		#define null NULL
	#endif
	
	#define IS_NULL(x) ((void*)(x) == NULL)
	
	// Bit and bit mask macros 
	#define BIT(x, y) ((x) & (1 << y))
	#define BIT_MASK(x) (1 << x)
	
	#define SET_BIT(x, y)   ((x) |= (1 << (y)))
	
	#define CLEAR_BIT(x, y) ((x) &= ~(1 << (y)))

	// If we want to use a specific byte of something or a specific dword of something, use these
	#define BYTE_MASK BIT_MASK(0)|BIT_MASK(1)|BIT_MASK(2)|BIT_MASK(3)|BIT_MASK(4)|BIT_MASK(5)|BIT_MASK(6)|BIT_MASK(7)
	#define WORD_MASK (BYTE_MASK<<8)|BYTE_MASK
	#define DWORD_MASK (WORD_MASK<<16)|WORD_MASK
	#define QWORD_MASK (DWORD_MASK<<32)|DWORD_MASK
	
	#define HIGH_BYTE(x) ((x >> 8) & BYTE_MASK)
	
#endif