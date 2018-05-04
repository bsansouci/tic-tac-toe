# tic-tac-toe
**ReasonDojo 2 starter app**

Make sure to install the toolchain: https://reasonml.github.io/docs/en/editor-plugins.html for some fun fun in-editor type errors and autocomplete.

## Your task
This project contains a single-player implementation of a tic-tac-toe game. 

The **goal** of this project is to realize that one can share type information and business logic between the server and the client, all such that the type checker ensures the client and server both correctly talk to each other.

Your **task** is to use `reasonml-community/bs-socket` to make it multi-player. At then end you should be able to have two browser windows open side by side and be able to play against yourself (because you have no friends).

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

