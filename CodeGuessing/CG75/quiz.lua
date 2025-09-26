function skip_ws(line, i)
    while true do
        local c = line:sub(i, i)
        if c == ' ' or c == '\t' then
            i = i + 1
        else
            break
        end
    end
    return i
end

function expect(line, phrase, i, lineno)
    local start_i = i
    local phrase_len = #phrase
    for idx=1,phrase_len do
        local c1 = phrase:sub(idx, idx)
        local c2 = line:sub(i + idx - 1, i + idx - 1)

        if c1 ~= c2 then
            error("Expected \"" .. phrase .. "\" on ".. lineno .. ":" .. i)
        end
    end
    return i + phrase_len
end

function parse_quoted(line, i, lineno)
    i = expect(line, "\"", i, lineno)
    local content = ""
    local idx = 1
    while true do
        local c = line:sub(i + idx - 1, i + idx - 1)
        if c == "\"" then
            i = i + idx
            break
        elseif c == "\\" then
            local nextc = line:sub(i + idx, i + idx)
            local ch = ""
            if nextc == "\\" then
                ch = "\\"
            elseif nextc == "\"" then
                ch = "\""
            elseif nextc == "t" then
                ch = "\t"
            elseif nextc == "n" then
                ch = "\n"
            else
                error("Unknown escape sequence \"\\" .. nextc .. "\" on " .. (i + idx) .. ":" .. lineno)
            end

            content = content .. ch
            idx = idx + 1
        else
            content = content .. c
        end
        idx = idx + 1
    end
    return content, i
end

function parse_num(line, i)
    local final_num = ""
    while true do
        local c = line:sub(i, i)
        local b = string.byte(c)
        if not b then
            break
        end
        if b >= string.byte('0') and b <= string.byte('9') then
            final_num = final_num .. c
            i = i + 1
        else
            break
        end
    end
    return tonumber(final_num), i
end

function parse_ending(line, i, lineno)
    while true do
        local c = line:sub(i, i)
        if c == "\0" or c == nil or c == "#" or #c == 0 then
            break
        elseif c ~= " " and c ~= "\t" then
            error("Unexpected character '" .. c .. "' at " .. lineno .. ":" .. i)
        end
        i = i + 1
    end
    return i
end

function parse_line(line, lineno)
    local line_index = 1
    line_index = skip_ws(line, line_index)
    local first_char = line:sub(line_index, line_index)
    if first_char == '#' or #first_char == 0 then
        return nil
    end

    local question_body = ""
    local answers = {}
    local correct_idx = 0
    line_index = expect(line, "QUESTION", line_index, lineno)
    line_index = skip_ws(line, line_index)
    question_body, line_index = parse_quoted(line, line_index, lineno)
    line_index = skip_ws(line, line_index)
    line_index = expect(line, "ANSWERS", line_index, lineno)
    line_index = skip_ws(line, line_index)
    while true do
        local c = line:sub(line_index, line_index)
        if c ~= "\"" then
            break
        end

        local answer = ""
        answer, line_index = parse_quoted(line, line_index, lineno)
        table.insert(answers, answer)
        line_index = skip_ws(line, line_index)
    end
    line_index = expect(line, "CORRECT", line_index, lineno)
    line_index = skip_ws(line, line_index)
    correct_idx, line_index = parse_num(line, line_index)
    line_index = parse_ending(line, line_index, lineno)

    return {
        question = question_body,
        answers = answers,
        correct_idx = correct_idx
    }
end

function read_file(filename)
    local file = io.open(filename, "r")
    if not file then
        error("Unable to load file")
    end

    local quiz_list = {}
    local lineno = 1
    for line in file:lines() do
        local quiz = parse_line(line, lineno)
        if not quiz then
            goto continue
        end
        table.insert(quiz_list, quiz)
        lineno = lineno + 1
        ::continue::
    end

    file:close()
    return quiz_list
end

print("LyricLy quiz")
print()

local quiz_list = read_file("data.txt")
local total = #quiz_list
local correct = 0
local alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
for i,quiz in ipairs(quiz_list) do
    print(quiz.question)
    for j,answer in ipairs(quiz.answers) do
        print(alphabet:sub(j,j) .. ") " .. answer)
    end
    ::take_input::
    io.write("> ")
    local ans = io.read("*l"):upper()
    if #ans ~=1 or not (string.byte(ans) >= string.byte("A") and string.byte(ans) <= string.byte("Z")) then
        print("Invalid input!")
        goto take_input
    end

    local idx = string.byte(ans) - string.byte('A') + 1
    if idx == quiz.correct_idx then
        correct = correct + 1
    end
end

print("Quiz is over, you got " .. correct .. " out of " .. total .. " correct.")

