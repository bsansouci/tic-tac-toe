type state = unit;

type action = unit;

let component = ReasonReact.reducerComponent("DataFetcher");

let make = _children => {
  ...component,
  reducer: (_action: action, _state: state) => ReasonReact.NoUpdate,
  render: _self => <Main you=Main.X/>,
};
