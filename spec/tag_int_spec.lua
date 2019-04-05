local nbt = require("nbt")

local intTest = {
	-- name, minrange, maxrange, testtruncval, cmptrunc, oormin, oormax
	{"newByte", -128, 127, 32.4, 32, -2147483648, 65535},
	{"newShort", -32768, 32767, 662.4, 662, -2147483648, 2147483647},
	{"newInt", -2147483648, 2147483647, -222222222.2, -222222222, -4294967296, 4294967295}
}

describe("NBT tag", function()
	describe("can be created with nbt.new* functions.", function()
		for i, v in ipairs(intTest) do
			describe(string.format("when calling nbt.%s", v[1]), function()
				it(string.format("with values in range of %d...%d, the value is truncated", v[2], v[3]), function()
					local x = nbt[v[1]](v[4])
					assert.is.equal(v[5], x:getInteger())
					assert.is_not.equal(v[4], x:getInteger())
				end)

				it(string.format("with values outside %d...%d, the value is clamped", v[2], v[3]), function()
					local lo = nbt[v[1]](v[6])
					local hi = nbt[v[1]](v[7])
					assert.is.equal(v[2], lo:getInteger())
					assert.is_not.equal(v[6], lo:getInteger())
					assert.is.equal(v[3], hi:getInteger())
					assert.is_not.equal(v[7], hi:getInteger())
				end)
			end)
		end
	end)
end)
