


co1 = coroutine.create(
    function()
        for i = 1, 100 do
            print("co1_"..i)
            coroutine.yield(co2)
        end
    end
)

co2 = coroutine.create(
    function()
        for i = 1, 100 do
            print("co2_"..i)
            coroutine.yield(co1)
        end
    end
)

for i = 1, 100 do
    coroutine.resume(co1)
    coroutine.resume(co2)
end


