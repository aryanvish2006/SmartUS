const relays = [
  {id:0, name: "Unknown", icon: "fa-couch", theme: "theme-blue", state: false },
  {id:1, name: "Unknown", icon: "fa-utensils", theme: "theme-orange", state: false },
  {id:2, name: "Unknown", icon: "fa-tree", theme: "theme-green", state: false },
  {id:3, name: "Unknown", icon: "fa-fan", theme: "theme-purple", state: false }
];
const timers = [
  { remainingTime: 0 , state : false , isTimerRunning:false},
  { remainingTime: 0 , state : false, isTimerRunning:false},
  { remainingTime: 0 , state : false, isTimerRunning:false},
  { remainingTime: 0 , state : false, isTimerRunning:false},
]

let deviceId = "--";
let userName = "--";
let firmwareVersion = "--"; 
let activated = "--";

let apSsid = "--";
let apPass = "--";
let staSsid = "--";
let staPass = "--";
let relayDelay = 0;

// prevents polling from fighting user actions
let userInteracting = false;


//
const apEditBtn = document.getElementById("ap-edit");
const apSaveBtn = document.getElementById("ap-save-btn");
const apInputs=[
  document.getElementById("ap-ssid"),
  document.getElementById("ap-pass")
]
const staEditBtn = document.getElementById("sta-edit");
const staSaveBtn = document.getElementById("sta-save-btn");
const staInputs=[
  document.getElementById("sta-ssid"),
  document.getElementById("sta-pass")
]

let apEditing = false;
let staEditing = false;

apEditBtn.addEventListener("click",()=>{
  apEditing = !apEditing;

  apInputs.forEach(i=>i.toggleAttribute("readonly"));
  apSaveBtn.classList.toggle("hidden",!apEditing);
  document.getElementById("appassinput").classList.toggle("hidden",!apEditing);
})
staEditBtn.addEventListener("click",()=>{
  staEditing = !staEditing;

  staInputs.forEach(i=>i.toggleAttribute("readonly"));
  staSaveBtn.classList.toggle("hidden",!staEditing);
  document.getElementById("stapassinput").classList.toggle("hidden",!staEditing);
})

// ======================
// RENDER FUNCTIONS
// ======================

function renderRelays() {
  const container = document.getElementById("relay-grid");
  container.innerHTML = "";

  relays.forEach(r => {
    const active = r.state ? `active ${r.theme}` : "";
    const status = r.state ? "ON" : "OFF";

    container.innerHTML += `
      <div class="relay-card ${active}">
        <div class="card-top">
        
          <div class="card-text">
            <h3>${r.name}</h3>
            <p>${status}</p>
          </div>
          <div class="icon-box">
            <i class="fas ${r.icon}"></i>
          </div>
        </div>

        <button class="big-toggle" onclick="toggleRelay(${r.id})">
          <div class="big-knob">
            <svg class="icon"><use href="#power"></use></svg>
          </div>
        </button>
        <div class="card-actions">
          <button class="sub-btn" onclick="openModal('timer',${r.id})"><i class="fas fa-hourglass-half"></i> Timer</button>
          <button class="sub-btn" onclick="openModal('schedule',${r.id})"><i class="fas fa-clock"></i> Plan</button>
        </div>
        
    `;
  });

  updateRelayUi();
}

function toggleRelay(id){
  console.log(id)
  relays[id].state = true;
}
// Modal Logic

function openModal(type) {
  
  const overlay = document.getElementById('modal-overlay');
    const tDiv = document.getElementById('modal-timer');
    const sDiv = document.getElementById('modal-schedule');
    const rDiv = document.getElementById("modal-timer-remaining");
    const title = document.getElementById('modal-title');
    
    overlay.classList.add('open'); 
          
    if(type === 'timer') {
        title.innerText = "Set Timer";
        tDiv.classList.remove('hidden');
        sDiv.classList.add('hidden');
        rDiv.classList.add('hidden');
    } else {
        title.innerText = "Schedule Event";
        tDiv.classList.add('hidden');
        sDiv.classList.remove('hidden');
        rDiv.classList.add('hidden');
    } 
}

function closeModal() {
  
  const overlay = document.getElementById('modal-overlay');
    
    overlay.classList.remove('open'); 

}

function navigateTo(viewId) {
  ["home", "settings", "mqtt"].forEach(v => {
    document.getElementById("view-" + v).classList.add("hidden");
  });

  document.getElementById("view-" + viewId).classList.remove("hidden");

  const btn = document.getElementById("nav-settings-btn");
  viewId === "home" ? btn.classList.remove("hidden") : btn.classList.add("hidden");
}

renderRelays();