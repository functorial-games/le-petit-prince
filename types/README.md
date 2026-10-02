# Edriç type sketch

`LittlePrince.idric` is a type-level sketch of the game boundary.

The intended implementation remains:

- **Lua** for game rules and scene behavior;
- **C** for spherical walking, terrain, rendering, Android glue, and other native work;
- **Edriç** here as a compact specification experiment.

The interesting part is the indexed `Command` type. Ordinary movement preserves state, while actions such as watering the rose or boarding a comet change only the state they are allowed to change.

The geometric frame stays abstract. The C implementation is responsible for the invariant that `up` and `forward` remain unit, tangent, and orthogonal. Latitude and longitude therefore never become part of the walker state.
