def get_permutation_data(s):
    out_str = ''
    out_table = {}
    i = 0
    for c in s:
        if c not in out_table.keys():
            out_table[c] = i
            i += 1
        out_str = out_str + str(out_table[c])

    return out_str, out_table

def entry(text, all_words):
    text = text.lower()
    words = text.split()
    table = entry_helper(words, all_words, {})
    if table is None:
        return None
    inv_table = {v: k for k, v in table.items()}
    inv_table = {ord(k): ord(v) for k, v in inv_table.items()}
    return text.translate(inv_table)

def entry_helper(words, all_words, table):
    if len(words) == 0:
        return table

    word = words[0]
    rest = words[1:]
    word_id, word_mapping = get_permutation_data(word)
    word_inv = {v: k for k, v in word_mapping.items()}

    for w in all_words:
        if len(w) != len(word):
            continue

        w_id, w_mapping = get_permutation_data(w)
        if w_id == word_id:
            w_inv = {v: k for k, v in w_mapping.items()}
            new_mapping = {}
            for i in range(len(w_inv.keys())):
                new_mapping[w_inv[i]] = word_inv[i]

            for k, v in new_mapping.items():
                if k in table.keys():
                    if table[k] != v:
                        break
                else:
                    if v in table.values():
                        break
            else:
                old_table = table.copy()
                for k, v in new_mapping.items():
                    table[k] = v
                result = entry_helper(rest, all_words, table)
                if result is not None:
                    return result
                table = old_table
            
    return None
