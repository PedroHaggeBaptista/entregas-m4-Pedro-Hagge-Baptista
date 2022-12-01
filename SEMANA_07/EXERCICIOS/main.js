const ip = '192.168.15.11'

function testeConec() {
    console.log("Conectado");
    axios.get(`http://${ip}:3001/testeConec`)
    .then(function (response) {
        console.log(response);
    })
    .catch(function (error) {
        console.log(error);
    });
}