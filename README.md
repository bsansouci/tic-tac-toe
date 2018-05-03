# tic-tac-toe
**ReasonDojo 2 starter app**

## Run Project

```sh
npm install
npm start
# in yet-another tab
node server.js
```

This will start a simple `express` server at `localhost:3000` which will deliver your app.

You need to restart your server (`node server.js`) each time you change `Server.re`. For the client changes you can just refresh.

### server.js
Simplest `express` setup possible to deliver a static HTML page. It calls a function exposed by `Server.re` and gives it the `http` object, which is useful for doing the socket.io stuff ;)
You don't need to touch this file.


### Server.re
That's where you'll write your server-side logic for talking to the client.

### DataFetcher.re
This is where you'll write your client-side logic for talking to the server.

### Main.re
The main tic tac toe game.

### Index.re
Well that one's pretty straightforward.

