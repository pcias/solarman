// Copyright (C) 2024 (PCS) Przemysław Ciaś


const express = require('express')
const axios = require('axios')
const { SSL_OP_SSLEAY_080_CLIENT_DH_BUG } = require('constants')
const { Console } = require('console')

const app = express()
const port = 3000

var solarman;


app.get('/', async (req, res) =>  {
  console.log("/ call");
  solarman.realTime(stationId = "1498958")
  solarman.history(stationId = "1498958")
  res.send(String(solarman.generationPower+"W;"+solarman.generationValueToday+"kWh"))
})

app.get('/realtime', async (req, res) =>  {
    console.log("/realtime call");
    res.send(String(solarman.generationPower+"W"))
})

app.get('/today', async (req, res) =>  {
    console.log("/today call");
    res.send(String(solarman.generationValueToday+"kWh"))
})


app.listen(port, () => {
  solarman = new Solarman(appId = "202112289634412", appSecret = "229093b3c3f9e068c02238137673f8cd", email = "przemyslaw.cias@gmail.com", password = "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92")
  console.log(`Solarman app listening on port ${port}`);

  setInterval(()=>{solarman.realTime(stationId = "1498958"); solarman.history(stationId = "1498958")}, 30000)
  
})

class Solarman {

    constructor(appId, appSecret, email, password) {
        this.appSecret = appSecret;
        this.appId = appId;
        this.email = email;
        this.password = password;
        this._generationPower = 0.0;
        this._generationValueToday = 0.0;
        this.token = "";
        this.lastUpdateTime = 0; 
        this.plantList = {}
        
        var axios = require('axios');
        var data = JSON.stringify({
            "appSecret": appSecret,
            "email": email,
            "password": password
        });

        var config = {
            method: 'post',
            url: 'https://globalapi.solarmanpv.com/account/v1.0/token?appId='+appId,
            headers: { 
                'Content-Type': 'application/json'
            },
            data : data
        };


        axios(config)
            .then((response) => {this.token = response.data.access_token})
            .catch((response) => {console.log(response)})
    }


    
    get generationPower() {
        return this._generationPower;
    }

    get generationValueToday() {
        return this._generationValueToday;
    }

    realTime(stationId) {

        var data = JSON.stringify({
            "stationId": stationId
        });

        var config = {
            method: 'post',
            url: 'https://globalapi.solarmanpv.com/station/v1.0/realTime',
            headers: { 
                'Authorization': 'Bearer ' + this.token, 
                'Content-Type': 'application/json'
            },
            data : data
        };

        axios(config)
        .then((response) => {
            this._generationPower = response.data.generationPower})
        .catch((response) => {console.log("HISTORY ERROR");console.log(response)})
    
    }

    plantlist() {
        var config = {
            method: 'post',
            url: 'https://globalapi.solarmanpv.com/station/v1.0/list',
            headers: { 
                'Authorization': 'Bearer ' + this.token
            }
        };

        axios(config)
            .then((response) => {this.plantList = response.data.plantList})
            .catch((response) => {console.log(response)})
    }


     history(stationId) {

        var today = new Date();
        var date = today.getFullYear()+'-'+(today.getMonth()+1)+'-'+today.getDate();

        var data = JSON.stringify({
        "stationId": stationId,
        "endTime": date,
        "startTime": date,
        "timeType": 2
        });

        var config = {
            method: 'post',
            url: 'https://globalapi.solarmanpv.com/station/v1.0/history',
            headers: { 
                'Authorization': 'Bearer ' + this.token,
                'Content-Type': 'application/json'
            },
            data : data
        };

        axios(config)
        .then((response) => {
            try {
                this._generationValueToday = response.data.stationDataItems[0].generationValue
            }
            catch(e) {}
        })
        .catch((response) => {console.log("HISTORY ERROR");console.log(response)})
    }
    
}



// "appSecret": "229093b3c3f9e068c02238137673f8cd",
// "email": "przemyslaw.cias@gmail.com",
// "password": "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92"
// "stationId": 1498958
