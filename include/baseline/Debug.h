#ifndef DEBUG_H_
#define DEBUG_H_

namespace baseline {
#ifdef __cplusplus
template<bool> struct CompileTimeAssert;
template<> struct CompileTimeAssert<true> {};
#define COMPILE_TIME_ASSERT(_exp) \
  template class CompileTimeAssert< (_exp) >;
#endif
#define COMPILE_TIME_ASSERT_FUNCTION_SCOPE(_exp) \
  CompileTimeAssert<( _exp )>();

// ---------------------------------------------------------------------------

#ifdef __cplusplus
template<bool C, typename LSH, typename RHS> struct CompileTimeIfElse;
template<typename LHS, typename RHS>
struct CompileTimeIfElse<true,  LHS, RHS> {
  typedef LHS TYPE;
};
template<typename LHS, typename RHS>
struct CompileTimeIfElse<false, LHS, RHS> {
  typedef RHS TYPE;
};
#endif

// ---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

const char* stringForIndent( int32_t indentLevel );

typedef void ( *debugPrintFunc )( void* cookie, const char* txt );

void printTypeCode( uint32_t typeCode,
                    debugPrintFunc func = 0, void* cookie = 0 );

void printHexData( int32_t indent, const void* buf, size_t length,
                   size_t bytesPerLine = 16, int32_t singleLineBytesCutoff = 16,
                   size_t alignment = 0, bool cArrayStyle = false,
                   debugPrintFunc func = 0, void* cookie = 0 );

#ifdef __cplusplus
}
#endif
}

#endif // DEBUG_H_