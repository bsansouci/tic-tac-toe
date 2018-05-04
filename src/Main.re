/* Type representing a grid cell */
type gridCellT =
  | X
  | O
  | Empty;

/* State declaration.
   The grid is a simple linear list.
   The turn uses a gridCellT to figure out whether it's X or O's turn.
   The winner will be a list of indices which we'll use to highlight the grid when someone won. */
type state = {
  grid: list(gridCellT),
  turn: gridCellT,
  you: gridCellT,
  winner: option(list(int)),
};

/* Action declaration */
type action =
  | Restart
  | Click(int);

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
  initialState: () => {
    grid: [Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty],
    turn: X,
    you: X,
    winner: None,
  },
  /* State transitions */
  reducer: (action, state) =>
    switch (state, action) {
    | ({turn, grid}, Click(cell)) =>
      /* Apply the action to the grid first, then we check if this new grid is in a winning state. */
      let newGrid =
        List.mapi(
          (i, el) =>
            if (cell === i) {
              turn;
            } else {
              el;
            },
          grid,
        );
      let arrGrid = Array.of_list(newGrid);
      /* Military grade, Machine Learning based, winning-condition checking algorithm:
         just list all the possible options one by one.
         */
      let winner =
        if (arrGrid[0] != Empty
            && arrGrid[0] == arrGrid[1]
            && arrGrid[1] == arrGrid[2]) {
          Some([0, 1, 2]);
        } else if (arrGrid[3] != Empty
                   && arrGrid[3] == arrGrid[4]
                   && arrGrid[4] == arrGrid[5]) {
          Some([3, 4, 5]);
        } else if (arrGrid[6] != Empty
                   && arrGrid[6] == arrGrid[7]
                   && arrGrid[7] == arrGrid[8]) {
          Some([6, 7, 8]);
        } else if (arrGrid[0] != Empty
                   && arrGrid[0] == arrGrid[3]
                   && arrGrid[3] == arrGrid[6]) {
          Some([0, 3, 6]);
        } else if (arrGrid[1] != Empty
                   && arrGrid[1] == arrGrid[4]
                   && arrGrid[4] == arrGrid[7]) {
          Some([1, 4, 7]);
        } else if (arrGrid[2] != Empty
                   && arrGrid[2] == arrGrid[5]
                   && arrGrid[5] == arrGrid[8]) {
          Some([2, 5, 8]);
        } else if (arrGrid[0] != Empty
                   && arrGrid[0] == arrGrid[4]
                   && arrGrid[4] == arrGrid[8]) {
          Some([0, 4, 8]);
        } else if (arrGrid[2] != Empty
                   && arrGrid[2] == arrGrid[4]
                   && arrGrid[4] == arrGrid[6]) {
          Some([2, 4, 6]);
        } else {
          None;
        };
      /* Return new winner, new turn and new grid. */
      ReasonReact.Update({
        ...state,
        winner,
        turn: turn === X ? O : X,
        grid: newGrid,
      });
    | (_, Restart) =>
      /* Reset the entire state */
      ReasonReact.Update({
        ...state,
        grid: [Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty],
        turn: X,
        winner: None,
      })
    },
  render: self => {
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
          onClick=(_event => self.send(Restart))>
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
                      onClick=(_event => canClick ? self.send(Click(i)) : ())
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
