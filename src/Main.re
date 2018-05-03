type gridPiece =
  | X
  | O
  | Empty;

/* State declaration */
type state = {
  grid: list(gridPiece),
  turn: gridPiece,
};

/* Action declaration */
type action =
  | Click(int);

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("Game");

let px = x => string_of_int(x) ++ "px";

let pxf = x => string_of_float(x) ++ "px";

/* greeting and children are props. `children` isn't used, therefore ignored.
   We ignore it by prepending it with an underscore */
let make = (~you, _children) => {
  /* spread the other default fields of component here and override a few */
  ...component,
  initialState: () => {
    grid: [X, O, X, Empty, Empty, Empty, Empty, Empty, Empty],
    turn: X,
  },
  /* State transitions */
  reducer: (action, state) =>
    switch (state, action) {
    | ({turn, grid}, Click(i)) when turn == you =>
      ReasonReact.Update({
        turn: you === X ? O : X,
        grid:
          List.mapi(
            (j, el) =>
              if (i === j) {
                you;
              } else {
                el;
              },
            grid,
          ),
      })
    | _ => ReasonReact.NoUpdate
    },
  render: self => {
    let yourTurn = you == self.state.turn;
    let message = yourTurn ? "Your turn" : "Their turn";
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
        <div
          style=(
            ReactDOMRe.Style.make(
              ~display="flex",
              ~width=px(443),
              ~height=px(443),
              ~flexWrap="wrap",
              ~justifyContent="left",
              ~alignItems="center",
              ~backgroundColor="black",
              (),
            )
          )>
          (
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
                    let canClick = canClick && yourTurn;
                    <div
                      key=(string_of_int(i))
                      onClick=(_event => canClick ? self.send(Click(i)) : ())
                      style=(
                        ReactDOMRe.Style.make(
                          ~display="flex",
                          ~width=px(145),
                          ~height=px(145),
                          ~fontSize=px(45),
                          ~marginLeft=px(2),
                          ~justifyContent="center",
                          ~alignItems="center",
                          ~backgroundColor="white",
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
