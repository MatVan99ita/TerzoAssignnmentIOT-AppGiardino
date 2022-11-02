
var xhttp = new XMLHttpRequest();
var url = "http://localhost:8000/esp/data/"

$(document).ready({
  xhttp: onreadystatechange = function() {
    if (this.readyState === 4 && this.status === 200) {
      // Typical action to be performed when the document is ready:
      document.getElementById("demo").innerHTML = xhttp.responseText;
      console.log(xhttp.responseText);
    }
  },
  xhttp:open("GET", url, true),
  xhttp:send()

});
