//Variable
var name;
//connecting to socket ws:...
var socket = new WebSocket('ws://192.168.43.177/');

   
    ////orientation
            //function openFullScreen() {
        //    if (document.fullscreenEnabled) {

        //        if (document.documentElement.requestFullscreen) {
        //            document.documentElement.requestFullscreen();
        //        } else if (document.mozRequestFullScreen) { /* Firefox */
        //            document.mozRequestFullScreen();
        //        } else if (document.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
        //            document.webkitRequestFullscreen();
        //        } else if (document.msRequestFullscreen) { /* IE/Edge */
        //            document.msRequestFullscreen();
        //        } else if (document.mozFullScreenEnabled) { /* */
        //            document.mozFullScreenEnabled();
        //        } else if (document.fullScreenEnabled) { /*  */
        //            document.fullScreenEnabled();
        //        }
        //    }
        //    var iOS = /iPad|iPhone|iPod/.test(navigator.userAgent) && !window.MSStream;
        //    if (iOS) {


        //    }
        //}                     
        //window.addEventListener("unhandledrejection", function (promiseRejectionEvent) {
        //    // handle error here, for example log  
        //    console.log(promiseRejectionEvent);
        //    console.log(this.window);
        //    console.log(this.document);,
        //});
        //function lockScreen() {
        //    openFullScreen();

        //    //rotation funkar inte på datorn!!!
        //    var orientation = screen.msOrientation || screen.mozOrientation || (screen.orientation || {}).type;
        //    screen.lockOrientationUniversal = screen.lockOrientation || screen.mozLockOrientation || screen.msLockOrientation;

        //    if (screen.orientation.lock("landscape-primary")) {
        //        // Orientation was locked
        //    } else if (screen.lockOrientationUniversal("landscape-primary")) {
        //        // Orientation was locked
        //    }
        //    else {
        //        // Orientation lock failed
        //    }

        //    //changeView
        //    document.getElementById("startView").style.visibility = "hidden";
        //    document.getElementById("nameView").style.visibility = "visible";
        //}

//standard thigs delete???
socket.onopen = function (event) {
    console.log('Connection is open ...');
};
socket.onerror = function (err) {
    console.log('err: ', err);
};
socket.onclose = function () {
    document.getElementById("startView").style.visibility = "visible";
    document.getElementById("nameView").style.visibility = "hidden";
    document.getElementById("weaponView").style.visibility = "hidden";
    document.getElementById("gameView").style.visibility = "hidden";
    console.log("Connection is closed...");
};

//Messege recived from socket
socket.onmessage = function (event) {
    arg = event.data.split(' ');
    //Change background depending on the players id
    //When the game is ending do something
    if (arg[0] == "changeBackground") {
        url = "url(Images/Avatars/avatar" + arg[1] + ".png)";
        document.getElementById("gameView").style.backgroundImage = url;
    }
    else if (arg[0] == "end") {
        //do something
    }
};

//change view from start to name
function startFun() {
    document.getElementById("startView").style.visibility = "hidden";
    document.getElementById("nameView").style.visibility = "visible";
}

//Save name and make change view from name to weapon
function nameFun() {
    //openFullScreen();
    name = document.getElementById("nameField").value.trim();
    //check the input name.
    if (name.toLowerCase().includes("bajs") || true == name.toLowerCase().includes("kuk")) {
        document.getElementById("nameField").placeholder = "Va, det där är inte ditt namn!";
        document.getElementById("nameField").value = "";
    }
    else if (!(name == null || name == "" || name == "Name" || name == "Skriv in ditt namn" || name == "Oj det där var inte ditt namn!" || name == "Va fan det där är inte ditt namn!" || name.length < 3)) {
        document.getElementById("nameView").style.visibility = "hidden";
        document.getElementById("weaponView").style.visibility = "visible";
    }
    else {
        document.getElementById("nameField").placeholder = "Oj det där var inte ditt namn!";
        document.getElementById("nameField").value = "";
    }
}

//To enable enter key
function runScript(e) {
    //See notes about 'which' and 'key'
    if (e.keyCode == 13) {
        nameFun();
        return false;
    }
}

//save vepon and change view from vepon to game
function weaponFun(arg) {
    socket.send("info " + name + " " + arg);
    document.getElementById("weaponView").style.visibility = "hidden";
    document.getElementById("gameView").style.visibility = "visible";
}
   
//Send a messege arg that is called from the playbuttons
function message(arg) {
    socket.send("message " + arg);
}
    

    