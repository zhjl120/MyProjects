/**
 * Created by Dexter on 2015/2/5.
 */



var map = new Array();

exports.addClient = function(mac, socket){
    var date = new Date();
    var member = {
        'mac' : mac,
        'socket' : socket,
        'time' : date.getTime()
    }
    map.push(member);
}

exports.removeClient = function(mac){
    for(var index in map){
        if(mac === map[index].mac){
            map.splice(index,1);
            break;
        }
    }
}

exports.findSocketByMac = function(mac){
    for(var index in map){
        if(mac === map[index].mac)
            return map[index].socket;
    }
    return null;
}

exports.setKeepAlive = function (mac, time) {
    for(var index in map){
        if(mac === map[index].mac){
            map[index].time = time;
            return true;
        }
    }
    return false;
}