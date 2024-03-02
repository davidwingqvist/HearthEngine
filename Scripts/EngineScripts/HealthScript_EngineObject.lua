HealthScript_Objects={}

function UpdateHealthScriptObjects_Engine()
		
		for k, v in pairs(HealthScript_Objects) do
			v:OnUpdate()
		end

end