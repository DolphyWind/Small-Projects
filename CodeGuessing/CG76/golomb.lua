tbl = {[1]=1}
function entry(x)
    if tbl[x] ~= nil then
        return tbl[x]
    end

    tbl[x] = 1 + entry(x - entry(entry(x - 1)))
    return tbl[x]
end
