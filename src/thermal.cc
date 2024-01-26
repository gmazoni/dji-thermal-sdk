#include <node.h>
#include <v8.h>
#include <node_buffer.h>

#include "dirp_api.h"

namespace thermal
{

  using namespace v8;

  void Version(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    dirp_api_version_t *versionPointer = new dirp_api_version_t;

    int32_t status = dirp_get_api_version(versionPointer);

    if (status == 0)
    {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, versionPointer->magic).ToLocalChecked());
    }
    else
    {
      args.GetReturnValue().SetNull();
    }
  }

  void GetTiff(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() != 1)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
      return;
    }

    // must be a js buffer
    if (!node::Buffer::HasInstance(args[0]))
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "First argument must be a Buffer").ToLocalChecked()));
      return;
    }

    dirp_set_verbose_level(DIRP_VERBOSE_LEVEL_NONE);

    uint8_t *bufferData = (uint8_t *)node::Buffer::Data(args[0]);
    int32_t bufferLength = node::Buffer::Length(args[0]);

    DIRP_HANDLE *handle = new DIRP_HANDLE;

    int32_t status = dirp_create_from_rjpeg(bufferData, bufferLength, handle);
    if (status != 0)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Failed to create DIRP handle").ToLocalChecked()));
      return;
    }

    dirp_resolution_t *resolution = new dirp_resolution_t;

    status = dirp_get_rjpeg_resolution(*handle, resolution);
    if (status != 0)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Failed to get resolution").ToLocalChecked()));
      return;
    }

    dirp_measurement_params_t *params = new dirp_measurement_params_t;

    status = dirp_get_measurement_params(*handle, params);
    if (status != 0)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Failed to get measurement params").ToLocalChecked()));
      return;
    }

    status = dirp_set_measurement_params(*handle, params);
    if (status != 0)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Failed to set measurement params").ToLocalChecked()));
      return;
    }

    float *temp_image = new float[resolution->width * resolution->height];
    int32_t size = resolution->width * resolution->height * sizeof(float);

    status = dirp_measure_ex(*handle, temp_image, size);
    if (status != 0)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Failed to measure").ToLocalChecked()));
      return;
    }

    int len = resolution->width * resolution->height;

    Local<Array> arr = v8::Array::New(isolate, len);
    for (int i = 0; i < len; i++)
    {
      arr->Set(isolate->GetCurrentContext(), i, Number::New(isolate, temp_image[i]));
    }

    delete[] temp_image;

    dirp_destroy(*handle);

    args.GetReturnValue().Set(arr);
  }

  void Init(Local<Object> exports)
  {
    NODE_SET_METHOD(exports, "version", Version);
    NODE_SET_METHOD(exports, "getTiff", GetTiff);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
}
