import React, {userState} from "react";
import { NavLink } from "react-router-dom";
import { useHistory } from "react-router-dom";

const Register = () => {

    const history = useHistory()

    const [user, setUser] = userState({
        username : "",
        email : "",
        password : ""
    })

    // Manejar Inputs
    const handleInput = (event) => {
        let name = event.target.name;
        let value = event.target.value;

        setUser({...user, [name]: value});
    }

    // Manejar submit
    const handleSubmit = (event) => {
        event.preventDefault();
        const {username, email, password} = user;
        try {
            const res = fetch('/register', {
                method : "POST",
                heathers : {
                    "Content-Type" : "application/json"
                },
                body : JSON.stringify({
                    username, email, password
                })
            })

            if(res.status === 400 || !res){
                window.alert("Los datos ya están usados")
            } else {
                window.alert("Registrado con exito");
                history.push("/login")
            }
        } catch (error) {
            
        }
    }

        return (
            <div>
                <div className="container shadow my-5">
                    <div className="row justify-content-end">
                        <div className="col-md-5 d-flex flex-column align-items-center text-white justify-content-center form order-2">
                            <h1 className="display-6 fw-bolder">¡Hola!</h1>
                            <p className="lead text-center">Ingrese sus credenciales para registrarse</p>
                            <h5 className="mb-4">O</h5>
                            <NavLink to="./login" className="btn btn-outline-light rounded-pill pb-2 w-50">Inicia Sesión aquí</NavLink>
                        </div>
                        <div className="col-md-6 p-5">
                            <h1 className="display-6 fw-bolder mb-5">REGISTRARSE</h1>
                            <form onSubmit={handleSubmit} method="POST">
                                <div class="mb-3">
                                    <label for="text" class="form-label">Nombre de usuario</label>
                                    <input type="text" class="form-control" id="name" name = "username" value={user.username} onCharge={handleInput}/>
                                </div>
                                <div class="mb-3">
                                    <label for="exampleInputEmail1" class="form-label">Correo electrónico</label>
                                    <input type="email" class="form-control" id="exampleInputEmail1" aria-describedby="emailHelp" name = "email" value={user.email} onCharge={handleInput}/>
                                    <div id="emailHelp" class="form-text">No daremos su correo electrónico a nadie </div>
                                </div>
                                <div class="mb-3">
                                    <label for="exampleInputPassword1" class="form-label">Contraseña</label>
                                    <input type="password" class="form-control" id="exampleInputPassword1" name = "password" value={user.password} onCharge={handleInput}/>
                                </div>
                                <div class="mb-3 form-check">
                                    <input type="checkbox" class="form-check-input" id="exampleCheck1" />
                                    <label class="form-check-label" for="exampleCheck1">Aceptar términos y condiciones</label>
                                </div>
                                <button type="submit" class="btn btn-outline-primary rounded-pill pb-2 w-100 mt-4">Registrar</button>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        )
}

export default Register;
