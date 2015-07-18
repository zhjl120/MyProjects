/**
 * Created by Dexter on 2015/2/5.
 */

var logger   = require('./_log.js').logger;
var helper   = require('./_lib_helper.js');
var MongoDB    = require('./_mongodb.js');

module.exports = DBOperate;

var mongo = new MongoDB();

function DBOperate(){

    this.logOn = function(mac, cb){
        var deviceInfo = mongo.getDeviceInfoModel();
        deviceInfo.update({mac : mac}, {$set : {online:222}}, function(err,docs){
            if(err) cb(err);
            if(docs)    cb(null, 'success');
            else    cb(err);
        });
    }

    this.isRegister = function(mac, cb){
        var deviceInfo = mongo.getDeviceInfoModel();
        deviceInfo.findOne({mac : mac}, function (err, docs) {
            if(err) cb(err);
            if(docs)    cb(null, true);
            else    cb(null, false);
        });
    }

    this.logOff= function(mac){     //未计算在线时长
        var curTime = helper.getCurrentTime(1);
        var deviceInfo = mongo.getDeviceInfoModel();
        deviceInfo.update({mac:mac},{$set : {recent_offline:curTime, online:0}}, function(err, docs){
            if(err) logger.error('logoff error :' + err.message);
        });
    }

    this.getAreaIdByMac = function(mac){

        return '';
    }

    this.getMaclistByAreaid = function(areaId){
        return '';
    }
}
