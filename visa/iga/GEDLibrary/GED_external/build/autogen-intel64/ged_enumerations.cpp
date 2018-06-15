/*===================== begin_copyright_notice ==================================

Copyright (c) 2017 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


======================= end_copyright_notice ==================================*/

/*
 * !!! DO NOT EDIT THIS FILE !!!
 *
 * This file was automagically crafted by GED's model parser.
 */

#include <cstddef>
#include "ged_enumerations_internal.h"
const char* AccessModeEnumeration[3] =
{
    "Align1", // 0
    "Align16", // 1
    NULL // 2
}; // AccessModeEnumeration[]
const char* AccWrCtrlEnumeration[3] =
{
    "", // 0
    "AccWrEn", // 1
    NULL // 2
}; // AccWrCtrlEnumeration[]
const char* AddrModeEnumeration[3] =
{
    "Direct", // 0
    "Indirect", // 1
    NULL // 2
}; // AddrModeEnumeration[]
const char* ArchRegEnumeration[16] =
{
    "null", // 0
    "a0", // 1
    "acc", // 2
    "f", // 3
    "ce", // 4
    "msg", // 5
    "sp", // 6
    "sr0", // 7
    "cr0", // 8
    "n0", // 9
    "ip", // 10
    "tdr", // 11
    "tm0", // 12
    "fc", // 13
    "dbg0", // 14
    NULL // 15
}; // ArchRegEnumeration[]
const char* AtomicOperationTypeEnumeration[16] =
{
    "AOP_AND", // 0
    "AOP_OR", // 1
    "AOP_XOR", // 2
    "AOP_MOV", // 3
    "AOP_INC", // 4
    "AOP_DEC", // 5
    "AOP_ADD", // 6
    "AOP_SUB", // 7
    "AOP_REVSUB", // 8
    "AOP_IMAX", // 9
    "AOP_IMIN", // 10
    "AOP_UMAX", // 11
    "AOP_UMIN", // 12
    "AOP_CMPWR", // 13
    "AOP_PREDEC", // 14
    NULL // 15
}; // AtomicOperationTypeEnumeration[]
const char* BlockSizeEnumeration[6] =
{
    "OW1L", // 0
    "OW1H", // 1
    "OW2", // 2
    "OW4", // 3
    "OW8", // 4
    NULL // 5
}; // BlockSizeEnumeration[]
const char* BranchCtrlEnumeration[3] =
{
    "", // 0
    "Branch", // 1
    NULL // 2
}; // BranchCtrlEnumeration[]
const char* BranchCtrlEnumerationDisassembly[3] =
{
    "0", // 0
    "1", // 1
    NULL // 2
}; // BranchCtrlEnumerationDisassembly[]
const char* ChannelMaskEnumeration[6] =
{
    "R", // 0
    "", // 1
    "B", // 2
    "G", // 3
    "A", // 4
    NULL // 5
}; // ChannelMaskEnumeration[]
const char* ChannelOffsetEnumeration[9] =
{
    "M0", // 0
    "M4", // 1
    "M8", // 2
    "M12", // 3
    "M16", // 4
    "M20", // 5
    "M24", // 6
    "M28", // 7
    NULL // 8
}; // ChannelOffsetEnumeration[]
const char* CondModifierEnumeration[10] =
{
    "", // 0
    "z", // 1
    "nz", // 2
    "g", // 3
    "ge", // 4
    "l", // 5
    "le", // 6
    "o", // 7
    "u", // 8
    NULL // 9
}; // CondModifierEnumeration[]
const char* DataTypeEnumeration[16] =
{
    "ud", // 0
    "d", // 1
    "uw", // 2
    "w", // 3
    "ub", // 4
    "b", // 5
    "df", // 6
    "f", // 7
    "uq", // 8
    "q", // 9
    "hf", // 10
    "uv", // 11
    "vf", // 12
    "v", // 13
    "nf", // 14
    NULL // 15
}; // DataTypeEnumeration[]
const char* DebugCtrlEnumeration[3] =
{
    "", // 0
    "Breakpoint", // 1
    NULL // 2
}; // DebugCtrlEnumeration[]
const char* DepCtrlEnumeration[5] =
{
    "", // 0
    "NoDDClr", // 1
    "NoDDChk", // 2
    "NoDDClr, NoDDChk", // 3
    NULL // 4
}; // DepCtrlEnumeration[]
const char* DstChanEnEnumeration[17] =
{
    "", // 0
    "x", // 1
    "y", // 2
    "xy", // 3
    "z", // 4
    "xz", // 5
    "yz", // 6
    "xyz", // 7
    "w", // 8
    "xw", // 9
    "yw", // 10
    "xyw", // 11
    "zw", // 12
    "xzw", // 13
    "yzw", // 14
    "xyzw", // 15
    NULL // 16
}; // DstChanEnEnumeration[]
const char* EOTEnumeration[3] =
{
    "", // 0
    "EOT", // 1
    NULL // 2
}; // EOTEnumeration[]
const char* ExecMaskOffsetCtrlEnumeration[15] =
{
    "N1", // 0
    "N2", // 1
    "N3", // 2
    "N4", // 3
    "N5", // 4
    "N6", // 5
    "N7", // 6
    "N8", // 7
    "Q1", // 8
    "Q2", // 9
    "Q3", // 10
    "Q4", // 11
    "H1", // 12
    "H2", // 13
    NULL // 14
}; // ExecMaskOffsetCtrlEnumeration[]
const char* ExecutionDataTypeEnumeration[3] =
{
    "Integer", // 0
    "Float", // 1
    NULL // 2
}; // ExecutionDataTypeEnumeration[]
const char* HeaderPresentEnumeration[3] =
{
    "no", // 0
    "yes", // 1
    NULL // 2
}; // HeaderPresentEnumeration[]
const char* MaskCtrlEnumeration[3] =
{
    "", // 0
    "NoMask", // 1
    NULL // 2
}; // MaskCtrlEnumeration[]
const char* MathFCEnumeration[15] =
{
    "INV", // 0
    "LOG", // 1
    "EXP", // 2
    "SQRT", // 3
    "RSQ", // 4
    "SIN", // 5
    "COS", // 6
    "FDIV", // 7
    "POW", // 8
    "INT DIV BOTH", // 9
    "INT DIV QUOTIENT", // 10
    "INT DIV REMAINDER", // 11
    "INVM", // 12
    "RSQRTM", // 13
    NULL // 14
}; // MathFCEnumeration[]
const char* MessageTypeEnumeration[64] =
{
    "MSD0R_HWB", // 0
    "MSD0W_HWB", // 1
    "MT0R_OWB", // 2
    "MT0R_OWUB", // 3
    "MT0R_OWDB", // 4
    "MT0R_DWS", // 5
    "MT0R_BS", // 6
    "MT0_MEMFENCE", // 7
    "MT0W_OWB", // 8
    "MT0W_OWDB", // 9
    "MT0W_DWS", // 10
    "MT0W_BS", // 11
    "MT1R_T", // 12
    "MT1R_US", // 13
    "MT1A_UI", // 14
    "MT1A_UI4x2", // 15
    "MT1R_MB", // 16
    "MT1R_TS", // 17
    "MT1A_TA", // 18
    "MT1A_TA4x2", // 19
    "MT1W_US", // 20
    "MT1W_MB", // 21
    "MT1A_TC", // 22
    "MT1A_TC4x2", // 23
    "MT1W_TS", // 24
    "MT1R_A64_SB", // 25
    "MT1R_A64_US", // 26
    "MT1A_A64_UI", // 27
    "MT1A_A64_UI4x2", // 28
    "MT1R_A64_B", // 29
    "MT1W_A64_B", // 30
    "MT1W_A64_US", // 31
    "MT1W_A64_SB", // 32
    "MT2R_US", // 33
    "MT2R_A64_SB", // 34
    "MT2R_A64_US", // 35
    "MT2R_BS", // 36
    "MT2W_US", // 37
    "MT2W_A64_US", // 38
    "MT2W_A64_SB", // 39
    "MT2W_BS", // 40
    "MT_CC_OWB", // 41
    "MT_CC_OWUB", // 42
    "MT_CC_OWDB", // 43
    "MT_CC_DWS", // 44
    "MT_SC_OWUB", // 45
    "MT_SC_MB", // 46
    "MT_RSI", // 47
    "MT_RTW", // 48
    "MT_RTR", // 49
    "MTR_MB", // 50
    "MTRR_TS", // 51
    "MTRA_TA", // 52
    "MT_MEMFENCE", // 53
    "MTW_MB", // 54
    "MTRW_TS", // 55
    "MT0R_US", // 56
    "MT0A_UI", // 57
    "MT0W_US", // 58
    "MT1A_UF4x2", // 59
    "MT1A_UF", // 60
    "MT1A_A64_UF", // 61
    "MT1A_A64_UF4x2", // 62
    NULL // 63
}; // MessageTypeEnumeration[]
const char* MessageTypeEnumerationDisassembly[64] =
{
    "Scratch Block Read", // 0
    "Scratch Block Write", // 1
    "OWord Block Read", // 2
    "Unaligned OWord Block Read", // 3
    "OWord Dual Block Read", // 4
    "DWord Scattered Read", // 5
    "Byte Scattered Read", // 6
    "Memory Fence", // 7
    "OWord Block Write", // 8
    "OWord Dual Block Write", // 9
    "DWord Scattered Write", // 10
    "Byte Scattered Write", // 11
    "Transpose Read", // 12
    "Untyped Surface Read", // 13
    "Untyped Atomic Integer Operation", // 14
    "Untyped Atomic Integer Operation SIMD4x2", // 15
    "Media Block Read", // 16
    "Typed Surface Read", // 17
    "Typed Atomic Integer Operation", // 18
    "Typed Atomic Integer Operation SIMD4x2", // 19
    "Untyped Surface Write", // 20
    "Media Block Write", // 21
    "Typed Atomic Counter Operation", // 22
    "Typed Atomic Counter Operation SIMD4x2", // 23
    "Typed Surface Write", // 24
    "A64 Scattered Read", // 25
    "A64 Untyped Surface Read", // 26
    "A64 Untyped Atomic Integer Operation", // 27
    "A64 Untyped Atomic Integer Operation SIMD4x2", // 28
    "A64 Block Read", // 29
    "A64 Block Write", // 30
    "A64 Untyped Surface Write", // 31
    "A64 Scattered Write", // 32
    "Untyped Surface Read", // 33
    "A64 Scattered Read", // 34
    "A64 Untyped Surface Read", // 35
    "Byte Scattered Read", // 36
    "Untyped Surface Write", // 37
    "A64 Untyped Surface Write", // 38
    "A64 Scattered Write", // 39
    "Byte Scattered Write", // 40
    "Oword Block Read Constant Cache", // 41
    "Unaligned Oword Block Read Constant Cache", // 42
    "Oword Dual Block Read Constant Cache", // 43
    "Dword Scattered Read Constant Cache", // 44
    "Unaligned Oword Block Read Sampler Cache", // 45
    "Media Block Read Sampler Cache", // 46
    "Read Surface Info", // 47
    "Render Target Write", // 48
    "Render Target Read", // 49
    "Media Block Read", // 50
    "Typed Surface Read", // 51
    "Typed Atomic Operation", // 52
    "Memory Fence", // 53
    "Media Block Write", // 54
    "Typed Surface Write", // 55
    "Untyped Surface Read", // 56
    "Untyped Atomic Operation", // 57
    "Untyped Surface Write", // 58
    "A64 Untyped Atomic Float Add", // 59
    "Untyped Atomic Float Operation", // 60
    "A64 Untyped Atomic Float Operation", // 61
    "A64 Untyped Atomic Float Operation SIMD4x2", // 62
    NULL // 63
}; // MessageTypeEnumerationDisassembly[]
const char* NoSrcDepSetEnumeration[3] =
{
    "", // 0
    "NoSrcDepSet", // 1
    NULL // 2
}; // NoSrcDepSetEnumeration[]
const char* OpcodeEnumeration[75] =
{
    "illegal", // 0
    "mov", // 1
    "sel", // 2
    "movi", // 3
    "not", // 4
    "and", // 5
    "or", // 6
    "xor", // 7
    "shr", // 8
    "shl", // 9
    "smov", // 10
    "asr", // 11
    "cmp", // 12
    "cmpn", // 13
    "csel", // 14
    "bfrev", // 15
    "bfe", // 16
    "bfi1", // 17
    "bfi2", // 18
    "jmpi", // 19
    "brd", // 20
    "if", // 21
    "brc", // 22
    "else", // 23
    "endif", // 24
    "while", // 25
    "break", // 26
    "cont", // 27
    "halt", // 28
    "calla", // 29
    "call", // 30
    "ret", // 31
    "goto", // 32
    "join", // 33
    "wait", // 34
    "send", // 35
    "sendc", // 36
    "sends", // 37
    "sendsc", // 38
    "math", // 39
    "add", // 40
    "mul", // 41
    "avg", // 42
    "frc", // 43
    "rndu", // 44
    "rndd", // 45
    "rnde", // 46
    "rndz", // 47
    "mac", // 48
    "mach", // 49
    "lzd", // 50
    "fbh", // 51
    "fbl", // 52
    "cbit", // 53
    "addc", // 54
    "subb", // 55
    "sad2", // 56
    "sada2", // 57
    "dp4", // 58
    "dph", // 59
    "dp3", // 60
    "dp2", // 61
    "line", // 62
    "pln", // 63
    "mad", // 64
    "lrp", // 65
    "madm", // 66
    "nop", // 67
    "ror", // 68
    "rol", // 69
    "dp4a", // 70
    "f32to16", // 71
    "f16to32", // 72
    "dim", // 73
    NULL // 74
}; // OpcodeEnumeration[]
const char* PredCtrlEnumeration[19] =
{
    "", // 0
    "", // 1
    "anyv", // 2
    "allv", // 3
    "any2h", // 4
    "all2h", // 5
    "any4h", // 6
    "all4h", // 7
    "any8h", // 8
    "all8h", // 9
    "any16h", // 10
    "all16h", // 11
    "any32h", // 12
    "all32h", // 13
    "x", // 14
    "y", // 15
    "z", // 16
    "w", // 17
    NULL // 18
}; // PredCtrlEnumeration[]
const char* PredInvEnumeration[3] =
{
    "+", // 0
    "-", // 1
    NULL // 2
}; // PredInvEnumeration[]
const char* RegFileEnumeration[4] =
{
    "ARF", // 0
    "GRF", // 1
    "IMM", // 2
    NULL // 3
}; // RegFileEnumeration[]
const char* RegFileEnumerationDisassembly[4] =
{
    "", // 0
    "r", // 1
    "", // 2
    NULL // 3
}; // RegFileEnumerationDisassembly[]
const char* RepCtrlEnumeration[3] =
{
    "", // 0
    "", // 1
    NULL // 2
}; // RepCtrlEnumeration[]
const char* RepCtrlEnumerationDisassembly[3] =
{
    "", // 0
    "r", // 1
    NULL // 2
}; // RepCtrlEnumerationDisassembly[]
const char* ReturnDataControlEnumeration[3] =
{
    "", // 0
    "", // 1
    NULL // 2
}; // ReturnDataControlEnumeration[]
const char* ReturnDataControlEnumerationDisassembly[3] =
{
    "no", // 0
    "yes", // 1
    NULL // 2
}; // ReturnDataControlEnumerationDisassembly[]
const char* SaturateEnumeration[3] =
{
    "", // 0
    "sat", // 1
    NULL // 2
}; // SaturateEnumeration[]
const char* SFIDEnumeration[16] =
{
    "NULL", // 0
    "SAMPLER", // 1
    "GATEWAY", // 2
    "DP_DC2", // 3
    "DP_RC", // 4
    "URB", // 5
    "SPAWNER", // 6
    "VME", // 7
    "DP_DCRO", // 8
    "DP_DC0", // 9
    "PI", // 10
    "DP_DC1", // 11
    "CRE", // 12
    "DP_SAMPLER", // 13
    "DP_CC", // 14
    NULL // 15
}; // SFIDEnumeration[]
const char* SFIDEnumerationDisassembly[16] =
{
    "Null", // 0
    "Sampler", // 1
    "Message Gateway", // 2
    "Data Cache Data Port 2", // 3
    "Render Cache Data Port", // 4
    "URB", // 5
    "Thread Spawner", // 6
    "Video Motion Estimation", // 7
    "Data Cache Read Only Data Port", // 8
    "Data Cache Data Port 0", // 9
    "Pixel Interpolator", // 10
    "Data Cache Data Port 1", // 11
    "Check and Refinement Engine", // 12
    "Sampler Cache Data Port", // 13
    "Constant Cache Data Port", // 14
    NULL // 15
}; // SFIDEnumerationDisassembly[]
const char* SIMDModeEnumeration[4] =
{
    "SIMD4x2", // 0
    "SIMD16", // 1
    "SIMD8", // 2
    NULL // 3
}; // SIMDModeEnumeration[]
const char* SlotGroupEnumeration[4] =
{
    "SIMD4x2", // 0
    "low_8", // 1
    "high_8", // 2
    NULL // 3
}; // SlotGroupEnumeration[]
const char* SpecialAccEnumeration[10] =
{
    "acc2", // 0
    "acc3", // 1
    "acc4", // 2
    "acc5", // 3
    "acc6", // 4
    "acc7", // 5
    "acc8", // 6
    "acc9", // 7
    "noacc", // 8
    NULL // 9
}; // SpecialAccEnumeration[]
const char* SrcModEnumeration[5] =
{
    "", // 0
    "(abs)", // 1
    "-", // 2
    "-(abs)", // 3
    NULL // 4
}; // SrcModEnumeration[]
const char* SubFuncIDEnumeration[8] =
{
    "GetTimeStamp", // 0
    "BarrierMsg", // 1
    "OpenGateway", // 2
    "CloseGateway", // 3
    "ForwardMsg", // 4
    "UpdateGateState", // 5
    "MMIOReadWrite", // 6
    NULL // 7
}; // SubFuncIDEnumeration[]
const char* SwizzleEnumeration[5] =
{
    "x", // 0
    "y", // 1
    "z", // 2
    "w", // 3
    NULL // 4
}; // SwizzleEnumeration[]
const char* ThreadCtrlEnumeration[5] =
{
    "", // 0
    "Atomic", // 1
    "NoPreempt", // 2
    "Switch", // 3
    NULL // 4
}; // ThreadCtrlEnumeration[]
