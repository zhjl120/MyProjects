/*
* Create by dexter on 15-1-30
*/
var helper    = require('./_lib_helper.js');
var clientMap  = require('./_client_map.js');

module.exports = KeepAlive;


function KeepAlive(){
  this.handleRequest = function(socket, mac){
    var response = {
      'type' : "response",
      "cmd": "keep alive",
      "parameters": ''
    };

    var jsonRes = JSON.stringify(response);
    jsonRes += '\n';
    var buffRes = new Buffer(jsonRes, 'utf8');
    socket.write(buffRes);

    var currTime = helper.getCurrentTime(1);
    clientMap.setKeepAlive(mac, currTime);
  }
}