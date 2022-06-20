import React from "react";
import {NavLink} from "react-router-dom";

const Navbar = () => {
    return (
        <div>
            <nav className="navbar navbar-expand-lg navbar-light shadow">
                <div className="container">
                    <button className="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                        <span className="navbar-toggler-icon"></span>
                    </button>
                    <div className="collapse navbar-collapse" id="navbarSupportedContent">
                        <ul className="navbar-nav me-auto mb-2 mb-lg-0">
                            <li className="nav-item">
                                <NavLink className="nav-link active" aria-current="page" to="/">Inicio</NavLink>
                            </li>
                            <li className="nav-item">
                                <NavLink className="nav-link" to="/compress">Comprimir</NavLink>
                            </li>
                            <li className="nav-item">
                                <NavLink className="nav-link" to="/files">Archivos</NavLink>
                            </li>
                        </ul>
                        <NavLink className="navbar-brand fw-bolder fs-4 mx-auto" to="/">CLOUDDRIVE</NavLink>
                        <NavLink to="./login" className="btn btn-outline-primary ms-auto px-4 rounded-pill">
                            <i className="fa fa-sign-in me-2"></i>Iniciar Sesión</NavLink>
                        <NavLink to="/register" className="btn btn-outline-primary ms-2 px-4 rounded-pill">
                            <i className="fa fa-user-plus me-2"></i>Registrarse</NavLink>
                    </div>
                </div>
            </nav>
        </div>
    );
}

export default Navbar;
