//Pego dos paramêtros da URL, qual é o jogador (1 ou 2)
const queryString = window.location.search;
const urlParams = new URLSearchParams(queryString);
const jogador = urlParams.get('jogador')
console.log(jogador);

//Validação que varia a cor dos botões da página caso seja o jogador 1 ou 2
if (jogador == 1) {
    document.getElementById("titleJogador").innerHTML = "Jogador 1";
    document.getElementsByClassName("btnSelection")[0].style.backgroundColor = "rgb(151, 28, 28)";
    document.getElementsByClassName("btnSelection")[1].style.backgroundColor = "rgb(151, 28, 28)";
    document.getElementsByClassName("btnSelection")[2].style.backgroundColor = "rgb(151, 28, 28)";
} else {
    document.getElementById("titleJogador").innerHTML = "Jogador 2";
    document.getElementsByClassName("btnSelection")[0].style.backgroundColor = "rgb(28, 28, 153)";
    document.getElementsByClassName("btnSelection")[1].style.backgroundColor = "rgb(28, 28, 153)";
    document.getElementsByClassName("btnSelection")[2].style.backgroundColor = "rgb(28, 28, 153)";
}

//IP do computador que está rodando o servidor(app.js), para que seja possível o funcionamento por um outro dispositivo na mesma rede
const ip = '192.168.15.22'

//Função que envia a seleção do jogador para o servidor
function sendResult(selection) {
    axios.post(`http://${ip}:3001/verificador`, {
        selection: selection,
        jogador: jogador
    })
    .then(function (response) {
        console.log(response);
    })
    .catch(function (error) {
        console.log(error);
    });

    window.location.href = "./index.html";
}