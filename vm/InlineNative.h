/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Inlined native functions.
 */
#ifndef _DALVIK_INLINENATIVE
#define _DALVIK_INLINENATIVE

/* startup/shutdown */
bool dvmInlineNativeStartup(void);
void dvmInlineNativeShutdown(void);

/*
 * Basic 4-argument inline operation handler.
 */
typedef bool (*InlineOp4Func)(u4 arg0, u4 arg1, u4 arg2, u4 arg3,
    JValue* pResult);

/*
 * Table of inline operations.
 *
 * Try to keep this at a power-of-two size, so we don't have to multiply.
 *
 * TODO: might be to our advantage to generate a compact jump table on
 * the heap at runtime (or just declare two static tables, one with full
 * info and one with just function pointers).  Especially useful if we decide
 * to support other method call forms, e.g. /range.  We can also just
 * generate assembly code that knows how many args it needs and has the
 * target address embedded.
 */
typedef struct InlineOperation {
    InlineOp4Func   func;               /* MUST be first entry */
    const char*     classDescriptor;
    const char*     methodName;
    const char*     methodSignature;
} InlineOperation;

/* Must be kept in sync w/ gDvmInlineOpsTable in InlineNative.c */
typedef enum NativeInlineOps {
    INLINE_EMPTYINLINEMETHOD = 0,
    INLINE_STRING_CHARAT = 1,
    INLINE_STRING_COMPARETO = 2,
    INLINE_STRING_EQUALS = 3,
    INLINE_STRING_INDEXOF_I = 4,
    INLINE_STRING_INDEXOF_II = 5,
    INLINE_STRING_LENGTH = 6,
    INLINE_MATH_ABS_INT = 7,
    INLINE_MATH_ABS_LONG = 8,
    INLINE_MATH_ABS_FLOAT = 9,
    INLINE_MATH_ABS_DOUBLE = 10,
    INLINE_MATH_MIN_INT = 11,
    INLINE_MATH_MAX_INT = 12,
    INLINE_MATH_SQRT = 13,
    INLINE_MATH_COS = 14,
    INLINE_MATH_SIN = 15,
    INLINE_FLOAT_TO_INT_BITS = 16,
    INLINE_FLOAT_TO_RAW_INT_BITS = 17,
    INLINE_INT_BITS_TO_FLOAT = 18,
    INLINE_DOUBLE_TO_LONG_BITS = 19,
    INLINE_DOUBLE_TO_RAW_LONG_BITS = 20,
    INLINE_LONG_BITS_TO_DOUBLE = 21,
} NativeInlineOps;

/*
 * Get the inlineops table.
 */
const InlineOperation* dvmGetInlineOpsTable(void);
int dvmGetInlineOpsTableLength(void);

/*
 * The table, exposed so we can access it with C inlines.  Prefer access
 * through dvmGetInlineOpsTable().
 */
extern const InlineOperation gDvmInlineOpsTable[];

/*
 * Perform the operation specified by "opIndex".
 *
 * We want the arguments to appear in the first 4 registers so they can
 * be passed straight through to the handler function.  Ideally on ARM
 * they'll go into r0-r3 and stay there.
 *
 * Returns "true" if everything went normally, "false" if an exception
 * was thrown.
 */
INLINE bool dvmPerformInlineOp4Std(u4 arg0, u4 arg1, u4 arg2, u4 arg3,
    JValue* pResult, int opIndex)
{
    return (*gDvmInlineOpsTable[opIndex].func)(arg0, arg1, arg2, arg3, pResult);
}

/*
 * Like the "std" version, but will emit profiling info.
 */
bool dvmPerformInlineOp4Dbg(u4 arg0, u4 arg1, u4 arg2, u4 arg3,
    JValue* pResult, int opIndex);

#endif /*_DALVIK_INLINENATIVE*/
