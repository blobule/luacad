

require("table-extra")


t=table.flatten({1,2,allo=1,{3,toto=3,4},5,6})

for k,v in pairs(t) do
    print(k,v)
end

u=table.keyValues({1,2,allo=1,3,toto=3,4,5,6})

print("===")
for k,v in pairs(u) do print(k,v,type(k)) end

table.keyValues({1,blub="allo",2,allo=1,3,toto=3,4,5,6},t)

print("===")
for k,v in pairs(t) do print(k,v,type(k)) end


a={bbb=123,ccc=456}
b={{aaa=123,11,22,33},{44,55,toto=45,66}}
t=table.merge(a,b)

print("===")
for k,v in pairs(t) do print(k,v,type(k)) end



