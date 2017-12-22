require("lua_res1.printTable")

print(1);
local tab = {}
printTable(tab)

print(12345678)

names = {"Peter", "Paul", "Mary"}
grades = {Mary = 10, Paul = 7, Peter = 8}
table.sort(names, function (n1, n2)
return grades[n1] > grades[n2] -- compare the grades
end)
printTable(names)


function ProgressState(currentState,stateTable)
	if stateTable[currentState] ~= nil then
		return stateTable[currentState]()
	end
	return "Error"
end


function HoldFunction()
	if EnemyNearby() then
		return "pursue"
	else
		return "hold"
	end
end

function PursueFunction()
	if DistanceToEnemy() > 10 then
		return "to_base"
	elseif DistanceToEnemy() > 5 then
		return "purse"
	else
		return "attack"
	end
end

function AttackFunction()
	if EnemyHealth() > 0 then
		return "attack"
	else
		return "to_base"
	end
end

function ToBaseFunction()
	if DistanceToBase() > 0 then
		return "to_base"
	else
		return "hold"
	end
end

Unit = {}
Unit.CurrentState = "hold"
Unit["hold"] = HoldFunction
Unit["pursue"] = PursueFunction
Unit["attack"] = AttackFunction
Unit["to_base"] = ToBaseFunction

--Unit.CurrentState = ProgressState(Unit.CurrentState,Unit)












