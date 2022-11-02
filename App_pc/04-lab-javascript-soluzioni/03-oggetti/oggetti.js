function Computer(processore, disco, ram) {
    this.processore = processore;
    this.disco = disco;
    this.ram = ram;
}
Computer.prototype.infoComputerConsole = function() { 
    console.log("Processore: " + this.processore + "\nDisco: " + this.disco + "\nRam: " + this.ram)
};

Computer.prototype.infoComputerDOM = function(id){
    document.getElementById(id).innerHTML = `
    <p>Processore: ${this.processore}</p>
    <p>Disco: ${this.disco}</p>
    <p>RAM: ${this.ram}</p>
    `;
}

class Computer2 {
    constructor(processore, disco, ram) {
        this.processore = processore;
        this.disco = disco;
        this.ram = ram;
        
    }
    infoComputerConsole = function () {
        console.log("Processore: " + this.processore + "\nDisco: " + this.disco + "\nRam: " + this.ram);
    }
    infoComputerDOM (id) {
        document.getElementById(id).innerHTML = `
            <p>Processore: ${this.processore}</p>
            <p>Disco: ${this.disco}</p>
            <p>RAM: ${this.ram}</p>
            `;
    }
}


const mioPc = new Computer("i7", "500GB", "16GB");
mioPc.infoComputerConsole();
mioPc.infoComputerDOM("miopc");

const mioPc2 = new Computer("i5", "250GB", "16GB");
mioPc2.infoComputerConsole();
mioPc2.infoComputerDOM("miopc2");