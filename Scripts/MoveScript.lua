local MoveScript={}


--This function runs when object is created.
function MoveScript:OnAwake()
end


--This function runs each Update cycle
function MoveScript:OnUpdate()
		--Transform.xPosition = Transform.xPosition + GetDeltaTime();
		SetRotation(45, 0, 0)
end

return MoveScript