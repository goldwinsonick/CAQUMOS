// Firebase Setups
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.1.0/firebase-app.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/10.1.0/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyAnVjkhiPUXtsexWzpWUVxANJGxaIilqTI",
  authDomain: "city-air-quality-monitor.firebaseapp.com",
  databaseURL: "https://city-air-quality-monitor-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "city-air-quality-monitor",
  storageBucket: "city-air-quality-monitor.appspot.com",
  messagingSenderId: "105848888606",
  appId: "1:105848888606:web:184ee9e1fb80106bf83d0f",
  measurementId: "G-L0MNEDJMHJ"
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

const summaryDisplay = document.querySelector("#summary-status");
function updateStatus(AQI){
    // Good, moderate, unhealty, very unhealthy, hazardous
    if(0<=AQI && AQI<=12){
        summaryDisplay.innerHTML = "AQI 0-50 : Good";
    }else if(12<AQI && AQI<=35.4){
        summaryDisplay.innerHTML = "AQI 51-100 : Moderate";
    }else if(35.4<AQI && AQI<=56.4){
        summaryDisplay.innerHTML = "AQI 101-150 : Unhealthy for Sensitive Group";
    }else if(56.4<AQI && AQI<=150.4){
        summaryDisplay.innerHTML = "AQI 151-200 : Unhealthy";
    }else if(150.4<AQI && AQI<=250.4){
        summaryDisplay.innerHTML = "AQI 201-300 : Very Unhealthy";
    }else{
        summaryDisplay.innerHTML = "AQI 301+ : Hazardous"
    }
}

// Read from Realtime Database and update the values
let tempVal = "";
let humiVal = "";
let pm25Val = "";
let COVal = "";
let CO2Val = "";
let NH4Val = "";
const tempDisplay = document.querySelector("#temp-display");
const humiDisplay = document.querySelector("#humi-display");
const pm25Display = document.querySelector("#pm25-display");
const CODisplay = document.querySelector("#CO-display");
const CO2Display = document.querySelector("#CO2-display");
const NH4Display = document.querySelector("#NH4-display");
function updateValues(){
  tempDisplay.innerHTML = tempVal;
  humiDisplay.innerHTML = humiVal;
  pm25Display.innerHTML = (pm25Val/40).toFixed(3);
  CODisplay.innerHTML = CO2Val;
  CO2Display.innerHTML = COVal;
  NH4Display.innerHTML = NH4Val;

  updateStatus(pm25Val/40);
}
const tempRef = ref(database, 'Sensor/temperature');
const humiRef = ref(database, 'Sensor/humidity');
const pm25Ref = ref(database, 'Sensor/pm25');
const CORef = ref(database, 'Sensor/carbon-monoxide');
const CO2Ref = ref(database, 'Sensor/carbon-dioxide');
const NH4Ref = ref(database, 'Sensor/ammonium');
onValue(tempRef, (snapshot) => {
    tempVal = snapshot.val();
    updateValues();
});
onValue(humiRef, (snapshot) => {
    humiVal = snapshot.val();
    updateValues();
});
onValue(pm25Ref, (snapshot) => {
    pm25Val = snapshot.val();
    updateValues();
});
onValue(CORef, (snapshot) => {
    COVal = snapshot.val();
    updateValues();
});
onValue(CO2Ref, (snapshot) => {
    CO2Val = snapshot.val();
    updateValues();
});
onValue(NH4Ref, (snapshot) => {
    NH4Val = snapshot.val();
    updateValues();
});