import React from "react";
import Compress from "./Compress";
import Files from "./Files";

const Home = () => {
    return (
        <div>
            <section id="home">
                <div className="container">
                    <div className="row justify-content-center">
                        <div className="col-md-8 mt-5">
                            <h1 className="display-4 fw-bolder mb-4 text-center text-white">PROYECTO #3</h1>
                            <p className="lead text-center fs-4 mb-4 text-white"> Integrantes:</p>
                            <p className="lead text-center fs-4 mb-4 text-white"> Armando García</p>
                            <p className="lead text-center fs-4 mb-4 text-white"> Daniel Montolla</p>
                            <p className="lead text-center fs-4 mb-5 text-white"> Fabián Castillo</p>
                            <div className="buttons d-flex justify-content-center">
                                <button className="btn btn-light me-3 rounded-pill px4 py-2">Azure</button>
                                <button className="btn btn-light me-3 rounded-pill px4 py-2">Repositorio</button>
                                <button className="btn btn-light me-3 rounded-pill px4 py-2">Jira</button>
                            </div>
                        </div>
                    </div>
                </div>
            </section>
            <Compress/>
            <Files/>
        </div>
    )
}

export default Home;
