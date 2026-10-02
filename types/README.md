# Edriç type sketch

`LittlePrince.idric` is a type-level sketch of the game boundary.

The intended implementation remains:

- **Lua** for game rules and scene behavior;
- **C** for spherical walking, terrain, rendering, Android glue, and other native work;
- **Edriç** here as a compact specification experiment.

The interesting part is the indexed `Command` type. Ordinary movement and watering exist only during `Exploring`. `RideComet` transitions once to `ReturningHome`; that phase has no gameplay commands. The host shows one fixed flight-through-the-stars / approach-Earth sequence and closes the program when it finishes.

The geometric frame stays abstract. The C implementation is responsible for the invariant that `up` and `forward` remain unit, tangent, and orthogonal. Latitude and longitude therefore never become part of the walker state.
