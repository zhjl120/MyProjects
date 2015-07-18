/*
* Create by dexter on 15-1-30
*/

var clientMap = require('./_client_map.js');

module.exports = CaptureScreentshot;


function CaptureScreentshot(){

  this.handleRequest = function(jsonObj){
    var response = {
      'type' : 'request',
      'cmd'  : 'capture screenshot',
      'parameters' : null
    };
    var jsonRes = JSON.stringify(response);
    jsonRes += '\n';
    var buffRes = new Buffer(jsonRes, 'utf8');

    var arrMac = jsonObj.parameters.idArray;
    for(var index in arrMac){
      var mac = arrMac[index];
      var socket = clientMap.findSocketByMac(mac);
      if(socket != null)
        socket.wrap(buffRes);
    }
  }
}