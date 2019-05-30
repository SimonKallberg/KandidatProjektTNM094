var express = require('express');
var app = require('express')();
var server = require('http').Server(app);
var WebSocketServer = require('websocket').server;

//Tells the app that the map "Public" is static and use that for website stuff.
app.use(express.static(__dirname + '/Public'));
//Websocket variables
var port = process.env.PORT || 1337; port = 80;
var gameAddress = "::ffff:127.0.0.1";
var gameUpdateInterval = 100; //ms
var gameUpdateFunction;
var gameSocket = null;

//Variablesfor saving connections and players.
var connectionArray = [];
var playerArray = [];

//What the website publish
app.get('/', function (req, res) {
    res.sendFile(__dirname + '/public/Klient.html');
});

//Save a port that server is lisening to
server.listen(port, function () {
    console.log('server is listening on: ' + server.address().address + ':' + port);
});
//create websocket
var wsServer = new WebSocketServer({
    httpServer: server
});

//when a call for the websocket is made
wsServer.on('request', function (request) {
    
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
        //check if a player is defiend and if not, create player
        if (playerArray.map(function (p) { return p.rAddress; }).indexOf(connection.socket.remoteAddress) === -1) {
            playerArray.push({
                rAddress: connection.socket.remoteAddress,
                name: 'player' + playerArray.length,
                id: playerArray.length,
                controls: '0.0 0.0 0.0 0.0'
            });

        }

        //call från websidan
        connection.on('message', function (message) {
            if (message.type === 'utf8') {
                arg = message.utf8Data.split(' ');
          
                if ( arg[0] === "info") {
                    name = playerArray[playerArray.map(function (p) { return p.rAddress; }).indexOf(connection.socket.remoteAddress)].name = arg[1];
                    weapon = arg[2];
                    number = playerArray[playerArray.map(function (p) { return p.rAddress; }).indexOf(connection.socket.remoteAddress)].id;
                    connection.send('changeBackground '+ number);
                    console.log(name, weapon, number, "change person");
                    if (gameSocket) {
                        gameSocket.send('P ' + arg[1]);
                        console.log('notified GAME that player was added');
                    } else {
                        console.log('WARNING: NO GAME CONNECTION \n DANGER DANGER DANGER');
                    }
                }
                //when messege is called from client playbutton.
                else if (arg[0] === "message") {
                    //Skickar controller till gamesocket
                    if (gameSocket) {
                        gameSocket.send("C" + number + "" + arg[1]);
                    }
                    console.log("sending: C" + number + "" + arg[1]);
                    
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
        //gameSocket.send('C\n' + msg);
        //console.log('Sending game:\n' + msg);
    }
}


