local Objects = {}

function UpdateObjects_Engine()

	for k, v in pairs(Objects) do
		v:OnUpdate()
	end

end

function WakeUpObjects_Engine()
	
	for k, v in pairs(Objects) do
		v:OnAwake()
	end

end

function WakeUpSpecificObject_Engine(key)

	Objects[key]:OnAwake()

end