local MoveScript={}


--This function runs when object is created.
function MoveScript:OnAwake()
	print(Transform.xPosition)
end


--This function runs each Update cycle
function MoveScript:OnUpdate()
	
end

return MoveScript