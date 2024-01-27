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

## Example

Extract the temperature data from the R-JPEG image, use [sharp](https://www.npmjs.com/package/sharp) package to generate a UInt16 TIFF image, and then decode the TIFF image using [tiff](https://www.npmjs.com/package/tiff) package.

```javascript
const { getTemperatureData } = require("dji-thermal-sdk");
const sharp = require("sharp");
const { decode } = require("tiff");
const fs = require("fs");

const imageBuffer = fs.readFileSync("DJI_0001_R.jpg");

const { width, height, parameters, data } = getTemperatureData(imageBuffer);

console.log(parameters);

console.log(data);

//create a sharp image from the temperature data
// /sharp accepts raw image data (Uint16Array), so we need to specify the width, height and channels of the image.
const sharpImage = sharp(data, { raw: { width, height, channels: 1 } }); 

sharpImage
  .toColorspace("grey16") //convert the image to 16-bit grayscale (1 channel)
  .tiff({
    compression: "none", //no compression
  })
  .toBuffer() //get the buffer of the TIFF image
  .then((buffer) => {
    fs.writeFileSync("test.tiff", buffer); //write the buffer to a file
 
    const tiffBuffer = fs.readFileSync("test.tiff"); //read the TIFF image from the file

    const [image] = decode(tiffBuffer); //decode the TIFF image

    console.log(image.data); //print the temperature data of the TIFF image
  });
```

## TODO

- Support `Windows` platforms.
- Export typescript definition files.
- Add methods to get the temperature data as float array.
- Set thermal parameters to override the parameters in the image, before getting the temperature data.

## References

- [DJI Thermal SDK](https://www.dji.com/cn/downloads/softwares/dji-thermal-sdk) The DJI Thermal SDK enables you to process R-JPEG (Radiometric JPEG) images which were captured by DJI infrared camera products.
