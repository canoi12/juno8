x = 32

function _init()
end

function _update()
    if(btn(0)) then x = x - 1 end
end

function _draw()
    clear()

    for i=0,15 do
        pset(i,0,i)
    end
    --clip(90,0,64,64)
    --camera(32,0)
    rectfill(x,0,32,32,8)
    camera()
    clip()
end