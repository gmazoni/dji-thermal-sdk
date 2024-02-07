const { version, getTemperatureData } = require("./bindings");

const fs = require("fs");

const imageBuffer = fs.readFileSync("DJI_0001_R.jpg");


async function main(){
    console.log("version:", version());
    
    const { width, height, parameters, data } = getTemperatureData(imageBuffer);
    
    console.log(width, height, parameters);
    
    console.log(data);
}

main()

