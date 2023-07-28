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
  pm25Display.innerHTML = pm25Val;
  CODisplay.innerHTML = COVal;
  CO2Display.innerHTML = CO2Val;
  NH4Display.innerHTML = NH4Val;
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