#!/usr/bin/env python3
from typing import Dict, List
import json
import sys

import tiktoken


def read_file(path: str) -> str:
    with open(path, "r", encoding="utf-8") as f:
        return f.read()


def write_json(path: str, obj) -> None:
    with open(path, "w", encoding="utf-8") as f:
        json.dump(obj, f, ensure_ascii=False)


def main(input_path: str = "text.txt") -> None:
    text = read_file(input_path)

    encoding = tiktoken.get_encoding("o200k_base")
    token_ids: List[int] = encoding.encode(text)

    unique_token_ids: List[int] = []
    seen: Dict[int, bool] = {}
    for tid in token_ids:
        if tid not in seen:
            seen[tid] = True
            unique_token_ids.append(tid)

    token_id_to_enum: Dict[int, int] = {
        tid: i for i, tid in enumerate(unique_token_ids)
    }

    token_text_to_enum: Dict[str, int] = {}
    for tid, enum_idx in token_id_to_enum.items():
        token_text = encoding.decode([tid])
        token_text_to_enum[token_text] = enum_idx

    enum_to_token_text: Dict[str, str] = {
        str(enum_idx): token_text
        for token_text, enum_idx in token_text_to_enum.items()
    }

    write_json("token_id_to_enum.json", token_id_to_enum)
    write_json("token_text_to_enum.json", token_text_to_enum)
    write_json("enum_to_token_text.json", enum_to_token_text)
    print([token_id_to_enum[i] for i in token_ids])

    print(f"tokens total: {len(token_ids)}")
    print(f"unique token ids: {len(unique_token_ids)}")
    print("wrote: token_id_to_enum.json, token_text_to_enum.json, enum_to_token_text.json")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        main()
