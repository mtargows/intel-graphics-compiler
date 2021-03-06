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

#include "common/LLVMWarningsPush.hpp"
#include "llvm/Config/llvm-config.h"
#include "llvm/Support/ScaledNumber.h"
#include "llvm/Support/CommandLine.h"
#include "common/LLVMWarningsPop.hpp"

#include <cstring>
#include <string>
#include <stdexcept>
#include <fstream>

#include "AdaptorCommon/customApi.hpp"
#include "AdaptorOCL/OCL/LoadBuffer.h"
#include "AdaptorOCL/OCL/BuiltinResource.h"
#include "AdaptorOCL/OCL/TB/igc_tb.h"

#include "AdaptorOCL/UnifyIROCL.hpp"
#include "AdaptorOCL/DriverInfoOCL.hpp"

#include "Compiler/MetaDataApi/IGCMetaDataHelper.h"
#include "common/debug/Dump.hpp"
#include "common/debug/Debug.hpp"
#include "common/igc_regkeys.hpp"
#include "common/secure_mem.h"
#include "common/shaderOverride.hpp"

#include "CLElfLib/ElfReader.h"
#include "usc.h"

#include "AdaptorOCL/OCL/sp/gtpin_igc_ocl.h"

#if !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)
#include "common/LLVMWarningsPush.hpp"
#include <llvm/ADT/ScopeExit.h>
#include "vc/igcdeps/cmc.h"
#include "vc/igcdeps/ShaderDump.h"
#include "vc/Support/StatusCode.h"
#include "vc/GenXCodeGen/GenXWrapper.h"
#include "common/LLVMWarningsPop.hpp"
#endif // !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)

#include <iStdLib/MemCopy.h>


#if defined(IGC_SPIRV_ENABLED)
#include "common/LLVMWarningsPush.hpp"
#include "AdaptorOCL/SPIRV/SPIRVconsum.h"
#include "common/LLVMWarningsPop.hpp"
#include "AdaptorOCL/SPIRV/libSPIRV/SPIRVModule.h"
#include "AdaptorOCL/SPIRV/libSPIRV/SPIRVValue.h"
#endif

#ifdef IGC_SPIRV_TOOLS_ENABLED
#include "spirv-tools/libspirv.h"
#endif

#include "common/LLVMWarningsPush.hpp"
#include "llvmWrapper/Bitcode/BitcodeWriter.h"
#include "common/LLVMWarningsPop.hpp"

#include <sstream>
#include <iomanip>
#include "Probe/Assertion.h"

//In case of use GT_SYSTEM_INFO in GlobalData.h from inc/umKmInc/sharedata.h
//We have to do this temporary defines

#ifdef BOOLEAN
#define BOOLEAN_IGC_REPLACED
#pragma push_macro("BOOLEAN")
#undef BOOLEAN
#endif
#define BOOLEAN uint8_t

#ifdef HANDLE
#define HANDLE_IGC_REPLACED
#pragma push_macro("HANDLE")
#undef HANDLE
#endif
#define HANDLE void*

#ifdef VER_H
#define VER_H_IGC_REPLACED
#pragma push_macro("VER_H")
#undef VER_H
#endif
#define VER_H

#include "GlobalData.h"

//We undef BOOLEAN HANDLE and VER_H here
#undef VER_H
#ifdef VER_H_IGC_REPLACED
#pragma pop_macro("VER_H")
#undef VER_H_IGC_REPLACED
#endif

#undef BOOLEAN
#ifdef BOOLEAN_IGC_REPLACED
#pragma pop_macro("BOOLEAN")
#undef BOOLEAN_IGC_REPLACED
#endif

#undef HANDLE
#ifdef HANDLE_IGC_REPLACED
#pragma pop_macro("HANDLE")
#undef HANDLE_IGC_REPLACED
#endif

#if !defined(_WIN32)
#   define strtok_s strtok_r
#   define _strdup strdup
#   define _snprintf snprintf
#endif


#include "common/LLVMWarningsPush.hpp"
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Linker/Linker.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include "common/LLVMWarningsPop.hpp"

using namespace IGC::IGCMD;
using namespace IGC::Debug;
using namespace IGC;


namespace TC
{

extern bool ProcessElfInput(
  STB_TranslateInputArgs &InputArgs,
  STB_TranslateOutputArgs &OutputArgs,
  IGC::OpenCLProgramContext &Context,
  PLATFORM &platform, bool isOutputLlvmBinary);

extern bool ParseInput(
  llvm::Module*& pKernelModule,
  const STB_TranslateInputArgs* pInputArgs,
  STB_TranslateOutputArgs* pOutputArgs,
  IGC::OpenCLProgramContext &oclContext,
  TB_DATA_FORMAT inputDataFormatTemp);

bool TranslateBuild(
    const STB_TranslateInputArgs* pInputArgs,
    STB_TranslateOutputArgs* pOutputArgs,
    TB_DATA_FORMAT inputDataFormatTemp,
    const IGC::CPlatform& IGCPlatform,
    float profilingTimerResolution);

bool CIGCTranslationBlock::ProcessElfInput(
  STB_TranslateInputArgs &InputArgs,
  STB_TranslateOutputArgs &OutputArgs,
  IGC::OpenCLProgramContext &Context){
    return TC::ProcessElfInput(InputArgs, OutputArgs, Context, m_Platform, m_DataFormatOutput == TB_DATA_FORMAT_LLVM_BINARY);
}

CIGCTranslationBlock::CIGCTranslationBlock()
{

}

CIGCTranslationBlock::~CIGCTranslationBlock()
{

}

static void SetErrorMessage(const std::string & ErrorMessage, STB_TranslateOutputArgs & pOutputArgs)
{
    pOutputArgs.pErrorString = new char[ErrorMessage.size() + 1];
    memcpy_s(pOutputArgs.pErrorString, ErrorMessage.size() + 1, ErrorMessage.c_str(), ErrorMessage.size() + 1);
    pOutputArgs.ErrorStringSize = ErrorMessage.size() + 1;
}

bool CIGCTranslationBlock::Create(
    const STB_CreateArgs* pCreateArgs,
    CIGCTranslationBlock* &pTranslationBlock )
{
    bool    success = true;

    pTranslationBlock = new CIGCTranslationBlock();

    if( pTranslationBlock )
    {
        success = pTranslationBlock->Initialize(pCreateArgs );

        if( !success )
        {
            CIGCTranslationBlock::Delete( pTranslationBlock );
        }
    }
    else
    {
        success = false;
    }

    return success;
}

void CIGCTranslationBlock::Delete(
    CIGCTranslationBlock* &pTranslationBlock )
{
    delete pTranslationBlock;
    pTranslationBlock = NULL;
}

bool CIGCTranslationBlock::Translate(
    const STB_TranslateInputArgs* pInputArgs,
    STB_TranslateOutputArgs* pOutputArgs )
{
    LoadRegistryKeys();

    // Create a copy of input arguments that can be modified
    STB_TranslateInputArgs InputArgsCopy = *pInputArgs;

    IGC::CPlatform IGCPlatform(m_Platform);

    SUscGTSystemInfo gtSystemInfo = { 0 };
    gtSystemInfo.EUCount = m_SysInfo.EUCount;
    gtSystemInfo.ThreadCount = m_SysInfo.ThreadCount;
    gtSystemInfo.SliceCount = m_SysInfo.SliceCount;
    gtSystemInfo.SubSliceCount = m_SysInfo.SubSliceCount;
    gtSystemInfo.IsDynamicallyPopulated = m_SysInfo.IsDynamicallyPopulated;
    gtSystemInfo.TotalVsThreads = m_SysInfo.TotalVsThreads;
    gtSystemInfo.TotalPsThreadsWindowerRange = m_SysInfo.TotalPsThreadsWindowerRange;
    gtSystemInfo.TotalDsThreads = m_SysInfo.TotalDsThreads;
    gtSystemInfo.TotalGsThreads = m_SysInfo.TotalGsThreads;
    gtSystemInfo.TotalHsThreads = m_SysInfo.TotalHsThreads;
    gtSystemInfo.MaxEuPerSubSlice = m_SysInfo.MaxEuPerSubSlice;
    gtSystemInfo.EuCountPerPoolMax = m_SysInfo.EuCountPerPoolMax;

    IGC::SetGTSystemInfo(&gtSystemInfo, &IGCPlatform);
    IGC::SetWorkaroundTable(&m_SkuTable, &IGCPlatform);
    IGC::SetCompilerCaps(&m_SkuTable, &IGCPlatform);

    pOutputArgs->pOutput = nullptr;
    pOutputArgs->OutputSize = 0;
    pOutputArgs->pErrorString = nullptr;
    pOutputArgs->ErrorStringSize = 0;
    pOutputArgs->pDebugData = nullptr;
    pOutputArgs->DebugDataSize = 0;

    if (m_DataFormatInput == TB_DATA_FORMAT_ELF)
    {
        // Handle TB_DATA_FORMAT_ELF input as a result of a call to
        // clLinkLibrary(). There are two possible scenarios, link input
        // to form a new library (BC module) or link input to form an
        // executable.

        // First, link input modules together
        USC::SShaderStageBTLayout zeroLayout = USC::g_cZeroShaderStageBTLayout;
        IGC::COCLBTILayout oclLayout(&zeroLayout);
        CDriverInfoOCLNEO driverInfo;
        IGC::OpenCLProgramContext oclContextTemp(oclLayout, IGCPlatform, &InputArgsCopy, driverInfo, nullptr,
                                                 m_DataFormatOutput == TC::TB_DATA_FORMAT_NON_COHERENT_DEVICE_BINARY);
        RegisterComputeErrHandlers(*oclContextTemp.getLLVMContext());
        bool success = ProcessElfInput(InputArgsCopy, *pOutputArgs, oclContextTemp);

        return success;
    }

    if ((m_DataFormatInput == TB_DATA_FORMAT_LLVM_TEXT) ||
        (m_DataFormatInput == TB_DATA_FORMAT_SPIR_V) ||
        (m_DataFormatInput == TB_DATA_FORMAT_LLVM_BINARY))
    {
        return TC::TranslateBuild(&InputArgsCopy, pOutputArgs, m_DataFormatInput, IGCPlatform, m_ProfilingTimerResolution);
    }
    else
    {
        IGC_ASSERT_MESSAGE(0, "Unsupported input format");
        return false;
    }
    return false;
}

std::unordered_map<uint32_t, uint64_t> UnpackSpecConstants(
    const uint32_t* pSpecConstantsIds,
    const uint64_t* pSpecConstantsValues,
    uint32_t size)
{
    std::unordered_map<uint32_t, uint64_t> outSpecConstantsMap;
    for (uint32_t i = 0; i < size; i++)
    {
        outSpecConstantsMap[pSpecConstantsIds[i]] = pSpecConstantsValues[i];
    }
    return outSpecConstantsMap;
}

void GenerateCompilerOptionsMD(llvm::LLVMContext &C, llvm::Module &M, llvm::StringRef options)
{
    llvm::SmallVector<llvm::StringRef, 8> flags;
    llvm::StringRef sep(" ");
    options.split(flags, sep, -1, false);

    std::vector<llvm::Metadata*> ValueVec;
    for (auto flag : flags) {
        flag = flag.trim();
        flag = flag.rtrim(0);  // make sure no ending 0
        // flags : C string (ended with 0)
        if (!flag.empty() && flag.front() != 0)
            ValueVec.push_back(llvm::MDString::get(C, flag));
    }
    llvm::NamedMDNode* NamedMD = M.getOrInsertNamedMetadata("opencl.compiler.options");
    NamedMD->addOperand(llvm::MDNode::get(C, ValueVec));
}

// Dump shader (binary or text), to output directory.
// Create directory if it doesn't exist.
// Works for all OSes.
// ext - file name suffix (optional) and extension.
void DumpShaderFile(
    const std::string& dstDir,
    const char* pBuffer,
    const UINT bufferSize,
    const QWORD hash,
    const std::string& ext)
{
    if (pBuffer && bufferSize > 0)
    {
        std::ostringstream fullPath(dstDir, std::ostringstream::ate);
        fullPath << "OCL_asm"
            << std::hex
            << std::setfill('0')
            << std::setw(sizeof(hash) * CHAR_BIT / 4)
            << hash
            << std::dec
            << std::setfill(' ')
            << ext;

        FILE* pFile = NULL;
        fopen_s(&pFile, fullPath.str().c_str(), "wb");
        if (pFile)
        {
            fwrite(pBuffer, 1, bufferSize, pFile);
            fclose(pFile);
        }
    }
}

#if defined(IGC_SPIRV_TOOLS_ENABLED)
spv_result_t DisassembleSPIRV(const char* pBuffer, UINT bufferSize, spv_text* outSpirvAsm)
{
    const spv_target_env target_env = SPV_ENV_UNIVERSAL_1_3;
    spv_context context = spvContextCreate(target_env);
    const uint32_t* const binary = reinterpret_cast<const uint32_t*> (pBuffer);
    const size_t word_count = (bufferSize / sizeof(uint32_t));
    const uint32_t options = (SPV_BINARY_TO_TEXT_OPTION_FRIENDLY_NAMES | SPV_BINARY_TO_TEXT_OPTION_INDENT | SPV_BINARY_TO_TEXT_OPTION_SHOW_BYTE_OFFSET);
    spv_diagnostic diagnostic = nullptr;

    const spv_result_t result = spvBinaryToText(
        context,
        binary,
        word_count,
        options,
        outSpirvAsm,
        &diagnostic);

    spvContextDestroy(context);
    spvDiagnosticDestroy(diagnostic);
    return result;
}
#endif // defined(IGC_SPIRV_TOOLS_ENABLED)

bool ProcessElfInput(
  STB_TranslateInputArgs &InputArgs,
  STB_TranslateOutputArgs &OutputArgs,
  IGC::OpenCLProgramContext &Context,
  PLATFORM &platform,
  bool isOutputLlvmBinary)
{
  bool success = true;
  std::string ErrorMsg;

    CLElfLib::CElfReader *pElfReader = CLElfLib::CElfReader::Create(InputArgs.pInput, InputArgs.InputSize);
    CLElfLib::RAIIElf X(pElfReader); // When going out of scope this object calls the Delete() function automatically

    // If input buffer is an ELF file, then process separately
    const CLElfLib::SElf64Header* pHeader = pElfReader->GetElfHeader();
    if (pHeader != NULL)
    {
      // Create an empty module to store the output
      std::unique_ptr<llvm::Module> OutputModule;

      // Iterate over all the input modules.
      for (unsigned i = 1; i < pHeader->NumSectionHeaderEntries; i++)
      {
        const CLElfLib::SElf64SectionHeader* pSectionHeader = pElfReader->GetSectionHeader(i);
        IGC_ASSERT(pSectionHeader != NULL);

        char* pData = NULL;
        size_t dataSize = 0;

        if (pSectionHeader->Type == CLElfLib::SH_TYPE_SPIRV_SC_IDS)
        {
            pElfReader->GetSectionData(i, pData, dataSize);
            InputArgs.pSpecConstantsIds = reinterpret_cast<const uint32_t*>(pData);
            InputArgs.SpecConstantsSize = static_cast<uint32_t>(dataSize / sizeof(uint32_t));
        }

        if (pSectionHeader->Type == CLElfLib::SH_TYPE_SPIRV_SC_VALUES)
        {
            pElfReader->GetSectionData(i, pData, dataSize);
            InputArgs.pSpecConstantsValues = reinterpret_cast<const uint64_t*>(pData);
        }

        if ((pSectionHeader->Type == CLElfLib::SH_TYPE_OPENCL_LLVM_BINARY)  ||
            (pSectionHeader->Type == CLElfLib::SH_TYPE_OPENCL_LLVM_ARCHIVE) ||
            (pSectionHeader->Type == CLElfLib::SH_TYPE_SPIRV))
        {
          pElfReader->GetSectionData(i, pData, dataSize);

          // Create input module from the buffer
          llvm::StringRef buf(pData, dataSize);

          std::unique_ptr<llvm::Module> InputModule = nullptr;

          if (pSectionHeader->Type == CLElfLib::SH_TYPE_SPIRV)
          {
              llvm::Module* pKernelModule = nullptr;
#if defined(IGC_SPIRV_ENABLED)
              Context.setAsSPIRV();
              std::istringstream IS(buf);
              std::string stringErrMsg;
              std::unordered_map<uint32_t, uint64_t> specIDToSpecValueMap = UnpackSpecConstants(
                                                                                  InputArgs.pSpecConstantsIds,
                                                                                  InputArgs.pSpecConstantsValues,
                                                                                  InputArgs.SpecConstantsSize);
              bool success = spv::ReadSPIRV(*Context.getLLVMContext(), IS, pKernelModule, stringErrMsg, &specIDToSpecValueMap);
              // handle OpenCL Compiler Options
              GenerateCompilerOptionsMD(
                  *Context.getLLVMContext(),
                  *pKernelModule,
                  llvm::StringRef(InputArgs.pOptions, InputArgs.OptionsSize));
#else
              std::string stringErrMsg{ "SPIRV consumption not enabled for the TARGET." };
              bool success = false;
#endif
              // unset specialization constants, to avoid using them by subsequent SPIR-V modules
              InputArgs.pSpecConstantsIds = nullptr;
              InputArgs.pSpecConstantsValues = nullptr;
              InputArgs.SpecConstantsSize = 0;

              if (success)
              {
                  InputModule.reset(pKernelModule);
              }
          }
          else
          {
              std::unique_ptr<llvm::MemoryBuffer> pInputBuffer =
                  llvm::MemoryBuffer::getMemBuffer(buf, "", false);

              llvm::Expected<std::unique_ptr<llvm::Module>> errorOrModule =
                    llvm::parseBitcodeFile(pInputBuffer->getMemBufferRef(), *Context.getLLVMContext());
              if (llvm::Error EC = errorOrModule.takeError())
              {
                  std::string errMsg;
                  llvm::handleAllErrors(std::move(EC), [&](llvm::ErrorInfoBase &EIB) {
                      llvm::SMDiagnostic(pInputBuffer->getBufferIdentifier(), llvm::SourceMgr::DK_Error,
                          EIB.message());
                  });
                  IGC_ASSERT_MESSAGE(errMsg.empty(), "parsing bitcode failed");
              }

              InputModule = std::move(errorOrModule.get());
          }

          if (InputModule.get() == NULL)
          {
              success = false;
              break;
          }

          // Link modules
          if (OutputModule.get() == NULL)
          {
              InputModule.swap(OutputModule);
          }
          else
          {
              success = !llvm::Linker::linkModules(*OutputModule, std::move(InputModule));
          }

          if (!success)
          {
              break;
          }
        }
      }

      if (success == true)
      {
        // Now that the output modules are linked the resulting module needs to be
        // serialized out
        std::string OutputString;
        llvm::raw_string_ostream OStream(OutputString);
        IGCLLVM::WriteBitcodeToFile(OutputModule.get(), OStream);
        OStream.flush();

        // Create a copy of the string to return to the caller. The output type
        // determines how the buffer gets managed
        char *pBufResult = static_cast<char*>(operator new(OutputString.size(), std::nothrow));
        if (pBufResult != NULL)
        {
          memcpy_s(pBufResult, OutputString.size(), OutputString.c_str(), OutputString.size());

          if (isOutputLlvmBinary)
          {
            // The buffer is returned to the runtime. When the buffer is not
            // needed anymore the runtime ir responsible to call the module for
            // destroying it
            OutputArgs.OutputSize = OutputString.size();
            OutputArgs.pOutput = pBufResult;

#if defined(IGC_SPIRV_ENABLED)
            if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
            {
                // Dumping SPIRV files needs to be done after linking process, as we need to know
                // the output parameters to prepare the correct file hash
                for (unsigned i = 1; i < pHeader->NumSectionHeaderEntries; i++)
                {
                    const CLElfLib::SElf64SectionHeader* pSectionHeader = pElfReader->GetSectionHeader(i);
                    IGC_ASSERT(pSectionHeader != NULL);
                    if (pSectionHeader->Type == CLElfLib::SH_TYPE_SPIRV)
                    {
                        char* pSPIRVBitcode = NULL;
                        size_t size = 0;
                        pElfReader->GetSectionData(i, pSPIRVBitcode, size);
                        QWORD hash = ShaderHashOCL((const UINT*)OutputArgs.pOutput, OutputArgs.OutputSize / 4).getAsmHash();

                        // beyond of general hash, each SPIR-V module needs to have it's own hash
                        QWORD spvHash = ShaderHashOCL((const UINT*)pSPIRVBitcode, size / 4).getAsmHash();
                        std::ostringstream spvHashSuffix("_", std::ostringstream::ate);
                        spvHashSuffix << std::hex << std::setfill('0') << std::setw(sizeof(spvHash)* CHAR_BIT / 4) << spvHash;
                        const std::string suffix = spvHashSuffix.str();

                        const char* pOutputFolder = IGC::Debug::GetShaderOutputFolder();
                        DumpShaderFile(pOutputFolder, pSPIRVBitcode, size, hash, suffix + ".spv");

#if defined(IGC_SPIRV_TOOLS_ENABLED)
                        spv_text spirvAsm = nullptr;
                        if (DisassembleSPIRV(pSPIRVBitcode, size, &spirvAsm) == SPV_SUCCESS)
                        {
                            DumpShaderFile(pOutputFolder, spirvAsm->str, spirvAsm->length, hash, suffix + ".spvasm");
                        }
                        spvTextDestroy(spirvAsm);
#endif // defined(IGC_SPIRV_TOOLS_ENABLED)
                    }
                }
            }
#endif // defined(IGC_SPIRV_ENABLED)
          }
          else
          {
            IGC_ASSERT_MESSAGE(0, "Unrecognized output format when processing ELF input");
            success = false;
          }
        }
        else
        {
          success = false;
        }

        if (success == true)
        {
          // if -dump-opt-llvm is enabled dump the llvm output to the file
          std::string options = "";
          if((InputArgs.pOptions != nullptr) && (InputArgs.OptionsSize > 0)){
              options.append(InputArgs.pOptions, InputArgs.pOptions + InputArgs.OptionsSize);
          }
          size_t dumpOptPosition = options.find("-dump-opt-llvm");
          if (dumpOptPosition != std::string::npos)
          {
            std::string dumpFileName;
            std::istringstream iss(options.substr(dumpOptPosition));
            iss >> dumpFileName;
            size_t equalSignPosition = dumpFileName.find('=');
            if (equalSignPosition != std::string::npos)
            {
              dumpFileName = dumpFileName.substr(equalSignPosition + 1);
              // dump the buffer
              FILE* file = fopen(dumpFileName.c_str(), "wb");
              if (file != NULL)
              {
                fwrite(pBufResult, OutputString.size(), 1, file);
                fclose(file);
              }
            }
            else
            {
              std::string errorString = "\nWarning: File name not specified with the -dump-opt-llvm option.\n";
              SetErrorMessage(errorString, OutputArgs);
            }
          }
        }
      }
    }

    success = true;

  return success;
}

bool ParseInput(
    llvm::Module*& pKernelModule,
    const STB_TranslateInputArgs* pInputArgs,
    STB_TranslateOutputArgs* pOutputArgs,
    llvm::LLVMContext &oclContext,
    TB_DATA_FORMAT inputDataFormatTemp)
{
    pKernelModule = nullptr;

    // Parse the module we want to compile
    llvm::SMDiagnostic err;
    // For text IR, we don't need the null terminator
    size_t inputSize = pInputArgs->InputSize;

    if (inputDataFormatTemp == TB_DATA_FORMAT_LLVM_TEXT)
    {
        const char* input_ptr = pInputArgs->pInput; //shortcut
        inputSize = std::find(input_ptr, input_ptr + inputSize, 0) - input_ptr;
    }
    llvm::StringRef strInput = llvm::StringRef(pInputArgs->pInput, inputSize);

    // IGC does not handle legacy ocl binary for now (legacy ocl binary
    // is the binary that contains text LLVM IR (2.7 or 3.0).
    if (strInput.size() > 1 && !(strInput[0] == 'B' && strInput[1] == 'C'))
    {
        bool isLLVM27IR = false, isLLVM30IR = false;

        if (strInput.find("triple = \"GHAL3D") != llvm::StringRef::npos)
        {
            isLLVM27IR = true;
        }
        else if ((strInput.find("triple = \"IGIL") != llvm::StringRef::npos) ||
            (strInput.find("metadata !\"image_access_qualifier\"") != llvm::StringRef::npos))
        {
            isLLVM30IR = true;
        }

        if (isLLVM27IR || isLLVM30IR)
        {
            SetErrorMessage("Old LLVM IR (possibly from legacy binary) :  not supported!", *pOutputArgs);
            return false;
        }
    }

    if (inputDataFormatTemp == TB_DATA_FORMAT_LLVM_BINARY) {
        std::unique_ptr<llvm::MemoryBuffer> Buf =
            llvm::MemoryBuffer::getMemBuffer(strInput, "<origin>", false);
        llvm::Expected<std::unique_ptr<llvm::Module>> MOE =
            llvm::parseBitcodeFile(Buf->getMemBufferRef(), oclContext);
        if (llvm::Error E = MOE.takeError())
        {
            llvm::handleAllErrors(std::move(E), [&](llvm::ErrorInfoBase &EIB) {
              err = llvm::SMDiagnostic(Buf->getBufferIdentifier(), llvm::SourceMgr::DK_Error,
                                 EIB.message());
            });
        }
        else
        {
            // the MemoryBuffer becomes owned by the module and does not need to be managed
            pKernelModule = MOE->release();
        }
    }
    else if (inputDataFormatTemp == TB_DATA_FORMAT_SPIR_V) {
#if defined(IGC_SPIRV_ENABLED)
        //convert SPIR-V binary to LLVM module
        std::istringstream IS(strInput);
        std::string stringErrMsg;
        std::unordered_map<uint32_t, uint64_t> specIDToSpecValueMap = UnpackSpecConstants(
                                                                            pInputArgs->pSpecConstantsIds,
                                                                            pInputArgs->pSpecConstantsValues,
                                                                            pInputArgs->SpecConstantsSize);
        bool success = spv::ReadSPIRV(oclContext, IS, pKernelModule, stringErrMsg, &specIDToSpecValueMap);
        // handle OpenCL Compiler Options
        GenerateCompilerOptionsMD(
            oclContext,
            *pKernelModule,
            llvm::StringRef(pInputArgs->pOptions, pInputArgs->OptionsSize));
#else
        std::string stringErrMsg{"SPIRV consumption not enabled for the TARGET."};
        bool success = false;
#endif
        if (!success)
        {
            IGC_ASSERT(false && stringErrMsg.c_str());
        }
    }
    else
    {
        // NOTE:
        //  llvm::parseIR routine expects input buffer to be zero-terminated,
        //  otherwise we trigger an assertion fail during parseAssemblyInto (from MemoryBuffer::init)
        //  (see llvm/src/lib/Support/MemoryBuffer.cpp).
        pKernelModule = llvm::parseIR({ std::string(strInput.begin(), strInput.size()), "" },
                                      err, oclContext).release();
    };
    if (pKernelModule == nullptr)
    {
        err.print(nullptr, llvm::errs(), false);
        IGC_ASSERT_MESSAGE(0, "Parsing module failed!");
    }
    if (pKernelModule == nullptr)
    {
        SetErrorMessage("Parsing llvm module failed!", *pOutputArgs);
        return false;
    }

    return true;
}

#if defined(IGC_SPIRV_ENABLED)
bool ReadSpecConstantsFromSPIRV(std::istream &IS, std::vector<std::pair<uint32_t, uint32_t>> &OutSCInfo)
{
    using namespace spv;

    std::unique_ptr<SPIRVModule> BM(SPIRVModule::createSPIRVModule());
    IS >> *BM;

    auto SPV = BM->parseSpecConstants();

    for (auto& SC : SPV)
    {
        SPIRVType *type = SC->getType();
        uint32_t spec_size = type->isTypeBool() ? 1 : type->getBitWidth() / 8;

        if (SC->hasDecorate(DecorationSpecId))
        {
            SPIRVWord spec_id = *SC->getDecorate(DecorationSpecId).begin();
            Op OP = SC->getOpCode();
            if (OP == OpSpecConstant ||
                OP == OpSpecConstantFalse ||
                OP == OpSpecConstantTrue)
            {
                OutSCInfo.push_back(std::make_pair(spec_id, spec_size));
            }
            else
            {
                IGC_ASSERT_MESSAGE(0, "Wrong instruction opcode, shouldn't be here!");
                return false;
            }
        }
    }
    return true;
}
#endif

void overrideOCLProgramBinary(OpenCLProgramContext &Ctx, char *&binaryOutput, int &binarySize)
{
    auto name = DumpName(IGC::Debug::GetShaderOutputName())
        .Hash(Ctx.hash)
        .Type(ShaderType::OPENCL_SHADER)
        .Extension("progbin");

    std::string Path = name.overridePath();

    std::ifstream f(Path, std::ios::binary);
    if (!f.is_open())
        return;

    appendToShaderOverrideLogFile(Path, "OVERRIDDEN: ");

    f.seekg(0, f.end);
    int newBinarySize = (int)f.tellg();
    f.seekg(0, f.beg);

    char *newBinaryOutput = new char[newBinarySize];
    f.read(newBinaryOutput, newBinarySize);

    IGC_ASSERT_MESSAGE(f.good(), "Not fully read!");

    delete[] binaryOutput;
    binaryOutput = newBinaryOutput;
    binarySize = newBinarySize;
}

void dumpOCLProgramBinary(OpenCLProgramContext &Ctx, char *binaryOutput, int binarySize)
{
#if LLVM_VERSION_MAJOR >= 7
    auto name = DumpName(IGC::Debug::GetShaderOutputName())
        .Hash(Ctx.hash)
        .Type(ShaderType::OPENCL_SHADER)
        .Extension("progbin");

    std::error_code EC;
    llvm::raw_fd_ostream f(name.str(), EC);

    if (!EC)
        f.write(binaryOutput, binarySize);
#endif
}

#if !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)
static std::error_code TranslateBuildVC(
    const STB_TranslateInputArgs* pInputArgs,
    STB_TranslateOutputArgs* pOutputArgs, TB_DATA_FORMAT inputDataFormatTemp,
    const IGC::CPlatform& IGCPlatform, float profilingTimerResolution);
#endif //  !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)

#if !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)
struct VcPayloadInfo {
    bool IsValid = false;
    uint64_t VcOptsOffset = 0;
    uint64_t IrSize = 0;
};
VcPayloadInfo tryExtractPayload(const char* pInput, size_t inputSize)
{

    // Payload format:
    // |-vc-codegen|c-str llvm-opts|i64(IR size)|i64(Payload size)|-vc-codegen|
    //
    // Should be in sync with:
    //  Source/IGC/AdaptorOCL/ocl_igc_interface/impl/fcl_ocl_translation_ctx_impl.cpp

    // Check for availability of "-vc-codegen" marker at the end
    const std::string CodegenMarker = "-vc-codegen";
    // make sure that we also have a room for 2 i64 size items
    if (inputSize < (CodegenMarker.size() + 2 * sizeof(uint64_t)))
        return {};
    const char* const pInputEnd = pInput + inputSize;
    if (std::memcmp(pInputEnd - CodegenMarker.size(),
                    CodegenMarker.data(), CodegenMarker.size()) != 0)
        return {};

    // Read IR and Payload sizes. We already ensured that we have the room
    uint64_t IrSize;
    uint64_t PayloadSize;
    const char* pIrSizeBuff = pInputEnd - CodegenMarker.size() - 2 * sizeof(uint64_t);
    const char* pPayloadSizeBuff = pInputEnd - CodegenMarker.size() - 1 * sizeof(uint64_t);
    memcpy_s(&IrSize, sizeof(IrSize), pIrSizeBuff, sizeof(IrSize));
    memcpy_s(&PayloadSize, sizeof(PayloadSize), pPayloadSizeBuff,
             sizeof(PayloadSize));
    if (inputSize != (PayloadSize + IrSize))
        return {};

    // Search for the start of payload, it should start with "-vc-codegen" marker
    const char* const pIREnd = pInputEnd - PayloadSize;
    if (std::memcmp(pIREnd, CodegenMarker.data(), CodegenMarker.size()) != 0)
        return {};

    // Make sure that we have a zero-terminated c-string (vc-options are encoded as such)
    auto NullPos = std::find(pIREnd, pInputEnd, 0);
    if (NullPos == pInputEnd)
        return {};
    // Consistency check, see the Payload format
    if ((NullPos + 1) != pIrSizeBuff)
        return {};

    VcPayloadInfo Result;
    Result.VcOptsOffset = (pIREnd + CodegenMarker.size()) - pInput;
    Result.IrSize = IrSize;
    Result.IsValid = true;

    return Result;
}
#endif //  !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)

static std::unique_ptr<llvm::MemoryBuffer> GetGenericModuleBuffer() {
    char Resource[5] = {'-'};
    _snprintf(Resource, sizeof(Resource), "#%d", OCL_BC);
    return std::unique_ptr<llvm::MemoryBuffer>{llvm::LoadBufferFromResource(Resource, "BC")};
}

bool TranslateBuild(
    const STB_TranslateInputArgs* pInputArgs,
    STB_TranslateOutputArgs* pOutputArgs,
    TB_DATA_FORMAT inputDataFormatTemp,
    const IGC::CPlatform& IGCPlatform,
    float profilingTimerResolution)
{
#if !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)
    if (pInputArgs->pOptions) {
        std::error_code Status =
            TranslateBuildVC(pInputArgs, pOutputArgs, inputDataFormatTemp,
                             IGCPlatform, profilingTimerResolution);
        if (!Status)
            return true;
        // If vc codegen option was not specified, then vc was not called.
        if (static_cast<vc::errc>(Status.value()) != vc::errc::not_vc_codegen)
            return false;
    }
#endif // !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)

    // Disable code sinking in instruction combining.
    // This is a workaround for a performance issue caused by code sinking
    // that is being done in LLVM's instcombine pass.
    // This code will be removed once sinking is removed from instcombine.
    auto optionsMap = llvm::cl::getRegisteredOptions();
    llvm::StringRef instCombineFlag = "-instcombine-code-sinking=0";
    auto instCombineSinkingSwitch = optionsMap.find(instCombineFlag.trim("-=0"));
    if (instCombineSinkingSwitch != optionsMap.end()) {
      if ((*instCombineSinkingSwitch).getValue()->getNumOccurrences() == 0) {
        const char* args[] = { "igc", instCombineFlag.data() };
        llvm::cl::ParseCommandLineOptions(sizeof(args) / sizeof(args[0]), args);
      }
    }

    if (IGC_IS_FLAG_ENABLED(QualityMetricsEnable))
    {
        IGC::Debug::SetDebugFlag(IGC::Debug::DebugFlag::SHADER_QUALITY_METRICS, true);
    }

    MEM_USAGERESET;

    // Parse the module we want to compile
    llvm::Module* pKernelModule = nullptr;
    LLVMContextWrapper* llvmContext = new LLVMContextWrapper;
    RegisterComputeErrHandlers(*llvmContext);

    ShaderHash inputShHash = ShaderHashOCL((const UINT*)pInputArgs->pInput, pInputArgs->InputSize / 4);

    if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
    {
        const char *pOutputFolder = IGC::Debug::GetShaderOutputFolder();
        QWORD hash = inputShHash.getAsmHash();

        if (inputDataFormatTemp == TB_DATA_FORMAT_LLVM_BINARY)
        {
            DumpShaderFile(pOutputFolder, (char *)pInputArgs->pInput, pInputArgs->InputSize, hash, ".bc");
        }
        else if (inputDataFormatTemp == TB_DATA_FORMAT_SPIR_V)
        {
            DumpShaderFile(pOutputFolder, (char *)pInputArgs->pInput, pInputArgs->InputSize, hash, ".spv");
#if defined(IGC_SPIRV_TOOLS_ENABLED)
            spv_text spirvAsm = nullptr;
            if (DisassembleSPIRV(pInputArgs->pInput, pInputArgs->InputSize, &spirvAsm) == SPV_SUCCESS)
            {
                DumpShaderFile(pOutputFolder, spirvAsm->str, spirvAsm->length, hash, ".spvasm");
            }
            spvTextDestroy(spirvAsm);
#endif // defined(IGC_SPIRV_TOOLS_ENABLED)
        }

        DumpShaderFile(pOutputFolder, (char *)pInputArgs->pInternalOptions, pInputArgs->InternalOptionsSize, hash, "_internal_options.txt");
        DumpShaderFile(pOutputFolder, (char *)pInputArgs->pOptions, pInputArgs->OptionsSize, hash, "_options.txt");
    }

    if (!ParseInput(pKernelModule, pInputArgs, pOutputArgs, *llvmContext, inputDataFormatTemp))
    {
        return false;
    }
    CDriverInfoOCLNEO driverInfoOCL;
    IGC::CDriverInfo* driverInfo = &driverInfoOCL;

    USC::SShaderStageBTLayout zeroLayout = USC::g_cZeroShaderStageBTLayout;
    IGC::COCLBTILayout oclLayout(&zeroLayout);
    OpenCLProgramContext oclContext(oclLayout, IGCPlatform, pInputArgs, *driverInfo, llvmContext);

#ifdef __GNUC__
    // Get rid of "the address of 'oclContext' will never be NULL" warning
#pragma GCC diagnostic push
#pragma GCC ignored "-Waddress"
#endif // __GNUC__
    COMPILER_TIME_INIT(&oclContext, m_compilerTimeStats);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__

    COMPILER_TIME_START(&oclContext, TIME_TOTAL);
    oclContext.m_ProfilingTimerResolution = profilingTimerResolution;

    if(inputDataFormatTemp == TB_DATA_FORMAT_SPIR_V)
    {
        oclContext.setAsSPIRV();
    }

    if (IGC_IS_FLAG_ENABLED(EnableReadGTPinInput))
    {
        // Set GTPin flags
        oclContext.gtpin_init = pInputArgs->GTPinInput;
    }

    oclContext.setModule(pKernelModule);
    if (oclContext.isSPIRV())
    {
        deserialize(*oclContext.getModuleMetaData(), pKernelModule);
    }

    oclContext.hash = inputShHash;
    oclContext.annotater = nullptr;

    // Set default denorm.
    // Note that those values have been set to FLOAT_DENORM_FLUSH_TO_ZERO
    if (IGFX_GEN8_CORE <= oclContext.platform.GetPlatformFamily())
    {
        oclContext.m_floatDenormMode16 = FLOAT_DENORM_RETAIN;
        oclContext.m_floatDenormMode32 = FLOAT_DENORM_RETAIN;
        oclContext.m_floatDenormMode64 = FLOAT_DENORM_RETAIN;
    }

    unsigned PtrSzInBits = pKernelModule->getDataLayout().getPointerSizeInBits();
    //TODO: Again, this should not happen on each compilation

    /// set retry manager
    bool retry = false;
    oclContext.m_retryManager.Enable();
    do
    {
        std::unique_ptr<llvm::Module> BuiltinGenericModule = nullptr;
        std::unique_ptr<llvm::Module> BuiltinSizeModule = nullptr;
        std::unique_ptr<llvm::MemoryBuffer> pGenericBuffer = nullptr;
        std::unique_ptr<llvm::MemoryBuffer> pSizeTBuffer = nullptr;
        {
            // IGC has two BIF Modules:
            //            1. kernel Module (pKernelModule)
            //            2. BIF Modules:
            //                 a) generic Module (BuiltinGenericModule)
            //                 b) size Module (BuiltinSizeModule)
            //
            // OCL builtin types, such as clk_event_t/queue_t, etc., are struct (opaque) types. For
            // those types, its original names are themselves; the derived names are ones with
            // '.<digit>' appended to the original names. For example,  clk_event_t is the original
            // name, its derived names are clk_event_t.0, clk_event_t.1, etc.
            //
            // When llvm reads in multiple modules, say, M0, M1, under the same llvmcontext, if both
            // M0 and M1 has the same struct type,  M0 will have the original name and M1 the derived
            // name for that type.  For example, clk_event_t,  M0 will have clk_event_t, while M1 will
            // have clk_event_t.2 (number is arbitary). After linking, those two named types should be
            // mapped to the same type, otherwise, we could have type-mismatch (for example, OCL GAS
            // builtin_functions tests will assertion fail during inlining due to type-mismatch).  Furthermore,
            // when linking M1 into M0 (M0 : dstModule, M1 : srcModule), the final type is the type
            // used in M0.

            // Load the builtin module -  Generic BC
            // Load the builtin module -  Generic BC
            {
                COMPILER_TIME_START(&oclContext, TIME_OCL_LazyBiFLoading);

                pGenericBuffer = GetGenericModuleBuffer();

                if (pGenericBuffer == NULL)
                {
                    SetErrorMessage("Error loading the Generic builtin resource", *pOutputArgs);
                    return false;
                }

                llvm::Expected<std::unique_ptr<llvm::Module>> ModuleOrErr =
                    getLazyBitcodeModule(pGenericBuffer->getMemBufferRef(), *oclContext.getLLVMContext());

                if (llvm::Error EC = ModuleOrErr.takeError())
                {
                    std::string error_str = "Error lazily loading bitcode for generic builtins,"
                                            "is bitcode the right version and correctly formed?";
                    SetErrorMessage(error_str, *pOutputArgs);
                    return false;
                }
                else
                {
                    BuiltinGenericModule = std::move(*ModuleOrErr);
                }

                if (BuiltinGenericModule == NULL)
                {
                    SetErrorMessage("Error loading the Generic builtin module from buffer", *pOutputArgs);
                    return false;
                }
                COMPILER_TIME_END(&oclContext, TIME_OCL_LazyBiFLoading);
            }

            // Load the builtin module -  pointer depended
            {
                char ResNumber[5] = { '-' };
                switch (PtrSzInBits)
                {
                case 32:
                    _snprintf(ResNumber, sizeof(ResNumber), "#%d", OCL_BC_32);
                    break;
                case 64:
                    _snprintf(ResNumber, sizeof(ResNumber), "#%d", OCL_BC_64);
                    break;
                default:
                    IGC_ASSERT_MESSAGE(0, "Unknown bitness of compiled module");
                }

                // the MemoryBuffer becomes owned by the module and does not need to be managed
                pSizeTBuffer.reset(llvm::LoadBufferFromResource(ResNumber, "BC"));
                IGC_ASSERT_MESSAGE(pSizeTBuffer, "Error loading builtin resource");

                llvm::Expected<std::unique_ptr<llvm::Module>> ModuleOrErr =
                    getLazyBitcodeModule(pSizeTBuffer->getMemBufferRef(), *oclContext.getLLVMContext());
                if (llvm::Error EC = ModuleOrErr.takeError())
                    IGC_ASSERT_MESSAGE(0, "Error lazily loading bitcode for size_t builtins");
                else
                    BuiltinSizeModule = std::move(*ModuleOrErr);

                IGC_ASSERT_MESSAGE(BuiltinSizeModule, "Error loading builtin module from buffer");
            }

            BuiltinGenericModule->setDataLayout(BuiltinSizeModule->getDataLayout());
            BuiltinGenericModule->setTargetTriple(BuiltinSizeModule->getTargetTriple());
        }

        oclContext.getModuleMetaData()->csInfo.forcedSIMDSize |= IGC_GET_FLAG_VALUE(ForceOCLSIMDWidth);

        if (llvm::StringRef(oclContext.getModule()->getTargetTriple()).startswith("spir"))
        {
            IGC::UnifyIRSPIR(&oclContext, std::move(BuiltinGenericModule), std::move(BuiltinSizeModule));
        }
        else // not SPIR
        {
            IGC::UnifyIROCL(&oclContext, std::move(BuiltinGenericModule), std::move(BuiltinSizeModule));
        }

        if (!(oclContext.oclErrorMessage.empty()))
        {
             //The error buffer returned will be deleted when the module is unloaded so
             //a copy is necessary
            if (const char *pErrorMsg = oclContext.oclErrorMessage.c_str())
            {
                SetErrorMessage(oclContext.oclErrorMessage, *pOutputArgs);
            }
            return false;
        }

        // Compiler Options information available after unification.
        ModuleMetaData *modMD = oclContext.getModuleMetaData();
        if (modMD->compOpt.DenormsAreZero)
        {
            oclContext.m_floatDenormMode16 = FLOAT_DENORM_FLUSH_TO_ZERO;
            oclContext.m_floatDenormMode32 = FLOAT_DENORM_FLUSH_TO_ZERO;
        }

        // Optimize the IR. This happens once for each program, not per-kernel.
        IGC::OptimizeIR(&oclContext);

        // Now, perform code generation
        IGC::CodeGen(&oclContext);

        retry = (oclContext.m_retryManager.AdvanceState() &&
                !oclContext.m_retryManager.kernelSet.empty());

        if (retry)
        {
            oclContext.clear();

            // Create a new LLVMContext
            oclContext.initLLVMContextWrapper();

            IGC::Debug::RegisterComputeErrHandlers(*oclContext.getLLVMContext());

            if (!ParseInput(pKernelModule, pInputArgs, pOutputArgs, *oclContext.getLLVMContext(), inputDataFormatTemp))
            {
                return false;
            }
            oclContext.setModule(pKernelModule);
        }
    } while (retry);

    if (!(oclContext.oclErrorMessage.empty()))
    {
        //The error buffer returned will be deleted when the module is unloaded so
        //a copy is necessary
        if (const char* pErrorMsg = oclContext.oclErrorMessage.c_str())
        {
            SetErrorMessage(oclContext.oclErrorMessage, *pOutputArgs);
        }
        return false;
    }

    // Prepare and set program binary
    unsigned int pointerSizeInBytes = (PtrSzInBits == 64) ? 8 : 4;

    // FIXME: zebin currently only support program output itself, will add debug info
    // into it
    int binarySize = 0;
    char* binaryOutput = nullptr;

    if (!IGC_IS_FLAG_ENABLED(EnableZEBinary) &&
        !oclContext.getModuleMetaData()->compOpt.EnableZEBinary) {
        Util::BinaryStream programBinary;
        // Patch token based binary format
        oclContext.m_programOutput.CreateKernelBinaries();
        oclContext.m_programOutput.GetProgramBinary(programBinary, pointerSizeInBytes);
        binarySize = static_cast<int>(programBinary.Size());
        binaryOutput = new char[binarySize];
        memcpy_s(binaryOutput, binarySize, (char*)programBinary.GetLinearPointer(), binarySize);
    } else {
        // ze binary foramt
        llvm::SmallVector<char, 64> buf;
        llvm::raw_svector_ostream llvm_os(buf);
        const char* spv_data = nullptr;
        uint32_t spv_size = 0;
        if (inputDataFormatTemp == TB_DATA_FORMAT_SPIR_V) {
            spv_data = pInputArgs->pInput;
            spv_size = pInputArgs->InputSize;
        }
        oclContext.m_programOutput.GetZEBinary(llvm_os, pointerSizeInBytes,
            spv_data, spv_size);

        // FIXME: try to avoid memory copy here
        binarySize = buf.size();
        binaryOutput = new char[binarySize];
        memcpy_s(binaryOutput, binarySize, buf.data(), buf.size());
    }

    if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
        dumpOCLProgramBinary(oclContext, binaryOutput, binarySize);

    if (IGC_IS_FLAG_ENABLED(ShaderOverride))
        overrideOCLProgramBinary(oclContext, binaryOutput, binarySize);

    pOutputArgs->OutputSize = binarySize;
    pOutputArgs->pOutput = binaryOutput;

    // Prepare and set program debug data
    Util::BinaryStream programDebugData;
    oclContext.m_programOutput.GetProgramDebugData(programDebugData);

    int debugDataSize = int_cast<int>(programDebugData.Size());
    if (debugDataSize > 0)
    {
        char* debugDataOutput = new char[debugDataSize];
        memcpy_s(debugDataOutput, debugDataSize, (char*)programDebugData.GetLinearPointer(), debugDataSize);

        pOutputArgs->DebugDataSize = debugDataSize;
        pOutputArgs->pDebugData = debugDataOutput;
    }

    const char* driverName =
        GTPIN_DRIVERVERSION_OPEN;
    // If GT-Pin is enabled, instrument the binary. Finally pOutputArgs will
    // be pointing to the instrumented binary with the new size.
    if (GTPIN_IGC_OCL_IsEnabled())
    {
        const GEN_ISA_TYPE genIsa = GTPIN_IGC_OCL_GetGenIsaFromPlatform(IGCPlatform.getPlatformInfo());
        int instrumentedBinarySize = 0;
        void* instrumentedBinaryOutput = NULL;
        GTPIN_IGC_OCL_Instrument(genIsa, driverName,
            binarySize, binaryOutput,
            instrumentedBinarySize, instrumentedBinaryOutput);

        void* newBuffer = operator new[](instrumentedBinarySize, std::nothrow);
        memcpy_s(newBuffer, instrumentedBinarySize, instrumentedBinaryOutput, instrumentedBinarySize);
        pOutputArgs->OutputSize = instrumentedBinarySize;
        pOutputArgs->pOutput = (char*)newBuffer;

        if (binaryOutput != nullptr)
        {
            delete[] binaryOutput;
        }
    }

    COMPILER_TIME_END(&oclContext, TIME_TOTAL);

    COMPILER_TIME_PRINT(&oclContext, ShaderType::OPENCL_SHADER, oclContext.hash);

    COMPILER_TIME_DEL(&oclContext, m_compilerTimeStats);

    return true;
}

bool CIGCTranslationBlock::FreeAllocations(
    STB_TranslateOutputArgs* pOutputArgs)
{
    delete [] pOutputArgs->pOutput;
    return true;
}

bool CIGCTranslationBlock::Initialize(
    const STB_CreateArgs* pCreateArgs)
{
    const SGlobalData* pCreateArgsGlobalData =
                  (const SGlobalData*)pCreateArgs->pCreateData;

    // IGC maintains its own WA table - ignore the version in the global arguments.
    m_Platform = *pCreateArgsGlobalData->pPlatform;
    m_SkuTable = *pCreateArgsGlobalData->pSkuTable;
    m_SysInfo  = *pCreateArgsGlobalData->pSysInfo;

    m_DataFormatInput  = pCreateArgs->TranslationCode.Type.Input;
    m_DataFormatOutput = pCreateArgs->TranslationCode.Type.Output;

    m_ProfilingTimerResolution = pCreateArgsGlobalData->ProfilingTimerResolution;

    bool validTBChain = false;

    auto isDeviceBinaryFormat = [] (TB_DATA_FORMAT format)
    {
        return (format == TB_DATA_FORMAT_DEVICE_BINARY)
            || (format == TB_DATA_FORMAT_COHERENT_DEVICE_BINARY)
            || (format == TB_DATA_FORMAT_NON_COHERENT_DEVICE_BINARY);
    };

    validTBChain |=
        (m_DataFormatInput == TB_DATA_FORMAT_ELF) &&
        (m_DataFormatOutput == TB_DATA_FORMAT_LLVM_BINARY);

    validTBChain |=
        (m_DataFormatInput == TB_DATA_FORMAT_LLVM_TEXT) &&
        isDeviceBinaryFormat(m_DataFormatOutput);

    validTBChain |=
        (m_DataFormatInput == TB_DATA_FORMAT_LLVM_BINARY) &&
        isDeviceBinaryFormat(m_DataFormatOutput);

    validTBChain |=
        (m_DataFormatInput == TB_DATA_FORMAT_SPIR_V) &&
        isDeviceBinaryFormat(m_DataFormatOutput);

    IGC_ASSERT_MESSAGE(validTBChain, "Invalid TB Chain");

    return validTBChain;
}

static const STB_TranslationCode g_cICBETranslationCodes[] =
{
    { { TB_DATA_FORMAT_ELF,           TB_DATA_FORMAT_LLVM_BINARY   } },
    { { TB_DATA_FORMAT_LLVM_TEXT,     TB_DATA_FORMAT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_LLVM_BINARY,   TB_DATA_FORMAT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_SPIR_V,        TB_DATA_FORMAT_DEVICE_BINARY } },

    { { TB_DATA_FORMAT_LLVM_TEXT,     TB_DATA_FORMAT_COHERENT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_LLVM_BINARY,   TB_DATA_FORMAT_COHERENT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_SPIR_V,        TB_DATA_FORMAT_COHERENT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_LLVM_TEXT,     TB_DATA_FORMAT_NON_COHERENT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_LLVM_BINARY,   TB_DATA_FORMAT_NON_COHERENT_DEVICE_BINARY } },
    { { TB_DATA_FORMAT_SPIR_V,        TB_DATA_FORMAT_NON_COHERENT_DEVICE_BINARY } }
};

TRANSLATION_BLOCK_API void Register(
    STB_RegisterArgs* pRegisterArgs)
{
    pRegisterArgs->Version = TC::STB_VERSION;

    if(pRegisterArgs->pTranslationCodes == NULL)
    {
        pRegisterArgs->NumTranslationCodes =
            sizeof(g_cICBETranslationCodes ) /
            sizeof(g_cICBETranslationCodes[0]);
    }
    else
    {
        pRegisterArgs->NumTranslationCodes =
            sizeof(g_cICBETranslationCodes) /
            sizeof(g_cICBETranslationCodes[0]);

        iSTD::MemCopy<sizeof(g_cICBETranslationCodes)>(
            pRegisterArgs->pTranslationCodes,
            g_cICBETranslationCodes);
    }
}

TRANSLATION_BLOCK_API CTranslationBlock* Create(
    STB_CreateArgs* pCreateArgs)
{
    CIGCTranslationBlock*  pIGCTranslationBlock = nullptr;

    CIGCTranslationBlock::Create(
        pCreateArgs,
        pIGCTranslationBlock);

    return pIGCTranslationBlock;
}

TRANSLATION_BLOCK_API void Delete(
    CTranslationBlock* pTranslationBlock)
{
    CIGCTranslationBlock*  pIGCTranslationBlock =
        (CIGCTranslationBlock*)pTranslationBlock;

    CIGCTranslationBlock::Delete(pIGCTranslationBlock);
}

#if !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)

static void adjustPlatformVC(const IGC::CPlatform& IGCPlatform,
                             vc::CompileOptions& Opts)
{
    Opts.CPUStr = cmc::getPlatformStr(IGCPlatform.getPlatformInfo());
    Opts.WATable = std::make_unique<WA_TABLE>(IGCPlatform.getWATable());
}

static void adjustFileTypeVC(TB_DATA_FORMAT DataFormat,
                             vc::CompileOptions& Opts)
{
    switch (DataFormat)
    {
    case TB_DATA_FORMAT::TB_DATA_FORMAT_SPIR_V:
        Opts.FType = vc::FileType::SPIRV;
        return;
    default:
        llvm_unreachable("Data format is not supported yet");
    }
}

static void adjustOptLevelVC(vc::CompileOptions& Opts)
{
    if (IGC_IS_FLAG_ENABLED(VCOptimizeNone))
        Opts.OptLevel = vc::OptimizerLevel::None;
}

// Overwrite binary format option for backward compatibility with
// environment variable approach.
static void adjustBinaryFormat(vc::BinaryKind &Binary)
{
    if (Binary == vc::BinaryKind::OpenCL &&
        IGC_IS_FLAG_ENABLED(EnableZEBinary))
        Binary = vc::BinaryKind::ZE;
}

static void adjustDumpOptions(vc::CompileOptions& Opts)
{
    if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
    {
        Opts.DumpIR = true;
        Opts.DumpIsa = true;
        Opts.DumpAsm = true;
    }
}

static void adjustOptionsVC(const IGC::CPlatform& IGCPlatform,
                            TB_DATA_FORMAT DataFormat, vc::CompileOptions& Opts)
{
    adjustPlatformVC(IGCPlatform, Opts);
    adjustFileTypeVC(DataFormat, Opts);
    adjustOptLevelVC(Opts);
    adjustBinaryFormat(Opts.Binary);
    adjustDumpOptions(Opts);
}

static std::error_code getErrorVC(llvm::Error Err,
                                  STB_TranslateOutputArgs* pOutputArgs)
{
    std::error_code Status;
    llvm::handleAllErrors(
        std::move(Err), [&Status, pOutputArgs](const llvm::ErrorInfoBase& EI) {
            Status = EI.convertToErrorCode();
            // Some tests check for build log when everything is ok.
            // So let's not even try to touch things if we were not called.
            if (static_cast<vc::errc>(Status.value()) == vc::errc::not_vc_codegen)
              return;
            SetErrorMessage(EI.message(), *pOutputArgs);
        });
    return Status;
}

static std::error_code getErrorVC(std::error_code Err,
                                  STB_TranslateOutputArgs* pOutputArgs)
{
    SetErrorMessage(Err.message(), *pOutputArgs);
    return Err;
}

static void outputBinaryVC(llvm::StringRef Binary,
                           llvm::StringRef DebugInfo,
                           STB_TranslateOutputArgs* pOutputArgs)
{
    size_t BinarySize = Binary.size();
    char* pBinaryOutput = new char[BinarySize];
    memcpy_s(pBinaryOutput, BinarySize, Binary.data(), BinarySize);
    pOutputArgs->OutputSize = static_cast<uint32_t>(BinarySize);
    pOutputArgs->pOutput = pBinaryOutput;
    if (DebugInfo.size())
    {
        char* pDebugInfo = new char[DebugInfo.size()];
        memcpy_s(pDebugInfo, DebugInfo.size(), DebugInfo.data(), DebugInfo.size());
        pOutputArgs->pDebugData = pDebugInfo;
        pOutputArgs->DebugDataSize = DebugInfo.size();
    }
}

// Similar to ShaderHashOCL though reinterpretation is hidden inside
// iStdLib so probably it will be safer (to use more specialized things).
static ShaderHash GetShaderHash(llvm::ArrayRef<char> Input)
{
    ShaderHash Hash;
    Hash.asmHash = iSTD::HashFromBuffer(Input.data(), Input.size());
    return Hash;
}

static std::error_code TranslateBuildVC(
    const STB_TranslateInputArgs* pInputArgs,
    STB_TranslateOutputArgs* pOutputArgs, TB_DATA_FORMAT inputDataFormatTemp,
    const IGC::CPlatform& IGCPlatform, float profilingTimerResolution)
{
    llvm::StringRef ApiOptions{pInputArgs->pOptions, pInputArgs->OptionsSize};
    llvm::StringRef InternalOptions{pInputArgs->pInternalOptions,
                                    pInputArgs->InternalOptionsSize};
    llvm::ArrayRef<char> Input{pInputArgs->pInput, pInputArgs->InputSize};

    std::unique_ptr<vc::ShaderDumper> Dumper;
    if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
    {
        const ShaderHash Hash = GetShaderHash(Input);
        Dumper = vc::createVC_IGCFileDumper(Hash);
    }
    else
    {
        Dumper = vc::createDefaultShaderDumper();
    }

    if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
    {
        Dumper->dumpText(ApiOptions, "options_raw.txt");
        Dumper->dumpText(InternalOptions, "internal_options_raw.txt");
        Dumper->dumpBinary(Input, "igc_input_raw.spv");
    }

    auto NewPathPayload = tryExtractPayload(Input.data(), Input.size());
    if (NewPathPayload.IsValid) {
        ApiOptions = "-vc-codegen";
        InternalOptions = pInputArgs->pInput + NewPathPayload.VcOptsOffset;
        Input = Input.take_front(static_cast<size_t>(NewPathPayload.IrSize));
    }

    if (IGC_IS_FLAG_ENABLED(ShaderDumpEnable))
    {
        Dumper->dumpText(ApiOptions, "options.txt");
        Dumper->dumpText(InternalOptions, "internal_options.txt");
        Dumper->dumpBinary(Input, "igc_input.spv");
    }

    auto ExpOptions = vc::ParseOptions(ApiOptions, InternalOptions);
    if (!ExpOptions)
        return getErrorVC(ExpOptions.takeError(), pOutputArgs);

    // Reset options when everything is done here.
    // This is needed to not interfere with subsequent translations.
    const auto ClOptGuard =
        llvm::make_scope_exit([]() { llvm::cl::ResetAllOptionOccurrences(); });

    vc::CompileOptions& Opts = ExpOptions.get();
    adjustOptionsVC(IGCPlatform, inputDataFormatTemp, Opts);

    Opts.Dumper = std::move(Dumper);

    std::unique_ptr<llvm::MemoryBuffer> OCLGenericBIFModule = GetGenericModuleBuffer();
    if (!OCLGenericBIFModule)
        return getErrorVC(vc::make_error_code(vc::errc::generic_bif_load_fail),
                          pOutputArgs);
    vc::ExternalData ExtData{std::move(OCLGenericBIFModule)};
    llvm::ArrayRef<uint32_t> SpecConstIds{pInputArgs->pSpecConstantsIds,
                                          pInputArgs->SpecConstantsSize};
    llvm::ArrayRef<uint64_t> SpecConstValues{pInputArgs->pSpecConstantsValues,
                                             pInputArgs->SpecConstantsSize};
    auto ExpOutput =
        vc::Compile(Input, Opts, ExtData, SpecConstIds, SpecConstValues);
    if (!ExpOutput)
        return getErrorVC(ExpOutput.takeError(), pOutputArgs);
    vc::CompileOutput& Res = ExpOutput.get();

    switch (Opts.Binary)
    {
    case vc::BinaryKind::CM:
    {
        auto &CompileResult = std::get<vc::cm::CompileOutput>(Res);
        outputBinaryVC(CompileResult.IsaBinary, llvm::StringRef(), pOutputArgs);
        break;
    }
    case vc::BinaryKind::OpenCL:
    {
        auto &CompileResult = std::get<vc::ocl::CompileOutput>(Res);
        iOpenCL::CGen8CMProgram CMProgram{IGCPlatform.getPlatformInfo()};
        vc::createBinary(CMProgram, CompileResult.Kernels);
        CMProgram.CreateKernelBinaries();
        Util::BinaryStream ProgramBinary;
        CMProgram.GetProgramBinary(ProgramBinary,
                                   CompileResult.PointerSizeInBytes);
        llvm::StringRef BinaryRef{ProgramBinary.GetLinearPointer(),
            static_cast<std::size_t>(ProgramBinary.Size())};

        Util::BinaryStream ProgramDebugData;
        CMProgram.GetProgramDebugData(ProgramDebugData);
        llvm::StringRef DebugInfoRef{ProgramDebugData.GetLinearPointer(),
            static_cast<std::size_t>(ProgramDebugData.Size())};

        outputBinaryVC(BinaryRef, DebugInfoRef, pOutputArgs);
        break;
    }
    case vc::BinaryKind::ZE:
    {
        auto &CompileResult = std::get<vc::ocl::CompileOutput>(Res);
        iOpenCL::CGen8CMProgram CMProgram{IGCPlatform.getPlatformInfo()};
        vc::createBinary(CMProgram, CompileResult.Kernels);
        llvm::SmallVector<char, 0> ProgramBinary;
        llvm::raw_svector_ostream ProgramBinaryOS{ProgramBinary};
        CMProgram.GetZEBinary(ProgramBinaryOS, CompileResult.PointerSizeInBytes);
        llvm::StringRef BinaryRef{ProgramBinary.data(), ProgramBinary.size()};

        Util::BinaryStream ProgramDebugData;
        CMProgram.GetProgramDebugData(ProgramDebugData);
        llvm::StringRef DebugInfoRef{ProgramDebugData.GetLinearPointer(),
            static_cast<std::size_t>(ProgramDebugData.Size())};

        outputBinaryVC(BinaryRef, DebugInfoRef, pOutputArgs);
        break;
    }
    }

    return {};
}
#endif // !defined(WDDM_LINUX) && (!defined(IGC_VC_DISABLED) || !IGC_VC_DISABLED)

} // namespace TC
