#include <napi.h>

#include"turbo_linecount.h"
#include<cstring>
#include<cstdio>

#ifdef _WIN32

#include<tchar.h>

#elif defined(TLC_COMPATIBLE_UNIX)

#include<stdlib.h>
#define _tprintf printf
#define _ftprintf fprintf
#define _tcscmp strcmp
#define _tcslen strlen
#define _ttoi atoi
#define _tcstoui64 strtoull
#define _T(x) x
#define TCHAR char

#endif

using namespace TURBOLINECOUNT;

Napi::Value Run(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string arg0 = info[0].As<Napi::String>();
  TCHAR const *filename = arg0.c_str();

  CLineCount::PARAMETERS params;
  params.buffersize = -1;
  params.threadcount = -1;
  params.offsets = true;

  // Create line count class
  CLineCount lc(&params);

  if (!lc.open(filename))
  {
    tlc_error_t err = lc.lastError();
    tlc_string_t errstr = lc.lastErrorString();

    _ftprintf(stderr, _T("Error %d (%s)\n"), err, errstr.c_str());
    return env.Null();
  }

  // Count lines
  tlc_linecount_t count;
  std::vector<std::vector<uint64_t>> lineoffsets;

  if (!lc.countLines(lineoffsets, count, params.offsets))
  {
    tlc_error_t err = lc.lastError();
    tlc_string_t errstr = lc.lastErrorString();

    _ftprintf(stderr, _T("Error %d: (%s)\n"), err, errstr.c_str());
    return env.Null();
  }

  // Display output
  if (!params.offsets) {
    _tprintf(_T(TLC_LINECOUNT_FMT) _T("\n"), count);
  }

  uint64_t totalsize = 0;

  for (size_t i = 0; i < lineoffsets.size(); i++)
  {
    totalsize += sizeof(u_int64_t)*lineoffsets[i].size();

    if (i != 0) {
      lineoffsets[0].insert( lineoffsets[0].end(), lineoffsets[i].begin(), lineoffsets[i].end() );
    }
  }

  auto offsets = Napi::ArrayBuffer::New(env, lineoffsets[0].data(), totalsize);
  auto typedOffsets = Napi::TypedArrayOf<uint64_t>::New(env, (size_t)count, offsets, (size_t)0);
  return typedOffsets;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "run"), Napi::Function::New(env, Run));
  return exports;
}

NODE_API_MODULE(addon, Init)
