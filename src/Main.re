module MyClient = BsSocket.Client.Make(Common);

let socket = MyClient.create();

type state = Common.state;

/* Action declaration, purposely named different from what's in Common.t to avoid confusion.
   You could name them the same. As a matter of fact you could share that type as well, but I
   don't think it's a realistic thing to do because the server might send many more kinds of
   messages to the client, and the local actions most likely won't be tied to what the server sends.
   It only makes sense here because our clients are basically mirror images of each other. */
type action =
  | RestartClicked
  | Click(int)
  | NewState(state);

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("Game");

/* Helper functions for CSS properties. */
let px = x => string_of_int(x) ++ "px";

/* Main function that creates a component, which is a simple record.
   `component` is the default record, of which we overwrite initialState, reducer and render.
   */
let make = _children => {
  /* spread the other default fields of component here and override a few */
  ...component,
  initialState: () => Common.initialState(Common.X),
  /* Listen on socket events when mounting and simply dispatch to reducer. */
  didMount: self =>
    MyClient.on(socket, data =>
      switch (data) {
      | OnConnection(state) => self.send(NewState(state))
      | Restart => self.send(RestartClicked)
      | Turn(i) => self.send(Click(i))
      }
    ),
  /* State transitions */
  reducer: (action, state) =>
    switch (action) {
    | Click(i) =>
      ReasonReact.Update(Common.updateState(Common.Turn(i), state))
    | RestartClicked =>
      ReasonReact.Update(Common.updateState(Common.Restart, state))
    | NewState(s) =>
      ReasonReact.Update(Common.updateState(Common.OnConnection(s), state))
    },
  render: self => {
    open Common;
    let yourTurn = self.state.you == self.state.turn;
    let message =
      switch (self.state.winner) {
      | None => yourTurn ? "Your turn" : "Their turn"
      | Some([i, ..._]) =>
        List.nth(self.state.grid, i) == X ? "X wins!" : "O wins"
      | _ => assert false
      };
    ReasonReact.(
      <div
        style=(
          ReactDOMRe.Style.make(
            ~display="flex",
            ~width=px(439),
            ~alignItems="center",
            ~flexDirection="column",
            (),
          )
        )>
        <div style=(ReactDOMRe.Style.make(~fontSize=px(45), ()))>
          (string(message))
        </div>
        <button
          style=(
            ReactDOMRe.Style.make(
              ~fontSize=px(20),
              ~marginTop=px(8),
              ~marginBottom=px(16),
              ~border="1px solid #AAAAAA",
              ~backgroundColor="#EEEEEE",
              ~cursor="pointer",
              (),
            )
          )
          onClick=(
            _event => {
              self.send(RestartClicked);
              MyClient.emit(socket, Restart);
            }
          )>
          (string("Restart"))
        </button>
        <div
          style=(
            ReactDOMRe.Style.make(
              ~display="flex",
              ~width=px(443),
              ~height=px(443),
              ~flexWrap="wrap",
              ~justifyContent="left",
              /*~alignItems="center",*/
              /*~backgroundColor="black",*/
              (),
            )
          )>
          (
            /* Iterate over our grid and create the cells, with their contents and background color
               if there's a winner.*/
            array(
              Array.of_list(
                List.mapi(
                  (i, piece) => {
                    let (txt, canClick) =
                      switch (piece) {
                      | Empty => (" ", true)
                      | X => ("X", false)
                      | O => ("O", false)
                      };
                    let backgroundColor =
                      switch (self.state.winner) {
                      | None => "white"
                      | Some(winner) =>
                        let isCurrentCellWinner = List.mem(i, winner);
                        if (isCurrentCellWinner
                            && List.nth(self.state.grid, i) == self.state.you) {
                          "green";
                        } else if (isCurrentCellWinner) {
                          "red";
                        } else {
                          "white";
                        };
                      };
                    /* We check if the user can click here so we can hide the cursor: pointer. */
                    let canClick =
                      canClick && yourTurn && self.state.winner == None;
                    <div
                      key=(string_of_int(i))
                      onClick=(
                        _event =>
                          if (canClick) {
                            self.send(Click(i));
                            MyClient.emit(socket, Turn(i));
                          }
                      )
                      style=(
                        ReactDOMRe.Style.make(
                          ~display="flex",
                          ~width=px(145),
                          ~height=px(145),
                          ~fontSize=px(45),
                          ~marginLeft=px(-1),
                          ~paddingTop=px(2),
                          ~marginBottom=px(-1),
                          ~justifyContent="center",
                          ~alignItems="center",
                          ~backgroundColor,
                          ~border="1px solid black",
                          ~cursor=canClick ? "pointer" : "",
                          (),
                        )
                      )>
                      <span> (string(txt)) </span>
                    </div>;
                  },
                  self.state.grid,
                ),
              ),
            )
          )
        </div>
      </div>
    );
  },
};
