console.log("Hello World");

//const tagHello = document.getElementById("ciao");
const tagHello = document.querySelector("#ciao");
tagHello.innerHTML = "Hello World";

//const tagYear = document.getElementsByClassName("anno")[0];
//const tagYear = document.querySelector(".anno");
const tagYear = document.querySelectorAll(".anno")[0];
tagYear.innerText = "2021";
