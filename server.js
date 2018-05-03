const path = require('path');
const express = require('express');
const app = express();
const http = require('http').Server(app);
const startSocketIOServer = require('./src/Server.bs.js').startSocketIOServer;

startSocketIOServer(http);

app.use(express.static(path.join(__dirname)))

app.get('/', function(req, res){
  res.sendFile(path.join(__dirname, 'src', 'index.html'));
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
