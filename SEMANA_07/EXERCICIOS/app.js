const express = require('express')
var bodyParser = require('body-parser')
const cors = require('cors')
const axios = require('axios')

//IP recebido pelo ESP32 da rede local
const ip = '192.168.15.26'


const app = express()

app.use(cors())

app.use(express.json()) //Irá suportar JSON
app.use(
    bodyParser.urlencoded({
        // Irá suportar urlenconded
        extended: true,
    })
)

const PORT = process.env.PORT || 3001

//Guarda a seleção dos jogadores 1 e 2
let selections = [];

app.use(express.static("./"))

//Função que recebe a seleção do jogador e guarda no array
app.post("/verificador", (req, res) => {
    selections[req.body.jogador] = req.body.selection;

    console.log(selections)

    //Quando o array estiver com a seleção dos dois jogadores, ele efetua a checagem do vencedor e envia o resultado para o ESP32
    if (selections[1] != null && selections[2] != null) {
        let result = selections[1] == selections[2] ? "empate" : selections[1] == "Pedra" && selections[2] == "Tesoura" || selections[1] == "Papel" && selections[2] == "Pedra" || selections[1] == "Tesoura" && selections[2] == "Papel" ? "jogador1" : "jogador2";
        axios.get(`http://${ip}/${result}`).then(resp => {
            //console.log(resp.data);
        });
        console.log(result);
        //Zero o array de seleção dos jogadores (1 e 2), para que possa ser usado novamente
        selections = [];
    }
    res.send("Adicionado");
})

//Função que recebe a chamada GET para teste de conexão
app.get("/testeConec", (req, res) => {
    //Efetua uma chamada para o ESP32, para que ele toque o buzzer informando que existe conexão
    axios.get(`http://${ip}/testeConnect`)
    res.send("Acionado");
})

app.listen(PORT, () => {
    console.log(`Servidor rodando na porta http://localhost:${PORT}`)
})
