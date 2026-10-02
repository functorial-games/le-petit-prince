# Lua game layer

Version 0 keeps the Lua side deliberately small.

Lua owns the rules:

- exploring the tiny planet;
- watering the rose;
- deciding when the player rides the comet;
- switching into the terminal return-home phase.

C/native code owns:

- spherical walking and turning;
- camera and rendering;
- terrain;
- Android input/output;
- video playback;
- process exit.

## Comet ending

`Game:ride_comet()` is a one-way transition:

```
Exploring -> ReturningHome -> Closed
```

On entry to `ReturningHome`, Lua asks the host to play exactly one asset:

```
assets/comet-return-earth.mp4
```

While that video is playing, movement, camera, and rose interactions are disabled. The host reports when playback has finished; Lua then asks the host to quit.

The video itself is not in the repository yet. The intended first cut is simply stars moving past while Earth grows larger in view. No steering, branching, second map, or comet gameplay is required.

## Native surface

The injected native table currently needs eight operations:

```
walk(metres)
turn(radians)
look(yaw, pitch)
reset_camera()
set_rose_watered(bool)
start_exit_video(path)
exit_video_finished() -> bool
quit()
```

That is the whole Lua/C boundary for this slice.
