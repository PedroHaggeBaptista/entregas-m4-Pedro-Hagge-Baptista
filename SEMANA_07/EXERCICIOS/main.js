//IP do computador que está rodando o servidor(app.js), para que seja possível o funcionamento por um outro dispositivo na mesma rede
const ip = '192.168.15.22'

//Função que envia a chamada GET para teste de conexão
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