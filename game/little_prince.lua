local Game = {}
Game.__index = Game

Game.EXPLORING = "exploring"
Game.RETURNING_HOME = "returning_home"
Game.CLOSED = "closed"

local required_native = {
  "walk",
  "turn",
  "look",
  "reset_camera",
  "set_rose_watered",
  "start_exit_video",
  "exit_video_finished",
  "quit",
}

function Game.new(native)
  assert(type(native) == "table", "native backend is required")

  for _, name in ipairs(required_native) do
    assert(type(native[name]) == "function", "native." .. name .. " is required")
  end

  return setmetatable({
    native = native,
    phase = Game.EXPLORING,
    rose_watered = false,
  }, Game)
end

local function exploring(self)
  return self.phase == Game.EXPLORING
end

function Game:walk(metres)
  if exploring(self) then
    self.native.walk(metres)
  end
end

function Game:turn(radians)
  if exploring(self) then
    self.native.turn(radians)
  end
end

function Game:look(yaw, pitch)
  if exploring(self) then
    self.native.look(yaw, pitch)
  end
end

function Game:reset_camera()
  if exploring(self) then
    self.native.reset_camera()
  end
end

function Game:water_rose()
  if not exploring(self) then
    return
  end

  if not self.rose_watered then
    self.rose_watered = true
    self.native.set_rose_watered(true)
  end
end

function Game:ride_comet()
  if not exploring(self) then
    return false
  end

  self.phase = Game.RETURNING_HOME
  self.native.start_exit_video("assets/comet-return-earth.mp4")
  return true
end

function Game:update(_dt)
  if self.phase ~= Game.RETURNING_HOME then
    return
  end

  if self.native.exit_video_finished() then
    self.phase = Game.CLOSED
    self.native.quit()
  end
end

return Game
