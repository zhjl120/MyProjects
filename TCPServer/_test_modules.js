/**
 * Created by Dexter on 2015/2/5.
 */

var LogOn 				= require('./_logon.js');
var LogOff 				= require('./_logoff.js');
var ContentInsert 		= require('./_content_insert.js');
var CancelInsert 		= require('./_cancel_insert.js');
var CaptureScreenshot 	= require('./_capture_screenshot.js');
var KeepAlive           = require('./_keep_alive.js');
var DeviceInfo 			= require('./_device_info.js');
var logger              = require('./_log.js').logger;


exports.logonTest = function(){
    var request = {
        "type":"request",
        "cmd":"log on",
        "parameters":
        {
            "id":"00-22-F4-CE-C8-50",
            "recently request":"2013/11/3 16:35:39"
        }
    }

    var logOn = new LogOn();
    logOn.handleRequest(request, null);
}

exports.logoffTest = function(){
    var request = {
        "type":"request",
        "cmd":"log off",
        "parameters":null
    }
    var mac = '00-22-F4-CE-C8-50';

    var logOff = new LogOff();
    logOff.handleRequest(request, null, mac);
}

exports.captureScreenshotTest = function(){
    var request = {
        "type":"request",
        "cmd":"capture screenshot",
        "parameters": {
            "idArray":
                [
                    "00-22-F4-CE-C8-50"
                ]
        }
    }
    var captureScreenshot = new CaptureScreenshot();
    captureScreenshot.handleRequest(request);
}

exports.contentInsertTest = function(){
    var request = {
        "type":"request",
        "cmd":"content embed",
        "parameters":
        {
            "resource type":"video",
            "content":"33",
            "area id":["1","2","3","4"],
            "start date":"2014/09/12 11:44:32",
            "end date":"2014/09/12 13:44:00"
        }
    }
    var contentInsert = new ContentInsert();
    contentInsert.handleRequest(request);
}