var express = require('express');
var app = require('express')();
var server = require('http').Server(app);
var WebSocketServer = require('websocket').server;

//Tells the app that the map "Public" is static and is website stuff.
app.use(express.static(__dirname + '/Public'));

var port = process.env.PORT || 1337; port = 80;
var gameAddress = "::ffff:127.0.0.1";
var gameUpdateInterval = 100; //ms

var gameUpdateFunction;
var gameSocket = null;

var connectionArray = [];
var playerArray = [];

//What the website publish
app.get('/', function (req, res) {
    res.sendFile(__dirname + '/public/Klient.html');
});
//create websockertserver
//var wsServer = new webSocketServer.Server({ server });

//Save a port that server is lisening to
server.listen(port, function () {
    console.log('server is listening on: ' + server.address().address + ':' + port);
});

var wsServer = new WebSocketServer({
    httpServer: server
});
/*
wsServer.on('connection', function (socket) {
    //create a local variabel
    var name;
    var number;
    var weapon;
    console.log('a user connected', socket.id);

    socket.on('message', function (arg) {
        if (arg[0] == "info") {
            name = info[1];
            weapon = info[2];

            if (user.length - 1 > 0) {
                number = user[0];
                user.pop(number);
            }

            socket.send(['changeBackground', number]);
            console.log(name, vepon, number);
        }
        //when messege is called from client.
        else if (arg[0] == "message") {
            console.log(arg, name);
        }
        console.log(arg);

     });

    //when client disconnect.
    socket.on('disconnect', function () {
        user.push(number);
        console.log(name, ' disconnected', user.length);
    });

});*/
//when a call for the websocket is made
wsServer.on('request', function (request) {
    console.log('request happening:');
    //check if the call is made from the game
    if (request.remoteAddress === gameAddress) {
        console.log('GAME CONNECTING...');
        gameSocket = request.accept(null, request.origin);

        gameSocket.on('message', function (message) {
            if (message.type === 'utf8' && !gameUpdateFunction) {
                console.log(message.utf8Data);
                gameUpdateFunction = setInterval(updateGameControls, gameUpdateInterval);
            }
        });

        gameSocket.on('close', function (reasonCode, description) {
            console.log('GAMEokej CONNECTION LOST. code: ' + reasonCode + ', desc: ' + description);
            clearInterval(gameUpdateFunction);
            gameSocket = null;
        });

    }
    //check if the websocket is defined
    else if (connectionArray.map(function (c) { return c.socket.remoteAddress; }).indexOf(request.remoteAddress) === -1 || connectionArray.length === 0) {
        var connection = request.accept(null, request.origin);
        var name;
        var weapon;
        var number;
        //check if a player is defiend, if not create player
        if (playerArray.map(function (p) { return p.rAddress; }).indexOf(connection.socket.remoteAddress) === -1) {
            playerArray.push({
                rAddress: connection.socket.remoteAddress,
                name: 'player' + (playerArray.length + 1),
                controls: '0.0 0.0 0.0 0.0'
            });
            if (gameSocket) {
                gameSocket.send('P');
                console.log('notified GAME that player was added');
            }

        }

        //call från websidan
        connection.on('message', function (message) {
            if (message.type === 'utf8') {
                arg = message.utf8Data.split(',');
          
                if ( arg[0]== "info") {
                    name = arg[1];
                    weapon = arg[2];
                    number = playerArray.length + 1;
                    connection.send(['changeBackground', number]);
                    console.log(name, weapon, number, "change person");
                }
                //when messege is called from client playbutton.
                else if (arg[0] == "message") {
                    console.log(arg[1], name);
                }
               
                /* if (message.type === 'utf8') {
                     playerArray[playerArray.map(function (p) { return p.rAddress; }).indexOf(connection.socket.remoteAddress)].controls = message.utf8Data;
                 }*/
                //var index;

                // logControls();
            }
        });
        //when connection closes 
        connection.on('close', function (reasonCode, description) {
            console.log('connection ' + connection.socket.remoteAddress + ' closed. code: ' + reasonCode + ', desc: ' + description);
            connectionArray.splice(connectionArray.map(function (e) { return e.socket.remoteAddress; }).indexOf(connection.socket.remoteAddress), 1);
            logConnections();
        });

        connectionArray.push(connection);
        logConnections();

    } else {
        request.reject(666, 'already a connection to ip');
        console.log('connection duplicate, denied');
    }
});


function logConnections() {
    console.log('LOG:\nclients connected:');
    connectionArray.forEach(function (c) {
        console.log(c.socket.remoteAddress + '    connected?:  ' + c.connected);
        c.send('User at: ' + c.socket.remoteAddress + ' connected.');
    });
    console.log('players:');
    playerArray.forEach(function (u) {
        console.log('rAddress: ' + u.rAddress + ',  name: ' + u.name + ', inputs: ' + u.controls);
    });
    console.log('\n');
}

function logControls() {
    console.log('player controls:');
    playerArray.forEach(function (p) {
        console.log(p.name + ': [ ' + p.controls + ' ]');
    });
}


function updateGameControls() {
    if (playerArray.length > 0) {
        var msg = '';
        playerArray.forEach(function (p) {
            msg += p.controls + '\n';
        });
        gameSocket.send('C\n' + msg);
        //console.log('Sending game:\n' + msg);
    }
}


/*var express = require('express');
var app = require('express')();
var WebSocketServer = require('websocket').server;
var http = require('http').Server(app);
//var io = require('socket.io')(http);
//Tells the app that the map "Public" is static and is website stuff.
app.use(express.static(__dirname + '/Public'));


var port = process.env.PORT || 1337;
//create a array with users?
//var user = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24];
//when enter / standard site show the file Klient.html
app.get('/', function (req, res) {
    res.sendFile(__dirname + '/public/Klient.html');
});
//create server
var server = new WebSocketServer({
    httpServer: http
});

//When connection is called from website.
server.on('connection', function (socket) {
    /*
    //create a local variabel
    var name;
    var number;
    var vepon;
    console.log('a user connected', socket.id);

    socket.on('info', function (info) {
        name = info[0];
        vepon = info[1];
        
        if (user.length > 0) {
            number = user[0];
            user.pop(number);
        }
        socket.emit('changeBackground', number);
        console.log(name, vepon, number);
    });

    //when messege is called from client. *
    socket.on('message', function (arg) {
        console.log(arg , name);
    });
    //when client disconnect.
    socket.on('disconnect', function () {
        user.push(number);
        console.log(name, ' disconnected', user.length);
    });
    console.log(socket.data)
});

//Choose port to lissen on. 
http.listen(port, function () {
    console.log('Server is listening on: '  + ':' + port);
});


//.emit för att skicka
//.on för att ta emot
*/