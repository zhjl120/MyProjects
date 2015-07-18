/**
 * Created by Dexter on 2015/2/4.
 */


var mongoose = require('mongoose');
var Schema   = require('./_mongoose_schema.js');
var logger   = require('./_log.js').logger;

module.exports =MongoDB;


function MongoDB(){

    var dbUrl = 'mongodb://localhost:27017/mianyang';
    mongoose.connect(dbUrl);

    var deviceSchema = new mongoose.Schema(Schema.deviceInfo);
    var deviceInfoModel = mongoose.model('deviceInfo', deviceSchema, 'device_info');

    this.closeDB = function(){
        mongoose.connection.close(function(){
            logger.info('Mongoose is closed.');
        });
    }

    mongoose.connection.on('connected', function () {
        logger.info('Mongoose connected to ' + dbUrl);
    });

    mongoose.connection.on('error', function(err){
        logger.error('Mongoose connection error: ' + err);
    });

    mongoose.connection.on('disconnected', function(){
        logger.info('Mongoose disconnected');
    })

    this.getDeviceInfoModel = function () {
        return deviceInfoModel;
    }
}

























