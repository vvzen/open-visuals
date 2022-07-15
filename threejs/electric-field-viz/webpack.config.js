const path = require('path');
const webpack = require('webpack');

module.exports = {
    entry: ['./src/main.js', './src/core.js' ],
    mode: 'development',
    output: {
        filename: 'bundle.js',
        path: path.resolve(__dirname, 'dist'),
        libraryExport: 'default',
        libraryTarget: 'window'
    },
    experiments: {
        topLevelAwait: true
    }
};
