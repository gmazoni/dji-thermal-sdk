const djiThermal = require('./'); 

const fs = require('fs')

const imageBuffer = fs.readFileSync('test.jpg')

console.log(imageBuffer)

console.time('version')
console.log(djiThermal.version())
console.timeEnd('version')

console.time('getTiff')
console.log(djiThermal.getTiff(imageBuffer))
console.timeEnd('getTiff')
