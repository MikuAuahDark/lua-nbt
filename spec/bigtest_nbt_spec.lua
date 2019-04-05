local nbt = require("nbt")

describe("bigtest.nbt", function()
	local result, nbtstr, result2

	it("should load without error", function()
		assert.is_not.error(function()
			local f = assert(io.open("spec/bigtest.nbt", "rb"))
			local str = f:read("*a")
			f:close()
			result = nbt.decode(str)
			nbtstr = str
		end)
	end)

	it("should parsed in plain mode without error", function()
		assert.is_not.error(function()
			result2 = nbt.decode(nbtstr, "plain")
		end)
		assert.is.equal("table", type(result2))
	end)

	describe("\"nested compound test\" should have member named \"egg\"", function()
		local egg
		assert.is_not.error(function()
			egg = result:getValue()["nested compound test"]:getValue().egg
		end)

		it("where \"name\" field should be \"Eggbert\"", function()
			assert.is.equal("Eggbert", egg:getValue().name:getString())
		end)

		it("where \"value\" field should be 0.5", function()
			assert.is.True(math.abs(egg:getValue().value:getNumber() - 0.5) <= 0.000001)
		end)
	end)

	it("should be able to be dumped correctly to human readable representation without error", function()
		assert.is_not.error(function() print(result) end)
	end)
end)
