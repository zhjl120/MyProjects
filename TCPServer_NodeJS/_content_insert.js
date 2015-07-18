/*
* Create by dexter on 15-1-30
*/

var helper    = require('./_lib_helper.js');
var areaEvent = require('./_area_event.js');

module.exports = ContentInsert;


function ContentInsert(){
  this.handleRequest = function(jsonObj){
    var event = jsonObj.parameters;
    event.isPublish = false;
    areaEvent.addEvent(event);
  }
}