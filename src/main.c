/**
 * Entry point for the Node.js addon.
*/

#include <assert.h>
#include <node_api.h>
#include "wimlib.h"
#include "wimlib_tchar.h"

#define TO_PERCENT(numerator, denominator) \
	(((denominator) == 0) ? 0 : ((numerator) * 100 / (denominator)))

imagex_progress_func(enum wimlib_progress_msg msg,
		     union wimlib_progress_info *info,
		     void *_ignored_context)
{
  unsigned percent_done = TO_PERCENT(info->extract.completed_bytes, info->extract.total_bytes);
  tprintf("Progress: %u%%\n", percent_done);
	return WIMLIB_PROGRESS_STATUS_CONTINUE;
}

static napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value return_value;

  uint64_t part_size = 3800 * (1 << 20);
  WIMStruct *wim;

  char source[] = "/Volumes/CCCOMA_X64FRE_EN-US_DV9/sources/install.wim";
  char destination[] = "/Users/kyleupton/Documents/install.swm";

  int ret = wimlib_open_wim_with_progress(source, WIMLIB_OPEN_FLAG_CHECK_INTEGRITY, &wim, imagex_progress_func, NULL);
  if (ret) {
    tprintf("Failed to open WIM\n");
    status = napi_create_string_utf8(env, "failed", 6, &return_value);
    assert(status == napi_ok);
    return return_value;
  }

  ret = wimlib_split(wim, destination, part_size, WIMLIB_WRITE_FLAG_CHECK_INTEGRITY);

  if (ret == 0) {
    tprintf("Splitting complete\n");
    status = napi_create_string_utf8(env, "ok", 2, &return_value);
  } else {
    tprintf("Splitting failed\n");
    status = napi_create_string_utf8(env, "failed", 6, &return_value);
  }

  wimlib_free(wim);

  assert(status == napi_ok);
  return return_value;
}

#define DECLARE_NAPI_METHOD(name, func) { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("split", Method);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
