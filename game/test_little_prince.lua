local Game ← dofile("game/little_prince.lua")

local calls ← {}
local finished ← false

local called ← λ(name, ...)
  calls[#calls + 1] ← { name, ... }
end

local native ← {
  walk ← λ(metres) called("walk", metres) end,
  turn ← λ(radians) called("turn", radians) end,
  look ← λ(yaw, pitch) called("look", yaw, pitch) end,
  reset_camera ← λ() called("reset_camera") end,
  set_rose_watered ← λ(value) called("set_rose_watered", value) end,
  start_exit_video ← λ(path) called("start_exit_video", path) end,
  exit_video_finished ← λ() return finished end,
  quit ← λ() called("quit") end,
}

local game ← Game.new(native)

game:walk(0.25)
game:water_rose()
assert(game.rose_watered)

assert(game:ride_comet())
assert(game.phase = Game.RETURNING_HOME)

-- The comet ride is terminal: normal gameplay no longer reaches native code.
local before ← #calls
game:walk(1.0)
game:turn(1.0)
game:water_rose()
assert(#calls = before)

game:update(0.016)
assert(game.phase = Game.RETURNING_HOME)

finished ← true
game:update(0.016)
assert(game.phase = Game.CLOSED)
assert(calls[#calls][1] = "quit")

-- Do not quit repeatedly.
local after_quit ← #calls
game:update(0.016)
assert(#calls = after_quit)

print("PASS Lua comet exit state machine")
