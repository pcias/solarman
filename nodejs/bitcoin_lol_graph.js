// Copyright (C) 2024 (PCS) Przemysław Ciaś
// 
const axios = require('axios');

// URL for the API request
const url = "https://api.coincap.io/v2/assets/bitcoin/history?interval=h2";

// Making the API request
axios.get(url)
    .then(response => {
        const history = response.data.data; // Extracting the 'data' part

        // Processing and displaying the data
        // For simplicity, this example just logs the first few elements
        history.slice(-12).forEach(item => {
            console.log(`Time: ${convertUnixTimeToHMS(item.time)}, Price: ${item.priceUsd}`);
        });
    })
    .catch(error => {
        console.error("Error fetching data:", error);
    });

function convertUnixTimeToHMS(unixTime) {
    // Create a new Date object from the Unix timestamp
    let date = new Date(unixTime);

    // Extract hours, minutes, and seconds
    let hours = date.getUTCHours();
    let minutes = date.getUTCMinutes();
    let seconds = date.getUTCSeconds();

    // Zero padding for single digit minutes and seconds
    minutes = minutes < 10 ? '0' + minutes : minutes;
    seconds = seconds < 10 ? '0' + seconds : seconds;

    // Format the time in HH:MM:SS
    return `${hours}:${minutes}:${seconds}`;
}