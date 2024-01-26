const { version, getTemperatureData } = require('./');

const fs = require('fs')

const imageBuffer = fs.readFileSync('test.jpg')

console.log(imageBuffer)

console.time('version')
console.log(version())
console.timeEnd('version')

console.time('getTemperatureData')
console.log(getTemperatureData(imageBuffer))
console.timeEnd('getTemperatureData')
