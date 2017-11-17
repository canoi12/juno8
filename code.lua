x = 32

function _init()
end

function _update()
    if(btn(0)) then x = x - 1 end
    if(btn(1)) then x = x + 1 end
end

function _draw()
    clear()
    pal()

    --camera(x-48, -48)
    camera(x-48,-48)
    for i=0,15 do
        pset(i,0,i)
    end
    --clip(90,0,64,64)
    rectfill(x,0,x+32,31,8)
    print("TA FUNCIONANDO! HIHIHIHIHIHI", 0, 0, 15)
    camera()
    --pal(7,11)
    spr(0,64,64,2)
    pal(11,8)
    spr(0,96,64,2)
    --spr(0,72,64, true, true)
    --pal(7,9)
    --print(((-1*(x-48))))
    --print(0-x-48)
    --print(x)
    --print(peek(0x5f20))
    --print(peek(0x5f21))
    --print(peek(0x5f23))
    --print("teste", 10, 20)
    --pal()
    --local teste = io.read()
    --print(teste)
    --print(((-1*(x-48))) < peek(0x5f22))
    --pset(1,6,0)
    --clip()
end