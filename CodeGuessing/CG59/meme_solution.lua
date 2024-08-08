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

local interpreter = {
    stack = {},
    random_access_memory = gen_array(30000, 0),

    pop_stack = function (i)
        if #i.stack == 0 then
            return 0
        end
        
        local x = i.stack[#i.stack]
        table.remove(i.stack)
        return x
    end,
    push_stack = function (i, x)
        table.insert(i.stack, x)
    end,
    execute = function (i, stmts)
        for _, v in ipairs(stmts) do
            v:run(i)
        end
    end,
    clone = function (i, upto)
        local clones = {}
        for idx=0,upto do
            local new_interpreter = clone_table(i)
            new_interpreter:push_stack(idx)
            table.insert(clones, new_interpreter)
        end
        return clones
    end
}

local function instruction_new(exec)
    return {
        run = exec,
    }
end

local function loop_new(c, b)
    return {
        cond = c,
        body = b,
        run = function (self, i)
            local bound = i:pop_stack()

            while bound ~= 0 do
                i:execute(self.cond)
                local num = i:pop_stack()

                if num == 0 or bound == 0 then
                    break
                end

                i:execute(self.body)
                bound = bound - 1
            end
        end,
    }
end

local instructions = {
    h = instruction_new(function (_, i)
        i:push_stack(0)
    end),
    p = instruction_new(function (_, i)
        local x = i:pop_stack()
        i:push_stack(x + 1)
    end),
    a = instruction_new(function (_, i)
        local address = i:pop_stack() + 1
        i:push_stack(i.random_access_memory[address])
    end),
    e = instruction_new(function (_, i)
        local address = i:pop_stack() + 1
        local value = i:pop_stack()
        i.random_access_memory[address] = value
    end),
    q = instruction_new(function (_, i)
        local number = i:pop_stack()
        i:push_stack(math.random(number + 1) - 1)
    end),
}

local function parse(code, idx)
    local stmts = {}

    while idx <= #code do
        local char = string.sub(code, idx, idx)

        if char == '#' then
            table.insert(stmts, instructions.h)
        elseif char == '+' then
            table.insert(stmts, instructions.p)
        elseif char == '@' then
            table.insert(stmts, instructions.a)
        elseif char == '!' then
            table.insert(stmts, instructions.e)
        elseif char == '?' then
            table.insert(stmts, instructions.q)
        elseif char == '|' or char == ']' then
            return stmts, idx
        elseif char == '[' then
            local body
            local condition

            condition, idx = parse(code, idx + 1)
            char = string.sub(code, idx, idx)

            if char ~= '|' then
                print("Expected '|', got '"..char.."'")
                os.exit(1)
            end

            body, idx = parse(code, idx + 1)
            char = string.sub(code, idx, idx)
            if char ~= ']' then
                print("Expected ']', got '"..char.."'")
                os.exit(1)
            end
            table.insert(stmts, loop_new(condition, body))
        end
        idx = idx + 1
    end

    return stmts, idx
end

function entry(code)
    local parsed = parse(code, 1)

    local N = 10000
    local dict = {}
    for _=1,N do
        interpreter:execute(parsed)

        local outcome = interpreter:pop_stack()
        if dict[outcome] == nil then
            dict[outcome] = 1
        else
            dict[outcome] = dict[outcome] + 1
        end
    end

    local max = 0
    local max_k = 0
    for k,v in pairs(dict) do
        if v > max then
            max_k = k
            max = v
        end
    end

    return max_k
end

return {
    e = entry
}
