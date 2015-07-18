/*
* Create by dexter on 15-1-30
*/

var DBOperate   = require('./_dboperate.js');
var clientMap = require('./_client_map.js');

module.exports = LogOff;


function LogOff(){
  var dbOperate = new DBOperate();

  this.handleRequest = function(jsonObj, socket, mac){
    clientMap.removeClient(mac);
    dbOperate.logOff(mac);
  }
}