local print = print
local pairs = pairs
local table = table
local tostring = tostring
local type = type
local getmetatable = getmetatable
local debug = debug
local type = type
local string = string

local encode_value = nil
local tableCache = {}


local function encode_table(writer, tab, indent)
  indent = indent or ""
  if #indent > 20 then
    return
  end
  writer:write("{ --" .. tostring(tab) .. "\n")
  do
    local indent = indent .. "\t"
    for k, v in pairs(tab) do
      writer:write(indent)
      writer:write("[")
      encode_value(writer, k, indent)
      writer:write("] = ")
      encode_value(writer, v, indent)
      writer:write(",\n")
    end
    local mt = getmetatable(tab)
    if mt then
      writer:write(indent)
      writer:write("[")
      encode_value(writer, 'metatable', indent)
      writer:write("] = ")
      encode_value(writer, mt, indent)
      writer:write(",\n")
    end
  end
  writer:write(indent)
  writer:write("}")
end

encode_value = function(writer, value, indent)
  indent = indent or ""
  local t = type(value)
  if t == "table" then
    if tableCache[value] then
      writer:write(tostring(value))
    else
      tableCache[value] = true
      encode_table(writer, value, indent)
    end
  elseif t == "string" then
    writer:write(string.format("%q", value))
  elseif t == "function" then
    local info = debug.getinfo(value)
    writer:write(tostring(info.func) .. '\n' .. indent .. tostring(info.short_src) .. ' [' .. info.linedefined .. '-' .. info.lastlinedefined .. ']')
  else
    writer:write(tostring(value))  
  end
end

--function returnString(t)
--  tableCache = {}
--  local info = debug.getinfo(2)
--  local luaFileName = string.match(info.short_src,"%a+%.lua") or ""
--  local writer = {}
--  writer.write = function(self,s)
--    table.insert(self,s)
--  end
--  writer:write("[")
--  writer:write(luaFileName)
--  writer:write(":")
--  writer:write(info.currentline)
--  writer:write("]")
--  writer:write("\n")
--  encode_value(writer,t)
----  for i, v in pairs(writer) do
----    print(i,v)
----  end
--  return table.concat(writer)
--end


function printTable(t)
  tableCache = {}
  local info = debug.getinfo(2)
  local luaFileName = string.match(info.short_src,"%a+%.lua") or ""
  local writer = {}
  writer.write = function(self,s)
    table.insert(self,s)
  end
  writer:write("[")
  writer:write(luaFileName)
  writer:write(":")
  writer:write(info.currentline)
  writer:write("]")
  writer:write("\n")
  encode_value(writer,t)
--  for i, v in pairs(writer) do
--    print(i,v)
--  end
  print(table.concat(writer))
end
--local a1 = function() 
--  return 1 
--end
--printTable({1,2,3,4,5,6,7,8,9,10,{a = "asd", b = 1, c = {1,2,3,4}}, a = a1,})