
var xhttp = new XMLHttpRequest();
var url1 = "http://localhost:8000/esp/data/"
var url2 = "http://localhost:8000/arduino/status/"
var url3 = "http://localhost:8000/arduino/irrigation/"
$(document).ready(function(){
  setInterval(getSensorData, 1000, url1);
  setInterval(getStatus, 1000, url2);
  setInterval(getIrrigationState, 1000, url3);
});


function getSensorData(url){
  $.getJSON(url, function(result) {
    temp_color = (result["temperatura"] == 1 ? "blue": result["temperatura"] == 2 ? "aqua" : result["temperatura"] == 3 ? "yellow" : "red")
    light_color = (result["lux"] == 1 ? "black": result["lux"] == 2 ? "gray" : result["lux"] == 3 ? "darkgray" : "lightgray")

    var termo_style = {
      "background-color": temp_color,
      "color": result["temperatura"] == 0 ? "white" : temp_color,
      "border-radius": "10px",
      "box-shadow":  "0 0 10px rgba(0,0,0,0.5)",
      "width": (result["temperatura"] == 0 ? 0 : result["temperatura"] == 1 ? 25: result["temperatura"] == 2 ? 50 : result["temperatura"] == 3 ? 75 : 100) + "%"
    };
    var light_style = {
      "background-color": light_color,
      "color": result["lux"] == 0 ? "white" : light_color,
      "border-radius": "10px",
      "box-shadow":  "0 0 10px rgba(0,0,0,0.5)",
      "width": (result["lux"] == 0 ? 0 : result["lux"] == 1 ? 25: result["lux"] == 2 ? 50 : result["lux"] == 3 ? 75 : 100) + "%"
    };
    $("#termometro").css(termo_style)
    $("#luciometro").css(light_style)
    
    $("#temperature_log").html(result["temperatura"] + "°C");
    $("#light_log").html(result["lux"] + " Cd")
    /*$.each(result, function(i, field){
      $("#demo").append(field + " ");
      console.log(field + " " + i);
    });*/
  });
  return ;
}
function getStatus(url){
  $.getJSON(url, function(result){
    $("#status").html(result["status"]);
  });
  return ;
}

function getIrrigationState(url){
  $.getJSON(url, function(result){
    $("#irrigazione").html(result["irrigazione"]);
  });
  return ;
}
