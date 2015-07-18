/**
 * Created by Dexter on 2015/2/2.
 */
var net = require('net');
var fs = require('fs');

var HOST = '127.0.0.1';
var PORT = 6868;

var client = new net.Socket();
client.connect(PORT, HOST, function() {

    console.log('CONNECTED TO: ' + HOST + ':' + PORT);

    var rs = fs.createReadStream('f://hls.txt');
    var data = '';
    rs.on('data', function(chunk){
        data += chunk;
    })
    rs.on('end', function () {
        var len = data.length;
        var lenStr = len.toString();
        console.log(lenStr.length);
        for (var index = 8; index > lenStr.length; index--) {
            lenStr += " ";
        }

        var sendMessage = "JSON" + lenStr + '  ' + data;
        client.write(sendMessage);
    })



    //client.destroy();

});

// 为客户端添加“data”事件处理函数
// data是服务器发回的数据
client.on('data', function(data) {

    console.log('DATA: ' + data);
    // 完全关闭连接
    client.destroy();

});

// 为客户端添加“close”事件处理函数
client.on('close', function() {
    console.log('Connection closed');
});
