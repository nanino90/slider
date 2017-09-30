
function send_data(){


var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
xmlhttp.open("POST", "/config");
xmlhttp.setRequestHeader("Content-Type", "application/json");

var obt = document.getElementById("obt").value;
var dur = document.getElementById("dur").value;
var fot = document.getElementById("fot").value;
var dis = document.getElementById("dis").value;

xmlhttp.send(JSON.stringify({firstName:"John Rambo", lastName:"2pm" , age:"23"}));

xmlhttp.onreadystatechange = function () { //Call a function when the state changes.
	//    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
	//      alert(xmlhttp.responseText);
//}
}
}

function start(){

var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
xmlhttp.open("POST", "/config");
xmlhttp.setRequestHeader("Content-Type", "application/json");
xmlhttp.send(JSON.stringify({status:1}));

xmlhttp.onreadystatechange = function () { //Call a function when the state changes.
	    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
	      alert(xmlhttp.responseText);
}
}
}
