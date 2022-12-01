const express = require('express')
var bodyParser = require('body-parser')
const cors = require('cors')
const axios = require('axios')

const ip = '192.168.15.23'


const app = express()
// app.use(cors());

//Só a necessidade de ativar está parte do código quando ele estiver em produção
app.use(cors())

app.use(express.json()) //Irá suportar JSON
app.use(
    bodyParser.urlencoded({
        // Irá suportar urlenconded
        extended: true,
    })
)

const PORT = process.env.PORT || 3001

let selections = [];

app.use(express.static("./"))

app.post("/verificador", (req, res) => {
    selections[req.body.jogador] = req.body.selection;

    console.log(selections)

    if (selections[1] != null && selections[2] != null) {
        let result = selections[1] == selections[2] ? "empate" : selections[1] == "Pedra" && selections[2] == "Tesoura" || selections[1] == "Papel" && selections[2] == "Pedra" || selections[1] == "Tesoura" && selections[2] == "Papel" ? "jogador1" : "jogador2";
        axios.get(`http://${ip}/${result}`).then(resp => {
            //console.log(resp.data);
        });
        console.log(result);
        selections = [];
    }
    res.send("Adicionado");
})

app.get("/testeConec", (req, res) => {
    axios.get(`http://${ip}/testeConnect`)
    console.log("Teste")
    res.send("Acionado");
})

app.listen(PORT, () => {
    console.log(`Servidor rodando na porta http://localhost:${PORT}`)
})
