const evt= new EventSource('/events');

evt.onmessage = () =>{
    fetch();
}

function fetch(){

}

function setRelayState(index,state){

}