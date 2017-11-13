console = {
	command="",
	cursory = 7
}

function _init()
	cursor(0,0)
	print("JUNO 8")
	print(">", 0, 7)
end

function _input()
	--console.command = input()
	console.command = console.command .. input()
	if keydown("backspace") then
		console.command = string.sub(console.command, 1, #console.command -1)
	end
	if keypress("return") then
		--dostring(console.command)
		f = loadstring(console.command)
		console.command = ""
		--cursor(0, peek(0x5f27)+6)
		cursor(0)
		f()
		console.cursory = peek(0x5f27) + 2
	end
end

function _update()
	--console.command = input()
end

function _draw()
	--console.command = input()
	--clear(1)
	--cursor(0,0)
	if (peek(0x5f27) - 6 < 0) then
		poke(0x5f27, peek(0x5f27)+6)
	end
	print(">", 0, console.cursory)
	print(console.command, 5, console.cursory)
end