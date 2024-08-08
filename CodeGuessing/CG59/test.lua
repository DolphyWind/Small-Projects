local cg = require("meme_solution")
local out = cg.e(io.read())

if type(out) == "table" then
    for _,v in ipairs(out) do
        print(v)
    end
else
    print(out)
end
