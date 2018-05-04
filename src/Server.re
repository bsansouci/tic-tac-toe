let startSocketIOServer = http => {
  print_endline("starting socket server");
  module MyServer = BsSocket.Server.Make(Common);
  let io = MyServer.createWithHttp(http);
  /* Simple ref to track the two players. When the first player connects we assign them X, when the
     second connects we assign them O.
     If the first player disconnects we set playerA to be false such that when they reconnect, they're
     the first player again. If the 2nd player disconnects we don't do anything because when they
     reconnect, they'll still be the 2nd player.
     */
  let playerA = ref(false);
  /* Track the state so we can handle disconnecting/reconnecting properly.
     This is obviously not secure at all, anyone connecting as a 2nd player can act as the 2nd player,
     whoever was the second player before :) */
  let state = ref(Common.initialState(Common.X));
  MyServer.onConnect(
    io,
    socket => {
      print_endline("Got a connection!");
      let you =
        if (! playerA^) {
          playerA := true;
          Common.X;
        } else {
          Common.O;
        };
      MyServer.Socket.emit(socket, Common.OnConnection({...state^, you}));
      MyServer.Socket.onDisconnect(socket, () =>
        if (you == Common.X) {
          playerA := false;
        }
      );
      /* On any event, broadcast it to all other connected users. */
      MyServer.Socket.on(
        socket,
        action => {
          state := Common.updateState(action, state^);
          MyServer.Socket.broadcast(socket, action);
        },
      );
    },
  );
};
