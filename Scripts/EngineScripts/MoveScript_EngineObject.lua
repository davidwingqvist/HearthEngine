MoveScript_Objects={}

function UpdateMoveScriptObjects_Engine()

		for k, v in pairs(MoveScript_Objects) do
			v:OnUpdate()
		end

end

function UpdateSingleMoveScriptObject_Engine(key)
	MoveScript_Objects[key]:OnUpdate()
end

function AwakeSingleMoveScriptObject_Engine(key)
	MoveScript_Objects[key]:OnAwake()
end