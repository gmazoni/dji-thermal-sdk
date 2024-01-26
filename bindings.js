const thermalModule = require('./build/Release/djiThermal.node');

/**
 * Module representing the DJI Thermal functionality.
 * @namespace DJIThermal
 */
const DJIThermal = {};

/**
 * The DJI Thermal module.
 * @namespace DJIThermal.thermalModule
 * @type {Object}
 */
DJIThermal.thermalModule = thermalModule;

/**
 * Retrieves the version of the DJI Thermal module.
 * @function
 * @memberof DJIThermal.thermalModule
 * @name version
 * @returns {string} The version of the DJI Thermal module.
 */
DJIThermal.version = () => thermalModule.version();

/**
 * Retrieves the temperature data from the image buffer.
 * @function
 * @memberof DJIThermal.thermalModule
 * @name getTemperatureData
 * @param {Buffer} imageBuffer - The image buffer from which to retrieve the temperature data
 * @returns {Object} An object representing the data retrieved from the image buffer.
 * @property {number} width
 * @property {number} height
 * @property {Object} parameters
 * @property {number} parameters.distance
 * @property {number} parameters.humidity
 * @property {number} parameters.emissivity
 * @property {number} parameters.reflection
 * @property {Array<number>} data - An array containing the temperature data.
 */
DJIThermal.getTemperatureData = (imageBuffer) => thermalModule.getTemperatureData(imageBuffer);

/**
 * Module exports containing functions related to DJI Thermal.
 * @namespace module.exports
 * @type {Object}
 */
module.exports = DJIThermal;
