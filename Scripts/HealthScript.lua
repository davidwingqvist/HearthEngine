local HealthScript={}

HealthScript.health = 100

--This function runs when object is created.
function HealthScript:OnAwake()



end


--This function runs each Update cycle
function HealthScript:OnUpdate()

	self.health = self.health - GetDeltaTime() * 50;
	
	if self.health <= 0 then
		Destroy()
	end

end

return HealthScript