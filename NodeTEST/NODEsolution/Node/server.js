'use strict';
var WebSocketServer = require('websocket').server;
var http = require('http');
var port = process.env.PORT || 1337;

var server = http.createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/plain' });
    res.end('Hello World\n' + res.connection.localPort);
});

server.listen(1337, function () {
    console.log('server is listening on port: ' + port);
});

var wsServer = new WebSocketServer({
    httpServer: server
});

wsServer.on('request', function (request) {
    var connection = request.accept(null, request.origin);
    console.log('client connected');
    connection.on('message', function (message) {
        if (message.type === 'utf8') {
            console.log(message);
        }
    });
});


