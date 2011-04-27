-- Load the font
verdana12 = pge.font.load("verdana.ttf", 12, PGE_RAM)

-- Check the font loaded correctly
if not verdana12 then
	error("Failed to load font.")
end

-- Init net and check success
if not pge.net.init() then
	error("Error on net init.")
end

-- Init net dialog and check success
if not pge.utils.netinit() then
	error("Error on net dialog init.")
end

-- Create a color
white = pge.gfx.createcolor(255, 255, 255)

-- Set an initial state for the net dialog
state = -1

-- Loop until exit requested
while pge.running() do

	-- Update controls
	pge.controls.update()

	-- Start drawing
	pge.gfx.startdrawing()
	
	-- Clear screen (to black)
	pge.gfx.clearscreen()
	
	-- End drawing
	pge.gfx.enddrawing()
	
	-- pge.utils.netupdate() must be called after pge.gfx.enddrawing(), but before pge.gfx.swapbuffers()
	state = pge.utils.netupdate()
	
	-- Check state of net dialog
	if state == 0 then
		-- Connected
		break
	elseif state == 1 then
		-- User cancelled
		-- Shutdown net
		pge.net.shutdown()
		break
	elseif state ~= PGE_UTILS_DIALOG_RUNNING then
		-- Internal error
		-- Shutdown net
		pge.net.shutdown()
		break
	end
	
	-- Swap buffers
	pge.gfx.swapbuffers()
	
end

cnt = 0
while pge.running() do

	-- Update controls
	pge.controls.update()
	
	-- Start drawing
	pge.gfx.startdrawing()
	
	-- Clear screen (to black)
	pge.gfx.clearscreen()
	
	-- Activate the font
	verdana12:activate()
	
	-- Print some text
	if pge.net.isconnected() then
		verdana12:print(20,15, white, "isConnected...")
	else
		verdana12:print(20, 15, white, "Not connected - Unable to send bytes")
	end
	
	-- End drawing
	pge.gfx.enddrawing()
	
	-- Swap buffers
	pge.gfx.swapbuffers()
	
	
	verdana12:print(20, 60, white, cnt)
	
	
	if pge.controls.held(PGE_CTRL_UP) then
		cnt=cnt+1
	end
	
	if pge.controls.held(PGE_CTRL_DOWN) then
		cnt = cnt -1
	end
	
	if pge.controls.held(PGE_CTRL_CROSS) then

	
	if pge.net.isconnected() then
	
		-- Download a file
		msocket = pge.net.socket.create()
		msocket:connect("192.168.1.4", 5555)
		msocket:send("yahoo")
		msocket:close()
	
		-- Shutdown net
		pge.net.shutdown()
	
		-- Start drawing
		pge.gfx.startdrawing()
	
		-- Clear screen (to black)
		pge.gfx.clearscreen()
	
		-- Activate the font
		verdana12:activate()
	
		-- Print some text
		verdana12:print(20, 45, white, "Finished")
	
		-- End drawing
		pge.gfx.enddrawing()
	
		-- Swap buffers
		pge.gfx.swapbuffers()
	
		-- Loop until START pressed
		while not pge.controls.pressed(PGE_CTRL_START) do
			pge.controls.update()
		end
		
		break
		
	end
	
	verdana12:print(20, 120, white, "Cross held")
	end
	-- If START pressed, end
	if pge.controls.pressed(PGE_CTRL_START) then
		break
	end
	
end

verdana12 = nil