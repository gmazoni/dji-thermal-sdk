#include <node.h>
#include <v8.h>
#include <node_buffer.h>

#include "dirp_api.h"

namespace thermal
{
  using namespace v8;

  class AddonData
  {
  public:
    AddonData(Isolate *isolate, Local<Object> exports) : isolate(isolate)
    {
      this->exports.Reset(isolate, exports);
    }

    ~AddonData()
    {
      this->exports.Reset();
    }

    Isolate *isolate;
    Persistent<Object> exports;
  };

  void Version(const FunctionCallbackInfo<Value> &args)
  {
    AddonData *data = reinterpret_cast<AddonData *>(args.Data().As<External>()->Value());
    auto isolate = data->isolate;

    dirp_api_version_t version;
    if (dirp_get_api_version(&version) == 0)
    {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, version.magic).ToLocalChecked());
    }
    else
    {
      args.GetReturnValue().SetNull();
    }
  }

  void GetTemperatureData(const FunctionCallbackInfo<Value> &args)
  {
    AddonData *data = reinterpret_cast<AddonData *>(args.Data().As<External>()->Value());
    auto isolate = data->isolate;

    if (args.Length() != 1 || !node::Buffer::HasInstance(args[0]))
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Wrong number of arguments or first argument must be a Buffer").ToLocalChecked()));
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

    /*
      status = dirp_set_measurement_params(*handle, params);
      if (status != 0)
      {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Failed to set measurement params").ToLocalChecked()));
        return;
      }
    */

    int16_t *temp_image = new int16_t[resolution->width * resolution->height];
    int32_t size = resolution->width * resolution->height * sizeof(int16_t);

    status = dirp_measure(*handle, temp_image, size);
    if (status != 0)
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Failed to measure").ToLocalChecked()));
      return;
    }

    dirp_destroy(*handle);

    int len = resolution->width * resolution->height;

    Local<Uint16Array> arr = Uint16Array::New(ArrayBuffer::New(isolate, len * sizeof(int16_t)), 0, len);
    for (int i = 0; i < len; i++)
    {
      arr->Set(isolate->GetCurrentContext(), i, Number::New(isolate, temp_image[i]));
    }

    delete[] temp_image;

    Local<Object> parameters = Object::New(isolate);

    parameters->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "distance").ToLocalChecked(), Number::New(isolate, params->distance));
    parameters->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "humidity").ToLocalChecked(), Number::New(isolate, params->humidity));
    parameters->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "emissivity").ToLocalChecked(), Number::New(isolate, params->emissivity));
    parameters->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "reflection").ToLocalChecked(), Number::New(isolate, params->reflection));

    Local<Object> result = Object::New(isolate);

    result->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "width").ToLocalChecked(), Number::New(isolate, resolution->width));
    result->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "height").ToLocalChecked(), Number::New(isolate, resolution->height));
    result->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "parameters").ToLocalChecked(), parameters);
    result->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "data").ToLocalChecked(), arr);

    args.GetReturnValue().Set(result);
  }

  NODE_MODULE_INIT()
  {

    Isolate *isolate = context->GetIsolate();
    AddonData *data = new AddonData(isolate, exports);

    Local<External> external = External::New(isolate, data);

    exports->Set(context,
                 String::NewFromUtf8(isolate, "version").ToLocalChecked(),
                 FunctionTemplate::New(isolate, Version, external)
                     ->GetFunction(context)
                     .ToLocalChecked())
        .FromJust();

    exports->Set(context,
                 String::NewFromUtf8(isolate, "getTemperatureData").ToLocalChecked(),
                 FunctionTemplate::New(isolate, GetTemperatureData, external)
                     ->GetFunction(context)
                     .ToLocalChecked())
        .FromJust();
  }
}
