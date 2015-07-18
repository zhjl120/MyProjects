/*
* Create by dexter on 15-1-30
*/

var DBOperate  = require('./_dboperate.js');
var async      = require('async');
var logger     = require('./_log.js').logger;
var clientMap  = require('./_client_map.js');
var areaEvent  = require('./_area_event.js');
module.exports = LogOn;


function LogOn(){
  var dbOperate = new DBOperate();

  this.handleRequest = function(jsonObj, socket){
    var mac = jsonObj.parameters.id;
    var response = {
      'type'  : 'response',
      'cmd'   : 'log on',
      'result': 'fail'
    };

    async.auto({
      isRegister : function(cb){
        dbOperate.isRegister(mac, cb);
      },
      logOn : ['isRegister', function (cb, results) {
        var isReg = results.isRegister;
        if(isReg){
          dbOperate.logOn(mac, cb);
          clientMap.addClient(mac, socket);
          response.result = 'success';
        }
      }]
    }, function (err, results) {
      if(err != null)
        logger.error('async.auto error : ' + err.message + '\nResult: ' + results);
    });

    var jsonRes = JSON.stringify(response);
    jsonRes += '\n';
    var buffRes = new Buffer(jsonRes, 'utf8');
   // socket.write(buffRes);

    areaEvent.pushEvent(socket, mac);
  }
}