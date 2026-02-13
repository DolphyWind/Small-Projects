from typing import List
import requests as req
from hashlib import sha256


def entry(round_number: int, participant_ids: List[int]) -> int:
    # Server should always be able to send anonymous requests to the cg website
    r = req.get(f"https://codeguessing.gay/{round_number}/")
    if not r.status_code.__str__().startswith('2'):
        raise RuntimeError("Try again later")

    hash = sha256(r.text.encode())
    winner = int(hash.hexdigest(), 16) % participant_ids.__len__()
    return participant_ids[winner]
