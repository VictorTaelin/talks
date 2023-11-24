function gen(n) {
  if (n === 0) {
    return {tag: "Leaf", x: 1};
  } else {
    const p = n - 1;
    return {tag: "Node", x0: gen(p), x1: gen(p)};
  }
}

function sum(t) {
  if (t.tag === "Leaf") {
    return t.x;
  } else {
    return sum(t.x0) + sum(t.x1);
  }
}

console.log(sum(gen(24)));
