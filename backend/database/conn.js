const mongoose = require('mongoose');

const database = process.env.DATABASE;

mongoose.connect(database, {
    userNewUrlParser : true,
    userUnifiedTopology : true
}).then(()=>{
    console.log("Connection Successfull")
}).catch((e) =>{
    console.log(e);
})
