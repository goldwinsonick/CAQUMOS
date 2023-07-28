// // Firebase Setups
// import { initializeApp } from "https://www.gstatic.com/firebasejs/10.1.0/firebase-app.js";
// import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/10.1.0/firebase-database.js";

// const firebaseConfig = {
//   apiKey: "AIzaSyAnVjkhiPUXtsexWzpWUVxANJGxaIilqTI",
//   authDomain: "city-air-quality-monitor.firebaseapp.com",
//   databaseURL: "https://city-air-quality-monitor-default-rtdb.asia-southeast1.firebasedatabase.app",
//   projectId: "city-air-quality-monitor",
//   storageBucket: "city-air-quality-monitor.appspot.com",
//   messagingSenderId: "105848888606",
//   appId: "1:105848888606:web:184ee9e1fb80106bf83d0f",
//   measurementId: "G-L0MNEDJMHJ"
// };

// const app = initializeApp(firebaseConfig);
// const database = getDatabase(app);

// // Read from Realtime Database and update the values
// let tempVal = 0;
// let humiVal = 0;
// const tempDisplay = document.querySelector("#temperature-display");
// const humiDisplay = document.querySelector("#humidity-display");
// function updateValues(){
//   tempDisplay.innerHTML = tempVal;
//   humiDisplay.innerHTML = humiVal;
// }
// const tempRef = ref(database, 'Sensor/temperature');
// const humiRef = ref(database, 'Sensor/humidity');
// const coRef = ref(database, 'Sensor/carbon-monoxide');
// const co2Ref = ref(database, 'Sensor/carbon-dioxide');
// const nh4Ref = ref(database, 'Sensor/ammonium');
// onValue(tempRef, (snapshot) => {
//     tempVal = snapshot.val();
//     updateValues();
// });
// onValue(humiRef, (snapshot) => {
//     humiVal = snapshot.val();
//     updateValues();
// });