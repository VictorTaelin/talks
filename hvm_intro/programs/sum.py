def gen(n: int) -> dict:
    if n == 0:
        return {"tag": "Leaf", "x": 1}
    else:
        p = n - 1
        return {"tag": "Node", "x0": gen(p), "x1": gen(p)}

def sum_tree(t: dict) -> int:
    if t["tag"] == "Leaf":
        return t["x"]
    else:
        return sum_tree(t["x0"]) + sum_tree(t["x1"])

print(sum_tree(gen(24)))
