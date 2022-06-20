import React, {userState} from "react";
import { NavLink } from "react-router-dom";

const Login = () => {

    const [user, setuser] = userState({
        email : '',
        password: ''
    });

    // Manejar Input

    const handleChange = (event) =>{
        let name = event.target.name
        let value = event.target.value

        setuser({...user, [name]:value})
    }

    // Manejar Login
    const handleSubmit = (event) => {
        event.preventDefault ();
        const {email, password} = user;
        try {
            const res = fetch('/login', {
                method : "POST",
                heathers : {
                    "Content-Type" : "application/json"
                },
                body : JSON.stringify({
                    email, password
                })
            });

            if(res.status === 400 || !res){
                window.alert("Credenciales Invalidas");
                window.location.reload();
            } 

        } catch (error) {
            console.log(error);
        }
    }

    return (
        <div>
            <div className="container shadow my-5">
                <div className="row">
                    <div className="col-md-5 d-flex flex-column align-items-center text-white justify-content-center form">
                        <h1 className="display-6 fw-bolder">Binevenido de Vuelta</h1>
                        <p className="lead text-center">Ingrese sus credenciales</p>
                        <h5 className="mb-4">O</h5>
                        <NavLink to="./register" className="btn btn-outline-light rounded-pill pb-2 w-50">Registrese aquí</NavLink>
                    </div>
                    <div className="col-md-6 p-5">
                        <h1 className="display-6 fw-bolder mb-5">INICIAR SESIÓN</h1>
                        <form onSubmit={handleSubmit}>
                            <div class="mb-3">
                                <label for="exampleInputEmail1" class="form-label">Correo electrónico</label>
                                <input type="email" class="form-control" id="exampleInputEmail1" aria-describedby="emailHelp" name="email" value={user.email} onChange={handleChange}/>
                                <div id="emailHelp" class="form-text">No daremos su correo electrónico a nadie </div>
                            </div>
                            <div class="mb-3">
                                <label for="exampleInputPassword1" class="form-label">Contraseña</label>
                                <input type="password" class="form-control" id="exampleInputPassword1" name="password" value={user.password} onChange={handleChange} />
                            </div>
                            <div class="mb-3 form-check">
                                <input type="checkbox" class="form-check-input" id="exampleCheck1" />
                                <label class="form-check-label" for="exampleCheck1">Recordar mis datos</label>
                            </div>
                            <button type="submit" class="btn btn-primary btn-outline-light rounded-pill pb-2 w-100 mt-4">Enviar</button>
                        </form>
                    </div>
                </div>
            </div>
        </div>
    )
}

export default Login;
