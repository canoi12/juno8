console = {
	command="",
	cursorx = 5,
	cursory = 7,

	camerax = 0,
	cameray = 0,

	pointer = {
		pos = 0,
		x = 5,
		y = 7,
		color = 8,
		t = 0
	},
	cmd_pointer = 0,
	old_commands = {}
}

function _init()
	cursor(0,0)
	print("JUNO 8")
	print(">", 0, 7)
end

function _input()
	local txt = input()
	if (txt ~= "") then
		local aux = string.sub(console.command, 1, console.pointer.pos)
		aux = aux .. txt
		console.command = aux .. string.sub(console.command, console.pointer.pos + 1, #console.command)
		console.pointer.pos = console.pointer.pos + 1
	end
	if keydown("backspace") then
		if (console.pointer.pos > 0) then
			local aux = string.sub(console.command, 1, console.pointer.pos-1)
			console.command = aux .. string.sub(console.command, console.pointer.pos + 1, #console.command)
			console.pointer.pos = console.pointer.pos - 1
		end
	end
	if keypress("return") then
		console.pointer.color = 0
		f = loadstring(console.command)
		if console.command ~= "" then
			console.cmd_pointer = #console.old_commands + 1
			console.old_commands[console.cmd_pointer] = ""
			console.old_commands[console.cmd_pointer] = console.command
		end
		console.command = ""
		cursor(0)
		if not f then 
			rectfill(0, peek(0x5f27), 128, peek(0x5f27)+6, 0)
			print("SYNTAX ERROR", nil, nil, 8)
		else
			f()
		end
		console.cursory = peek(0x5f27)
		console.pointer.x = 5
		console.pointer.y = console.cursory
		console.pointer.pos = 0
	end

	if keydown("left") then
		if console.pointer.pos > 0 then
			console.pointer.pos = console.pointer.pos - 1
		end
	elseif keydown("right") then
		if console.pointer.pos < #console.command then
			console.pointer.pos = console.pointer.pos + 1
		end
	end

	if keydown("up") then
		if console.cmd_pointer > 0 then
			console.command = console.old_commands[console.cmd_pointer]
			console.cmd_pointer = console.cmd_pointer - 1
			console.pointer.x = 5
			console.pointer.y = console.cursory
			console.pointer.pos = #console.command
		end
	end
end

function _update()
	console.pointer.t = console.pointer.t + 0.5
end

local teste = 0

function _draw()

	--camera(console.camerax, console.cameray)
	if (peek(0x5f27) - 6 < 0) then
		poke(0x5f27, peek(0x5f27)+6)
	end
	rectfill(0, console.cursory, 5, console.cursory+6, 0)
	print(">", 0, console.cursory)

	for i=console.cursory,teste do
		rectfill(5, i, 128, i + 6, 0)
	end
	teste = console.cursory
	console.cursorx = 5
	

	local x = console.cursorx - 4
	local y = teste

	for i = 0, #console.command do
		if (console.cursorx > 125) then
			console.cursorx = console.cursorx - 124
			teste = teste + 6
			--[[if (teste) > 128 then
				console.cameray = console.cameray + 6
			end]]
		end
		print(console.command:sub(i+1,i+1), console.cursorx, teste)
		console.cursorx = console.cursorx + 4
		if i == console.pointer.pos then
			console.pointer.x = console.cursorx - 4
			console.pointer.y = teste
		end
	end
	local mul = 1
	if math.sin(console.pointer.t) > 0 then
		mul = 0
	end
	rectfill(console.pointer.x, console.pointer.y, console.pointer.x + 3, console.pointer.y + 5, console.pointer.color * mul)
	console.pointer.color = 8
end