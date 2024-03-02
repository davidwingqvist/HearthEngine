local HealthScript_Objects={}

function UpdateHealthScriptObjects_Engine()

		for k, v in pairs(HealthScript) do
			v:OnUpdate()
		end

end