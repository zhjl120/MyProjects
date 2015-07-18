/**
 * Created by Dexter on 2015/2/5.
 */

var helper    = require('./_lib_helper.js');
var DBOperate   = require('./_dboperate.js');
var clientMap = require('./_client_map.js');

var eventList = new Array();
var pushedEvtlist = new Array();
var dbOperate = new DBOperate();

exports.addEvent = function(event){

    var startTime = event['start date'];
    var endTime = event['end date'];
    var resourceType = event['resource type'];
    var areaList = event['area id'];
    var currTime = helper.getCurrentTime(1);

 /* if(helper.isFromBiggerThanTo(currTime, startTime)){
        return;
    }
 */
    if(helper.isFromBiggerThanTo(currTime, endTime)){
        return;
    }

    for(var evt in eventList){
        if(resourceType == evt['resource type']){
            if(areaList.toString() == evt['area id'].toString())
                return;
        }
    }

    eventList.push(event);
}

exports.openTimer = function(){
    var response = {
        "type":"request",
        "cmd":"content embed",
        "parameters":
        {
            "resource type":"",
            "content":"",
            "playtime":""
        }
    }

    setInterval(function () {
        for(var index in eventList){
            var evt = eventList[index];
            var areaList = evt['area id'];
            var startTime = evt['start date']
            var currTime = helper.getCurrentTime(1);

            if(helper.isFromBiggerThanTo(startTime, currTime)){
                var endTime = evt['end date'];
                var start = new Date(startTime).getTime();
                var end = new Date(endTime).getTime() ;
                var playTime = end - start;

                response.parameters['resource type'] = evt['resource type'];
                response.parameters['content'] = evt['content'];
                response.parameters['playtime'] = playTime;
                var jsonRes = JSON.stringify(response);
                jsonRes += '\n';
                var buffRes = new Buffer(jsonRes, 'utf8');

                var macList = dbOperate.getMaclistByAreaid(areaList);
                for(var x in macList){
                    var sokcet = clientMap.findSocketByMac(x);
                    if(sokcet != null){
                        sokcet.write(buffRes);
                    }
                }

                pushedEvtlist.push(evt);
                eventList.splice(index, 1);
            }
        }

    }, 1000);
}

exports.pushEvent = function(socket, mac){
    var currTime = helper.getCurrentTime(1);
    var response = {
        "type":"request",
        "cmd":"content embed",
        "parameters":
        {
            "resource type":"",
            "content":"",
            "playtime":""
        }
    }

    for(var index in pushedEvtlist){
        var endTime = pushedEvtlist[index]['end date'];
        if(helper.isFromBiggerThanTo(endTime, currTime)){
            pushedEvtlist.splice(index,1);
        }
    }

    var areaId = dbOperate.getAreaIdByMac(mac);
    if(areaId == '') return;

    for(var index in pushedEvtlist){
        var evt = pushedEvtlist[index];
        var areaList = evt['area id'];
        for(var x in areaList){
            if(areaId == x){
                var startTime = evt['start date'];
                var endTime = evt['end date'];
                var start = new Date(startTime).getTime();
                var end = new Date(endTime).getTime() ;
                var playTime = end - start;
                response.parameters['resource type'] = evt['resource type'];
                response.parameters['content'] = evt['content'];
                response.parameters['playtime'] = playTime;
                var jsonRes = JSON.stringify(response);
                jsonRes += '\n';
                var buffRes = new Buffer(jsonRes, 'utf8');
                socket.write(buffRes);
            }
        }
    }
}

exports.removeEvent = function(areaList, resourceType){
    for(var index in eventList){
        var evt = eventList[index];
        if(evt['resource type'] == resourceType &&
           evt['area id'].toString() == areaList.toString()){
                eventList.splice(index, 1);
        }
    }
}

exports.removePushedEvent = function(areaList, resourceType){
    for(var index in pushedEvtlist){
        var evt = pushedEvtlist[index];
        if(evt['resource type'] == resourceType &&
            evt['area id'].toString() == areaList.toString()){
            pushedEvtlist.splice(index, 1);
        }
    }
}
























