//Importar todas las dependencias
const dotenv = require('dotenv');
const express = require('express');
const bcryptjs = require('bcryptjs');
const jwt = require('jsonwebtoken');
const cookieParser = require('cookie-parser');

const app = express();

// Configurar archgivo ENV y archivo de conexión requerido
dotenv.config(({path : './config.env'}));
require('./database/conn');
const port = process.env.PORT;

// Modelo requerido
const Users = require('./models/userSchema');

// Se usa para obtener datos del frontend
app.use(express.json());
app.use(express.urlencoded({extended : false}));
app.use(cookieParser());

app.get('/', (req, res)=> {
    res.send("Hello World");
})

// Registro
app.post('./register', async (req, res) => {
    try {
        // Obtener datos
        const username = req.body.username;
        const email = req.body.email;
        const password = req.body.password;

        const createUser = new Users({
            username : username,
            email : email,
            password : password
        });

        const created = await createUser.save()
        console.log(created);
        req.statusCode(200).send("Registered");

    } catch (error) {
        res.status(400).send(error)
    }
})

// Login dew usuario
app.post('/login', async (req, res)=> {
    try {
        const email = req.body.email;
        const password = req.body.password;
        
        // Encontrar usuario si existe
        const user = await Users.findOne({email : email});
        if (user){
            const isMatch = await bcryptjs.compare(password, users.password);
        
            if (isMatch){
                // Token generado que es definido por el esquema de usuario
                const token = await user.generatedToken();
                res.cookie("jwt", token, {
                    expires : new Date(Date.now() + 86400000),
                    httpOnly : true
                })
                req.status(200).send("Sesiño iniciada")
            } else {
                res.status(400).send("Credenciales invalidas")
            }
        } else {
            res.status(400).send("Credenciales invalidas")
        }

    } catch (error) {
        res.status(400).send(error);
    }
})

// Correr servidor
app.listen(port, () => {
    console.log("Server is listening")
})
