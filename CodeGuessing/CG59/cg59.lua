local function gen_array(n, x)
    local l = {}
    for i=1,n do
        l[i] = x
    end
    return l
end

local function clone_table(original)
    local copy = {}
    for key, value in pairs(original) do
        if type(value) == "table" then
            copy[key] = clone_table(value)
        else
            copy[key] = value
        end
    end
    return copy
end

local function push_table(dest, src)
    for i=1,#src do
        dest[#dest + 1] = src[i]
    end
end

local MAX_DEPTH = 10
local finished_interpreters = {}
local new_interpreters = {}

local function make_interpreter()
    local interpreter = {
        stack = {},
        random_access_memory = gen_array(30000, 0),
        depth = 0,
        bounds = {},
        loops = {},
        idx = 1,
        has_finished = false,
        coe = 1.0,
        code = '',
    }

    function interpreter.pop_stack(i)
        if #i.stack == 0 then
            return 0
        end

        local x = i.stack[#i.stack]
        table.remove(i.stack)
        return x
    end

    function interpreter.push_stack(i, x)
        table.insert(i.stack, x)
    end

    function interpreter.clone(i, upto)
        if i.depth >= MAX_DEPTH then
            return {}
        end

        local clones = {}
        for idx=0,upto do
            local new_interpreter = clone_table(i)
            new_interpreter.depth = i.depth + 1
            new_interpreter.coe = i.coe / (upto + 1)
            new_interpreter:push_stack(idx)
            table.insert(clones, new_interpreter)
        end
        return clones
    end

    function interpreter.step(i)
        if i.has_finished then
            return false
        end
        if i.idx > #i.code or i.depth >= MAX_DEPTH then
            table.insert(finished_interpreters, i)
            i.has_finished = true
            return false
        end

        local chr = string.sub(i.code, i.idx, i.idx)
        i.idx = i.idx + 1

        if chr == '#' then
            i:push_stack(0)
        elseif chr == '+' then
            local x = i:pop_stack()
            i:push_stack(x + 1)
        elseif chr == '@' then
            local address = i:pop_stack() + 1
            i:push_stack(i.random_access_memory[address])
        elseif chr == '!' then
            local address = i:pop_stack() + 1
            local value = i:pop_stack()
            i.random_access_memory[address] = value
        elseif chr == '?' then
            local number = i:pop_stack()
            push_table(new_interpreters, i:clone(number))
            return false
        elseif chr == '[' then
            local bound = i:pop_stack()
            table.insert(i.bounds, bound)
            table.insert(i.loops, i.idx)
        elseif chr == '|' then
            local condition = i:pop_stack()
            if condition == 0 or i.bounds[#i.bounds] == 0 then
                table.remove(i.loops, #i.loops)
                table.remove(i.bounds, #i.bounds)
                local lc = 0
                while true do
                    local char = string.sub(i.code, i.idx, i.idx)
                    if char == '[' then
                        lc = lc + 1
                    elseif char == ']' then
                        if lc == 0 then
                            break
                        end
                        lc = lc - 1
                    end
                    i.idx = i.idx + 1
                end
                i.idx = i.idx + 1
            end
        elseif chr == ']' then
            i.bounds[#i.bounds] = i.bounds[#i.bounds] - 1
            i.idx = i.loops[#i.loops]
        end

        return true
    end
    return interpreter
end


function entry(code)
    local interpreter = make_interpreter()
    interpreter.code = code

    local interpreters = {interpreter}
    finished_interpreters = {}
    new_interpreters = {}

    local stepped = true
    while stepped or #new_interpreters ~= 0 do
        new_interpreters = {}
        stepped = false

        local idx = 1
        while idx <= #interpreters do
            local i = interpreters[idx]
            if i:step() == false then
                table.remove(interpreters, idx)
            else
                stepped = true
                idx = idx + 1
            end
        end

        push_table(interpreters, new_interpreters)
    end

    local outputs = {}

    for _,i in ipairs(finished_interpreters) do
        local top = i:pop_stack()
        if outputs[top] == nil then
            outputs[top] = i.coe
        else
            outputs[top] = outputs[top] + i.coe
        end
    end

    local most_probable_outcomes = {}
    local high = 0

    for k,v in pairs(outputs) do
        if v > high then
            most_probable_outcomes = {k}
            high = v
        elseif v == high then
            table.insert(most_probable_outcomes, k)
        end
    end

    return most_probable_outcomes
end

return {
    e = entry
}
