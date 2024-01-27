const { version, getTemperatureData } = require("./");

const sharp = require("sharp");
const { decode } = require("tiff");

const fs = require("fs");

const imageBuffer = fs.readFileSync("test.jpg");

console.log(imageBuffer);

console.time("version");
console.log("version:", version());
console.timeEnd("version");

console.time("getTemperatureData");
const { width, height, parameters, data } = getTemperatureData(imageBuffer);
console.timeEnd("getTemperatureData");

console.log(parameters);

console.log(data);

const sharpImage = sharp(data, { raw: { width, height, channels: 1 } });

sharpImage
  .toColorspace("grey16")
  .tiff({
    compression: "none",
  })
  .toBuffer()
  .then((buffer) => {
    fs.writeFileSync("test.tiff", buffer);

    const tiffBuffer = fs.readFileSync("test.tiff");

    const [image] = decode(tiffBuffer);

    console.log(image.data);
  });
