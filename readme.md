# DJI Thermal SDK

## Notes

- The version of the SDK is `1.4` released on `2022-09-29`.
- Currently, the package only supports `Linux` platforms.
- It uses node-gyp to build the native module.

## Installation

```bash
npm install dji-thermal-sdk
```

## Usage

```javascript
const { version, getTemperatureData } = require("dji-thermal-sdk");

console.log("version:", version()); //print the version of the SDK (magic number)

const fs = require("fs");

const imageBuffer = fs.readFileSync("test.jpg"); //read the R-JPEG image from DJI infrared camera products

const { width, height, parameters, data } = getTemperatureData(imageBuffer);

/*
width: the width of the image
height: the height of the image

parameters: the parameters of the image eg: { distance: 5, humidity: 70, emissivity: 1, reflection: 23 }

data: the temperature data of the image as Uint16Array, each array element represents the temperature in 0.1℃
so the real temperature is the array element divided by 10. eg: 1234 means 123.4℃

*/
```

## TODO

- Support `Windows` platforms.
- Export typescript definition files.
- Add methods to get the temperature data as float array.
- Set thermal parameters to override the parameters in the image, before getting the temperature data.

## References

- [DJI Thermal SDK](https://www.dji.com/cn/downloads/softwares/dji-thermal-sdk) The DJI Thermal SDK enables you to process R-JPEG (Radiometric JPEG) images which were captured by DJI infrared camera products.
