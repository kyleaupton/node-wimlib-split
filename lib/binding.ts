const addon = require('../build/Release/main.node')
export const split = addon.split as () => 'world';
