const mongoose = require('mongoose');
const bcryptjs = require('bcryptjs');
const jwt = require('jsonwebtoken');

// Esquema de usuarios o Estructura de Documento
const userSchema = new mongoose.Schema({
    username : {
        type  : String,
        required  : true,
        unique : true
    },
    email : {
        type : String,
        required  : true,
        unique : true,
    },
    password : {
        type : String,
        required : true
    },
    tokens : [
        {
            token : {
                type : String,
                required  : true
            }
        }
    ]
})

// Seguridad de contrtaseña
userSchema.pre('save',async function(next){
    if(this.isModified('password')){
         this.password = bcryptjs.hashSync(this.password,10);
    }
    next();
 })

// Generar tokens para verificar usuarios
userSchema.methods.generatedToken = async function(){
    try{
        let generatedToken = jwt.sign({_id : this._id}, process.env.SECRET_KEY);
        this.tokens = this.tokens.concat({token : generateToken});
        await this.save();
        return generatedToken;
    } catch (error) {
        console.log(error)
    }
}

// Crear modelo
const Users=new mongoose.model("USER",userSchema);

module.exports = Users;
