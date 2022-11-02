
var xhttp = new XMLHttpRequest();
var url = "http://localhost:8000/esp/data/"

$(document).ready(function(){ setInterval(getSensorData, 5000, url); });


function getSensorData(url){
  $.getJSON(url, function(result){
    $("#temperature_log").html(result["temperatura"] + "°C");
    $("#light_log").html(result["lux"] + " Cd")
    $.each(result, function(i, field){
      $("#demo").append(field + " ");
      console.log(field + " " + i);
    });
  });
  return ;
}