### Como implementar uma moeda digital?

---

## Caso centralizado

### - Serviço + banco-de-dados

### - Trivial

```javascript
route "/send":
  if auth(from, token) && hasFunds(from, amount):
    db.atomic:
      from -= amount
      to   += amount
```

### - Paypal, Linden Dollar, milhas, pontos...

---

## Caso descentralizado

### - *"Parece trivial!"*

### - ...

### - Décadas depois...

---

## Tentativa 1

### - Todos armazenam uma cópia do DB.

### - Todos começam com a mesma quantia.

### - Transação = JSON (broadcast).

---

## Tentativa 1: DB


```javascript
Balance  | Alice | Enzo | Luana | Pedro
Alice DB | 100   | 100  | 100   | 100
Enzo DB  | 100   | 100  | 100   | 100
Luana DB | 100   | 100  | 100   | 100
Pedro DB | 100   | 100  | 100   | 100
```

---

## Tentativa 1: TX

```javascript
{
  "de": "Alice",
  "para": "Luana",
  "quantia": 10
}
```

---

## Funciona?

---

## Não.

### - Como provar que a tx veio da Alice?

```javascript
// Criada e espalhada por Luana
{
  "de": "Alice",
  "para": "Luana",
  "quantia": 999999999 // mwhahahah
}
```

---

## Tentativa 2:

### - Tentativa 1 + autenticação.

---

## Tentativa 2: DB

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

## Tentativa 2: TX

```javascript
{
  "de": "Alice",
  "para": "Luana",
  "quantia": 10,
  "senha": "33aallii"
}
```

---

## Funciona?

---

## ...

---

## Tentativa 3:

### - Tentativa 1 + autenticação criptográfica.

### - DBs armazenam chaves públicas.

### - Transações incluem *assinatura*, não *senha*.

---

## Tentativa 3: DB

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

## Tentativa 3: TX

```javascript
{
  "tx": {
    "de": "Alice",
    "para": "Luana",
    "quantia": 10
  },
  "sig": "0xf302fffc10a0677400ca441768289c76"
}
```

---

## Funciona?

---

## Ainda não.

### - Double spending attack.

---

## Luana recebe:

```javascript
{
  "tx": {
    "de": "Alice",
    "para": "Luana",
    "quantia": 100
  },
  "sig": "0xf56bcc8cb4de3edb1e5758c1f1de47c5"
}
```

## Pedro recebe:

```javascript
{
  "tx": {
    "de": "Alice",
    "para": "Pedro",
    "quantia": 100
  },
  "sig": "0xfe3a7060d7b850bb5b595e7034e4258e"
}
```

## Q: Qual transação veio primeiro?

---

## Tentativa 4:

## - Tentativa 3 + FTL communication.

---

## Funcionaria?

---

## Sim!

---

## Tentativa 5:

### - Tentativa 3 + ordenador.

---

## Tentativa 5: DB

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

## DB do ordenador

N | TX
0 | {"tx":{"de":"Alice","para":"Pedro","quantia":100},"sig:"0xfe3a7060d7b850bb5b595e7034e4258e"}
1 | {"tx":{"de":"Pedro","para":"Alice","quantia":100},"sig": "0xcfdef8e815c9de6f24608489d49dfd4b"}
2 | ...
```

---

## Funciona?

---

## Não.

### - Ordenador centralizado.

---

## Tentativa 6:

### - Tentativa 3 + ordenadores escolhidos aleatoriamente por IP.

---

## Funciona?

---

## Não.

### - Fácil de gerar milhares de IPs.

### - Sybil attack = forced centralization.

### - BitGold.

---

## Tentativa 7:

### - Tentativa 3 + ordenadores escolhidos aleatoriamente por wasted-CPU-time.

---

## Tentativa 7: DB

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
0 | Alice | 0x00008547 | {"tx":{"de":"Alice","para":"Pedro","quantia":100},sig:"0xfe3a7060d7b850bb5b595e7034e4258e"}
1 | Pedro | 0x0000ea86 | {"tx":{"de":"Alice","para":"Luana","quantia":100},"sig":"0xf56bcc8cb4de3edb1e5758c1f1de47c5"}
2 | Luana | 0x00003c52 | {"tx":{"de":"Pedro","para":"Alice","quantia":100},"sig":"0xcfdef8e815c9de6f24608489d49dfd4b"}
3 | ...   | ...        | ...
```

---

## Funciona?

---

## Sim!

### Impossível multiplicar wasted-CPU-time.

#### Detalhe: chamamos ordenadores de "mineradores" (porque é c00l).

#### Detalhe: colocamos várias transações por linha e chamamos de bloco.

#### Detalhe: chamamos a tabela de blocos de blockchain (porque é buzz).

#### Detalhe: dificuldade ajustada para termos 1 linha (bloco) por 10 mins.

### Bitcoin!

---


## Mas funciona mesmo?

---

## Talvez...

### MtGox: centralizado (deu ruim).

### Silk road: centralizado (deu ruim).

### Satoshi dice: centralizado (fees... pode dar ruim).

### Coinbase: centralizado (fees).

### BitGold: centralizado (fees).

### BitBet: centralizado (fees).

### Anonimato? Escalabilidade? Herança? Contratos?

---

## Tentativa 8

### - Tentativa 7 (Bitcoin) + scripting.

---

## Tentativa 9: TX

```javascript
{
  "tx": {
    "de": "Alice",
    "para": "Contrato 7",
    "metodo": "send_token",
    "metodo_args": {
      "to": "Luana",
      "quantia": 100
    }
  },
  "sig": "0x12ffae23746c2115c40937a30dc0ddab"
}
```

---

## Tentativa 9: DB

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

State    | Contract 0        | Contract 1   | Contract 2 
Alice DB | 01001010011011... | 010110001001 | 001110011010
Enzo DB  | 01001010011011... | 010110001001 | 001110011010
(...)

N | Sorter | POW        | Tx
0 | Alice  | 0x00008547 | {"tx":{"de":"Alice","para":"Contrato7","metodo":"send_token","metodo_args":{"to":"Luana","quantia":100}},"sig":"0x12ffae23746c2115c40937a30dc0ddab"}
1 | Enzo   | 0x00001234 | {"tx":{"de":"Enzo","para":"Contrato1","metodo":"send_token","metodo_args":{"to":"Alice","quantia":100}},"sig":"0x123123123123123123abcabcabcabcab"}
(...)
```

---

## Funciona?

---

## [descubra]