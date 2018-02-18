--TODO function
--
-- basic lua language
--
-- this section illustrate somme basic lua commands, most useful to luacad
--


-- comment!

--[[
very
long
comment
--]]

--
-- see https://www.lua.org/manual/5.3/  for complete reference
--

a=10
print(a)

--for loop goes like this: "for i=[start],[limit],[increment] do [something] end

print("for loop:")
for i=1,10,2 do print(i^2) end


--if works like this: 
--if [something is true] then [do something] elseif [something is true] then [do something]
--else [do something] end

print("if conditionals")
if 0 then print("the only false value are: false and nil (0 is true!)") end
if 1~=2 then print("~= is the not equal op") end
if 1==2 then print("== is the equal op") elseif 1 then print("elseif, not  else if nor elif!!") end
if true then print("booleans are: true, false") end
if nil then else print("the catch all is else [do something].. no then needed!!") end

--data structures are of the form {key1=value1, ...}. Values without keys are assigned numbers starting from 1 
print{"tables:"}
array = {'hi','hello',3}
print(array[1].."! note how the index starts at 1 and not 0...")
array.pebbles = 12
array[1] = 'bye'
table.insert(array,1,'first')
for k,v in pairs(array) do print("key:",k," value:",v) end





