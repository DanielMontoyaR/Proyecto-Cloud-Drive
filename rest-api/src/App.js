import logo from './logo.svg';
import './App.css';
import Navbar from './components/Navbar';
import Home from './components/Home';
import Compress from './components/Compress';
import Files from './components/Files';
import Login from './components/Login';
import Register from './components/Register';
import {Route, Switch} from 'react-router-dom';

function App() {
  return (
    <>
      <Navbar/>
      <Switch>
        <Route exact path="/" component ={Home}/>
        <Route exact path="/compress" component ={Compress}/>
        <Route exact path="/files" component ={Files}/>
        <Route exact path="/login" component ={Login}/>
        <Route exact path="/register" component ={Register}/>
      </Switch>
    </>
  );
}

export default App;
