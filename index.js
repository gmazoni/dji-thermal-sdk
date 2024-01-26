const { version, getTemperatureData } = require('./');

const sharp = require('sharp')
const { decode } = require('tiff')
const fs = require('fs')

const imageBuffer = fs.readFileSync('test.jpg')

console.log(imageBuffer)

console.time('version')
console.log(version())
console.timeEnd('version')

console.time('getTemperatureData')
const { width, height, parameters, data } = getTemperatureData(imageBuffer)
console.timeEnd('getTemperatureData')

const buffer = Buffer.from(data)
// Create a Sharp image from the buffer
const sharpImage = sharp(buffer, { raw: { width, height, channels: 1 } });

sharpImage.toFile('test.tiff', (err, info) => {
    if (err) {
        console.error(err);
    } else {
        console.log('Image saved:', info);
    }
});
