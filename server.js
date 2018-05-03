const path = require('path');
const express = require('express');
const app = express();
// Pass the express app to our http module, which we'll also pass to socket.io.
const http = require('http').Server(app);
const startSocketIOServer = require('./src/Server.bs.js').startSocketIOServer;

startSocketIOServer(http);

// Turn this server into a static server (so it can requests all the JS files one by one).
app.use(express.static(path.join(__dirname)))

// Serve the main html page.
app.get('/', function(req, res){
  res.sendFile(path.join(__dirname, 'src', 'index.html'));
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
