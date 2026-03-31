function getTotalSeconds(state){
  if(state=="true"){
    const h = Number(document.getElementById("on-hh").value || 0);
    const m = Number(document.getElementById("on-mm").value || 0);
    const s = Number(document.getElementById("on-ss").value || 0);
    return h * 3600 + m * 60 + s;
  }else{
    const h = Number(document.getElementById("off-hh").value || 0);
    const m = Number(document.getElementById("off-mm").value || 0);
    const s = Number(document.getElementById("off-ss").value || 0);
    return h * 3600 + m * 60 + s;
  }
}