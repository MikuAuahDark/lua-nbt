local nbt = require("nbt")

describe("test.nbt", function()
	local result

	it("should load without error", function()
		assert.is_not.error(function()
			local f = assert(io.open("spec/test.nbt", "rb"))
			local str = f:read("*a")
			f:close()
			result = nbt.decode(str)
		end)
	end)

	it("should return TAG_Compound with name \"hello world\"", function()
		assert.is.equal(nbt.TAG_COMPOUND, result:getTypeID())
		assert.is.equal("hello world", result:getName())
	end)

	it("should have entry \"name\" with value \"Bananrama\"", function()
		local value = result:getValue()
		assert.is.truthy(value.name)
		assert.is.equal("Bananrama", value.name:getString())
	end)

	it("should be able to be dumped correctly to human readable representation without error", function()
		assert.is_not.error(function() print(result) end)
	end)
end)
