/*
* Create by dexter on 15-1-30
*/

var net 				= require('net');
var iconv               = require('iconv-lite');
var config				= require('./_config.js');
var LogOn 				= require('./_logon.js');
var LogOff 				= require('./_logoff.js');
var ContentInsert 		= require('./_content_insert.js');
var CancelInsert 		= require('./_cancel_insert.js');
var CaptureScreenshot 	= require('./_capture_screenshot.js');
var KeepAlive           = require('./_keep_alive.js');
var DeviceInfo 			= require('./_device_info.js');
var logger              = require('./_log.js').logger;
var tester              = require('./_test_modules.js');

var LISTEN_PORT = config.serverConfig.listen_port;
var deviceInfo = new DeviceInfo();


var server = net.createServer(function (socket){
  logger.debug('New connection ---> ' + socket.remoteAddress + ':' + socket.remotePort);

  var allData = {};
  allData.request = '';

  socket.on('data', function(data){
    allData.request += iconv.decode(data, 'utf-8');
    recvData(allData, socket);
  });
  
  socket.on('close', function(){
    logger.debug('Socket is closed');
  });

  socket.on('error', function(data){
    logger.error(data);
  })
});


server.listen(LISTEN_PORT, function(){
  logger.info('Server is listening on '+ LISTEN_PORT);
 // tester.logonTest();
 // tester.logoffTest();
 // tester.captureScreenshotTest();
  tester.contentInsertTest();
});


function recvData(allData, socket){
  while(true) {
    if(allData.request.length < 12)
      return;

    var type = allData.request.substring(0,4);
    var length = parseInt(allData.request.substring(4,12));

    if(type != 'JSON' && type != 'DATA') {
      logger.warn('Bad data : ' + allData.request);
      allData.request = '';
      return;
    }

    if(isNaN(length))
      logger.warn('Bad data : ' + allData.request);

    if(allData.request.length < length + 12)
      return;

    if(type == 'JSON'){
      var jsonRequest = allData.request.substr(12, length);
      var jsonObj = null;
      try{
        jsonObj = JSON.parse(jsonRequest);
        handleFactory(jsonObj, socket);
      }catch (err) {
        logger.warn('Cannot parse json string : ' + allData.request);
      }
    }

    allData.request = allData.request.substr(length + 12);
    logger.trace('Request already handled !');
  }
}


function handleFactory(jsonObj, socket){
  var cmd = jsonObj.cmd;

  if(cmd === 'log on'){
    deviceInfo.setMac(jsonObj.parameters.id);
    var logOn = new LogOn();
    logger.info('Recv logon request from ' + jsonObj.parameters.id);
    logOn.handleRequest(jsonObj, socket);
  }
  else if(cmd === 'log off'){
    var mac = deviceInfo.getMac();
    var logOff = new LogOff();
    logOff.handleRequest(jsonObj, socket, mac);
  }
  else if(cmd === 'content embed'){
    var contentInsert = new ContentInsert();
    contentInsert.handleRequest(jsonObj);
  }
  else if(cmd === 'cancel embed'){
    var cancelInsert = new CancelInsert();
    cancelInsert.handleRequest(jsonObj, socket);
  }
  else if(cmd === 'capture screenshot'){
    logger.info('Recv screenshot request from web.');
    var captureScreenshot = new CaptureScreenshot();
    captureScreenshot.handleRequest(jsonObj);
  }
  else if(cmd === 'keep alive'){
    var keepAlive = new KeepAlive();
    keepAlive.handleRequest(socket, deviceInfo.getMac());
  }
}
