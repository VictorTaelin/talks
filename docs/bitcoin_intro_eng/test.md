### Q: How to implement a decentralized currency?

#### (Let's ignore the "why?")

---

## Centralized case

### - Service + database

### - Trivial

```javascript
route "/send":
  if auth(from, token) && hasFunds(from, amount):
    db.atomic:
      from -= amount
      to   += amount
```

### - Paypal, WoW money, linden dollars...

---

## Decentralized case

### - *"Should be trivial too!"*

### - ...

### - 3 decades later...

---

## Attempt 1

### - Everyone has a copy of the DB.

### - Everyone has the same amount of $.

### - Transaction = "broadcast a document" (JSON).

---

## Attempt 1: DB


```javascript
Balance  | Alice | Enzo | Luana | Pedro
Alice DB | 100   | 100  | 100   | 100
Enzo DB  | 100   | 100  | 100   | 100
Luana DB | 100   | 100  | 100   | 100
Pedro DB | 100   | 100  | 100   | 100
```

---

## Attempt 1: TX

```javascript
{
  "from": "Alice",
  "to": "Luana",
  "value": 10
}
```

---

## Works?

---

## No.

### - How to prove the tx came from Alice?

```javascript
// Made and spread by Luana
{
  "from": "Alice",
  "to": "Luana",
  "value": 999999999 // mwhahahah
}
```

---

## Attempt 2:

### - Introduce auth.

---

## Attempt 2: DB

```javascript
Balance  | Alice | Enzo | Luana | Pedro
Alice DB | 100   | 100  | 100   | 100
Enzo DB  | 100   | 100  | 100   | 100
Luana DB | 100   | 100  | 100   | 100
Pedro DB | 100   | 100  | 100   | 100

Password | Alice    | Enzo           | Luana  | Pedro
Alice DB | 39aallii | minhaSenhaBIRL | 123456 | hunter2
Enzo DB  | 39aallii | minhaSenhaBIRL | 123456 | hunter2
Luana DB | 39aallii | minhaSenhaBIRL | 123456 | hunter2
Pedro DB | 39aallii | minhaSenhaBIRL | 123456 | hunter2
```

---

## Attempt 2: TX

```javascript
{
  "from": "Alice",
  "to": "Luana",
  "amount": 10,
  "password": "33aallii"
}
```

---

## Works?

---

## ...

---

## Attempt 3:

### - Introduce CRYPTO auth!

### - DBs store public keys, not passwords.

### - Transactions include *signatures*, not *passwords*.

---

## Attempt 3: DB

```javascript
Balance  | Alice | Enzo | Luana | Pedro
Alice DB | 100   | 100  | 100   | 100
Enzo DB  | 100   | 100  | 100   | 100
Luana DB | 100   | 100  | 100   | 100
Pedro DB | 100   | 100  | 100   | 100

PubKey   | Alice          | Enzo           | Luana          | Pedro
Alice DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Enzo DB  | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Luana DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Pedro DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
```

---

## Attempt 3: TX

```javascript
{
  "tx": {
    "from": "Alice",
    "to": "Luana",
    "value": 10
  },
  "sig": "0xf302fffc10a0677400ca441768289c76"
}
```

---

## Works?

---

## No.

### - Double spending attack.

### - But we're making progress.

---

## Luana gets:

```javascript
{
  "tx": {
    "from": "Alice",
    "to": "Luana",
    "amount": 100
  },
  "sig": "0xf56bcc8cb4de3edb1e5758c1f1de47c5"
}
```

## Pedro gets:

```javascript
{
  "tx": {
    "from": "Alice",
    "to": "Pedro",
    "amount": 100
  },
  "sig": "0xfe3a7060d7b850bb5b595e7034e4258e"
}
```

## Q: What transaction cames first?

---

## Attempt 4:

### - Introduce a "sorter"!

---

## Attempt 4: DB

```javascript
Balance  | Alice | Enzo | Luana | Pedro
Alice DB | 100   | 100  | 100   | 100
Enzo DB  | 100   | 100  | 100   | 100
Luana DB | 100   | 100  | 100   | 100
Pedro DB | 100   | 100  | 100   | 100

PubKey   | Alice          | Enzo           | Luana          | Pedro
Alice DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Enzo DB  | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Luana DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Pedro DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000

## Sorter's DB

N | TX
0 | {"tx":{"from":"Alice","to":"Pedro","value":100},"sig:"0xfe3a7060d7b850bb5b595e7034e4258e"}
1 | {"tx":{"from":"Pedro","to":"Alice","value":100},"sig": "0xcfdef8e815c9de6f24608489d49dfd4b"}
2 | ...
```

---

## Works?

---

## No.

### - We just introduced a central actor ("sorter").

---

## Attempt 5:

### - Pick a RANDOM sorter every X minutes! (By IP?)

---

## Works?

---

## No.

### - Easy to generate a lot of fake IPs.

### - Sybil attack.

### - BitGold.

---

## Attempt 6:

### - Pick a RANDOM sorter every X minutes! (By CPU power.)

---

## Attempt 6: DB

```javascript
Balance  | Alice | Enzo | Luana | Pedro
Alice DB | 100   | 100  | 100   | 100
Enzo DB  | 100   | 100  | 100   | 100
Luana DB | 100   | 100  | 100   | 100
Pedro DB | 100   | 100  | 100   | 100

PubKey   | Alice          | Enzo           | Luana          | Pedro
Alice DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Enzo DB  | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Luana DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000
Pedro DB | 0xAAAA1111EEEE | 0xEEEE33330000 | 0xAAAA3333AAAA | 0xEEEE66660000

N | Miner | POW        | Tx
0 | Alice | 0x00008547 | {"tx":{"from":"Alice","to":"Pedro","amount":100},"sig":"0xfe3a7060d7b850bb5b595e7034e4258e"}
1 | Pedro | 0x0000ea86 | {"tx":{"from":"Alice","to":"Luana","amount":100},"sig":"0xf56bcc8cb4de3edb1e5758c1f1de47c5"}
2 | Luana | 0x00003c52 | {"tx":{"from":"Pedro","to":"Alice","amount":100},"sig":"0xcfdef8e815c9de6f24608489d49dfd4b"}
3 | ...   | ...        | ...
```

Notice the "POW" field. That's just a hash of the DB with `N` leading zeroes.

It proves you got selected!

---

## Works?

---

## Yes!

### Can't to duplicate CPU power, perfect anti-sybil.

### *"Sorting"* is not buzz enough. Call that **mining**.

### Group txs in blocks, make a chain, call that a *blockchain*.

### You could make a religion out of it.

### JK: mining is not that special. It is just for ordering!

---


## But wait, it really works?

---

## Maybe...

### MtGox: centralized (fucked up).

### Silk road: centralized (fucked up).

### Satoshi dice: centralized (fees + could fuck it up).

### Coinbase: centralized (fees).

### BitGold: centralized (fees).

### BitBet: centralized (fees).

---

## Q: How to implement a decentralized *computer*?
