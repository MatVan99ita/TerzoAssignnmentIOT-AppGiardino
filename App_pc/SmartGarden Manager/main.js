
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
  $.getJSON(url, function(result){
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
