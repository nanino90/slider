
function set(){

	var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
	xmlhttp.open("POST", "/set");
	xmlhttp.setRequestHeader("Content-Type", "application/json");

	var obt = document.getElementById("obt").value;
	var dur = document.getElementById("dur").value;
	var fot = document.getElementById("fot").value;
	var dis = document.getElementById("dis").value;


	var json = 	"{\"obturacion\":\""+obt+
			"\",\"duracion\":\""+dur+
			"\",\"fotos\":\""+fot+
			"\",\"distancia\":\""+dis+
			"\"}";
	xmlhttp.send(json);	

	xmlhttp.onreadystatechange = function () { //Call a function when the state changes.
		    if (xmlhttp.readyState == 4 && xmlhttp.status == 400) {
		      alert(xmlhttp.responseText);
		}
	}
}

function control(command){

	var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
	xmlhttp.open("POST", "/control");
	xmlhttp.setRequestHeader("Content-Type", "application/json");

	var json = 	"{\"control\":\""+command+
			"\"}";
	xmlhttp.send(json);	

	xmlhttp.onreadystatechange = function () { //Call a function when the state changes.
		if (xmlhttp.readyState == 4 && xmlhttp.status == 400) {
			alert(xmlhttp.responseText);
		}
	}
}

