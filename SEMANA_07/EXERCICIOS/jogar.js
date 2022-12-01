const queryString = window.location.search;
const urlParams = new URLSearchParams(queryString);
const jogador = urlParams.get('jogador')
console.log(jogador);

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

const ip = '192.168.15.11'

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