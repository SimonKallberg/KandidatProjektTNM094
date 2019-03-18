var express = require('express');
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.use(express.static(__dirname + '/public'));
var user = [];

app.get('/', function (req, res) {
    res.sendFile(__dirname + '/public/Klient.html');
});

io.on('connection', function (socket) {
    var name;
    console.log('a user connected');
    
    socket.emit('getName');

    socket.on('userName', function (name1) {
        name = name1;
        user.push(name);
        console.log(name);
        socket.emit('changeBackground');
    });

    socket.on('message', function (arg) {
        console.log(arg , name);
    });

    socket.on('disconnect', function () {
        user.pop(name);
        console.log(name, ' disconnected', user.length);
    });
});


http.listen(process.env.PORT || 1337, function () {
    console.log('listening on server');
});


//.emit för att skicka
//.on för att ta emot