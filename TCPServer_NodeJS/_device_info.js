/*
* Create by dexter on 15-1-30
*/

module.exports = DeviceInfo;


function DeviceInfo(){
  var mac = '';
  
  this.getMac = function(){
    return mac;
  }
  
  this.setMac = function(mac){
    this.mac = mac;
  }
}