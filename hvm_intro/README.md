# HVM: o futuro da computação paralela

| Higher Order Company |
|----------------------|
| ○          ╭──────╮  |
| │         ╭╯      │  |
| ╰────│╲  ╭╯ ╱│────╯  |
|      │⸗│─┼─│ │       |
| ╭────│╱ ╭╯  ╲│────╮  |
| │      ╭╯         │  |
| ╰──────╯          ●  |












> Victor Taelin

# Trend paralelismo - hardware

| Ano  | Empresa | Modelo                           | Núcleos    |
|------|---------|----------------------------------|------------|
| 2003 | Intel   | Pentium 4 Extreme 3.2GHz         | 1 núcleo   |
| 2007 | Intel   | Core 2 Extreme QX6800            | 4 núcleos  |
| 2011 | AMD     | FX-8150                          | 8 núcleos  |
| 2015 | Intel   | Core i7-6700K                    | 4 núcleos  |
| 2019 | AMD     | Ryzen 9 3950X                    | 16 núcleos |
| 2023 | AMD     | Ryzen Threadripper 7980X         | 64 núcleos |

> CPUs de Consumo com Maior Número de Núcleos por ano, Intel e AMD, 2003-2023

# Trend paralelismo - software

| Linguagem  | % paralelo |
|------------|------------|
| C          |        X % |
| Go         |        X % |
| Haskell    |        X % |
| Java       |        X % |
| JavaScript |        X % |
| Python     |        X % |
| Rust       |        X % |

> Percentual de projetos no top 1000 do GitHub usando paralelismo, por linguagem

# Por que?

**Programação paralela é difícil!**

- Sincronização e comunicação entre threads

- Deadlocks e condições de corrida

- Dificuldade em testar e depurar código paralelo

- Complexidade na estruturação do código

"vc tem 2 problemas"

# GPT programando sequencial

> Implement a C program that creates a tree with ints from 0 to 2^24-1 and then sums all.

```c
long long sumTree(Node* root) {
    if (root == NULL) return 0;
    return root->data + sumTree(root->left) + sumTree(root->right);
}
```

**Tamanho : 4 linhas**
**Tempo   : 0.537 segundos**

# GPT programando paralelo

> Create the same program, but now using 16 threads to accelerate the computation.

```c
void* sumTree(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    Node* root = threadArgs->root;
    if (root == NULL) {
        threadArgs->sum = 0;
        return NULL;
    }
    ThreadArgs leftArgs, rightArgs;
    pthread_t leftThread, rightThread;
    leftArgs.root = root->left;
    rightArgs.root = root->right;
    pthread_create(&leftThread, NULL, sumTree, &leftArgs);
    pthread_create(&rightThread, NULL, sumTree, &rightArgs);
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);
    threadArgs->sum = root->data + leftArgs.sum + rightArgs.sum;
    return NULL;
}
```

**Tamanho : 18 linhas**
**Tempo   : 2 meses**

*https://chat.openai.com/share/805b2df2-a9ea-4a1c-b52b-bb69fa7f624b*

# Por que linguagens não usam todos os núcleos?

- Linguagens são modeladas para 1 núcleo (loops são sequenciais)

- Overheads podem não compensar (spawn, locks, atomics...)

- Incompatível com referências mutáveis (race conditions, etc.)

```c
// se duas threads executam essa função, pode
// ser que *x só seja incrementado uma vez!
void fn(int *x) {
  int val = *x;
  *x = val + 1;
}
```

- Análise **estática** não funciona. É necessário informação **dinâmica**!

```c
// paralelizar ou não? depende do momento...
for (int i = 0; i < limit; ++i) {
  arr[i] *= 2;
}
```

- Considerado problema de pesquisa, sem resultados suficientemente bons.

# HVM: um runtime massivamente paralelo

- Runtimes são "programas que rodam programas"

- HVM executa linguagens de *alto nível* com paralelismo automático

- Completamente geral (em contraste com soluções data-parallel/array-based)

- Funcional: lambdas, recursividade, tipos algébricos, pattern-matching...

- Procedural: loops, branching, efeitos, "mutabilidade pura"...

- **A promessa: se a sua lógica não é sequencial, seu executável será paralelo!**

- Protótipo lançado ano passado, 1a versão estável em algumas semanas!

- Para ilustrar, vamos comparar tempos de execução de "programas equivalentes"

# Somando árvores - Python

```python
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
```

# Somando árvores - JavaScript

```javascript
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
```

# Somando árvores - C

```c
Tree* gen(unsigned int n) {
  if (n == 0) {
    return Leaf(1);
  } else {
    unsigned int p = n - 1;
    return Node(gen(p), gen(p));
  }
}

unsigned int sum(Tree* t) {
  if (t->tag == LeafTag) {
    return t->x;
  } else {
    return add(sum(t->x0), sum(t->x1));
  }
}

int main() {
  printf("%u\n", sum(gen(24)));
  return 0;
}
```

# Somando árvores - HVM

```haskell
data Tree = (Leaf x) | (Node x0 x1)

gen = λn match n {
  0: (Leaf 1)
  +: (Node (gen n-1) (gen n-1))
}

sum = λt match t {
  Leaf: t.x
  Node: (+ (sum t.x0) (sum t.x1))
}

main = (sum (gen 24))
```

# Resultados

| Linguagem  | Tempo   |
|------------|---------|
| Python     | 29.001s |
| JavaScript |  1.142s |
| C          |  0.395s |
| HVM        |  0.237s |

> Apple M3 Max, max opt lv, md5

# Como é possível?

[ ] A HVM foi feita pelo GPT-6

[ ] Eu roubei nos benchmarks

[ ] A HVM é mais rápida que C

[X] Paralelismo

> O Apple M3 Max tem 12 núcleos de performance

# Tempo X Núcleos
                                                         🌙 Python
    Tempo                                                    
    ~2.16s ┼───╮
    ~2.00s ┤   │
    ~1.84s ┤   │
    ~1.67s ┤   │
    ~1.51s ┤   │
    ~1.35s ┤   │
    ~1.19s ┼───│─────────────────────────────────────────── JavaScript
    ~1.03s ┤   ╰───╮
    ~0.86s ┤       ╰───╮
    ~0.70s ┤           ╰───╮
    ~0.54s ┤               ╰───────╮
    ~0.38s ┼───────────────────────╰───────────────╮─────── C
    ~0.22s ┤                                       ╰─────── HVM
           0   1   2   3   4   5   6   7   8   9  10  11  12
                                Núcleos

- Ainda há muito a melhorar em 1 núcleo!

- Uso manual e correto de threads em C sempre será mais rápido!

- Então por que? Paralelismo correto, sem esforço, em linguagens alto nível

- Lambdas, referências compartilhadas, comunicação... a HVM cuida de **tudo**

# Qual o segredo?

Muito simples!

- 1. Pegamos um programa de alto nível (Python, JavaScript, etc.)

- 2. Transformamos em um termo do **Cálculo Lambda**

- 3. Transformamos em um grafo de **Combinadores de Interação**

- 4. Aplicamos **regras de reescrita paralelas**, até acabar

- 5. Convertemos de volta para um termo do **Cálculo Lambda**

- 6. Convertemos de volta pra **Combinadores de Interação**

- 7. Printamos resultados, lançamos chamadas de sistema, etc.

> ????????? ??      ? ? ?           ?

> Ok, vamos por partes...

# O que é o Cálculo Lambda?

- O Cálculo Lambda é uma linguagem que contém apenas lambdas

- Um lambda é uma função anônima. Exemplo:

    Na matemática, temos...

    f(x) = x * 2

    No cálculo lambda, temos...

    λx. (x * 2)

    É a mesma coisa, só que sem nome!

- É possível representar QUALQUER algoritmo no Cálculo Lambda!

- Você já usa ele e nem sabe disso! Quer ver?

# Redescobrindo o Cálculo Lambda...

> Começamos com programinha aleatório...

```python
def show(arr):
    print(str(arr))

nums = [0, 1, 2, 3, 4]
func = lambda x: x * 2

if func is not None:
    for i in range(len(nums)):
        nums[i] = func(nums[i])

show(nums) # [0, 2, 4, 6, 8]
```

# Redescobrindo o Cálculo Lambda...

> Tiramos o "if"...

```python
def show(arr):
    print(str(arr))

nums = [0, 1, 2, 3, 4]
func = lambda x: x * 2

for i in range(len(nums)):
    nums[i] = func(nums[i])

show(nums) # [0, 2, 4, 6, 8]
```

# Redescobrindo o Cálculo Lambda...

> Tiramos o "for"...

```python
def show(arr):
    print(str(arr))

nums = [0, 1, 2, 3, 4]
func = lambda x: x * 2

nums[0] = func(nums[0])
nums[1] = func(nums[1])
nums[2] = func(nums[2])
nums[3] = func(nums[3])
nums[4] = func(nums[4])

show(nums) # [0, 2, 4, 6, 8]
```

# Redescobrindo o Cálculo Lambda...

> Tiramos o "array"...

```python
def show(num):
    print(str(num))

num0 = 0
num1 = 1
num2 = 2
num3 = 3
num4 = 4
func = lambda x: x * 2

num0 = func(num0)
num1 = func(num1)
num2 = func(num2)
num3 = func(num3)
num4 = func(num4)

show(num0) # 0
show(num1) # 2
show(num2) # 4
show(num3) # 6
show(num4) # 8
```

# Redescobrindo o Cálculo Lambda...

> Tiramos os números!

```python
def show(num):
    print(str(num(lambda x: x+1)(0)))

num0 = lambda s: lambda z: z
num1 = lambda s: lambda z: s(z)
num2 = lambda s: lambda z: s(s(z))
num3 = lambda s: lambda z: s(s(s(z)))
num4 = lambda s: lambda z: s(s(s(s(z))))
func = lambda x: lambda s: lambda z: x(lambda x: s(s(x)))(z)

num0 = func(num0)
num1 = func(num1)
num2 = func(num2)
num3 = func(num3)
num4 = func(num4)

show(num0) # 0
show(num1) # 2
show(num2) # 4
show(num3) # 6
show(num4) # 8
```

> Pronto, esse é o Cálculo Lambda!

# De Linguagem X para Cálculo Lambda

| Feature     | Python             | Cálculo Lambda                       |
|-------------|--------------------|--------------------------------------|
| número      | 2                  | λs. λz. (s (s (s z)))                |
| adição      | 10 + 20            | λs. λz. (10 s (20 s z))              |
| produto     | 10 * 20            | λs. (10 (20 s))                      |
| exponencial | 10 ^ 20            | (20 10)                              |
| boleano     | true               | λt. λf. t                            |
| condicional | x if c else y      | (c x y)                              |
| tupla       | (1, 2)             | λt. (t 1 2)                          |
| tripla      | (1, 2, 3)          | λt. (t 1 2 3)                        |
| projeção    | (a, b) = (1, 2); X | (λt.(t 1 2) λa. λb. X)               |
| lista       | [1, 2, 3, ...]     | λc. λn. (c 1 (c 2 (c 3 ... n)))      |
| map         | [f(x) in list]     | (list λh.λt.λc.(c (f h) t) λc.λn.n)  |
| indexação   | list[2]            | (head (2 tail list))                 |
| loop        | while x: f()       | (Y λW λc λs λf (if (c s) (W c (f ... |
| efeitos     | print("oi")        | monadic binding to external env...   |
| ...         | ...                | ...                                  |

- TODO conceito pode ser representado, de uma forma ou de outra!

- Diversas maneiras de representar cada conceito, com diferentes méritos

- Difícil? Talvez! Mas você não precisar usar o Cálculo Lambda diretamente.

- O paradigma funcional é baseado nele. Haskell, Lean, Clojure, Elixir...

# Porém, o Cálculo Lambda tem suas limitações...

- A regra de "substituição" não é atômica: ineficiente / anti-paralelismo

- Outros modelos como Máquina Turing, Automatas Celulares: problemas similares

- Pergunta: "quais são as regras fundamentais da computação?" - Yves Lafont, 1997

- Resposta: aniquilação e comutação, ou seja, *Combinadores de Interação*

- Um modelo computacional paralelo com 3 símbolos e 6 regras de reescrita

- Emula outros sistemas *sem perda de performance*! "Super Turing Completo"

- **Capaz de computar termos lambda otimamente, e em paralelo!**

# Interaction Combinators: 3 symbols, 6 interactions

           Eraser (ERA)          Constructor (CON)        Duplicator (DUP)       
                                                                                 
                                        /|--                      /|--           
              O----                  --| |                     --|#|             
                                        \|--                      \|--           
                                                                                 
   ERA-CON                              │ ERA-ERA                                
             /|---    O-----            │                                        
        O---| |    =>                   │      O-----O     =>       nothing      
             \|---    O-----            │                                        
────────────────────────────────────────┼────────────────────────────────────────
   ERA-DUP                              │  CON-DUP                /|-------|\    
                                        │                     ---|:|       | |---
             /|---    O-----            │  ---|\     /|---        \|--, ,--|/    
        O---|:|    =>                   │     | |---|:|    =>          X         
             \|---    O-----            │  ---|/     \|---        /|--' '--|\    
                                        │                     ---|:|       | |---
                                        │                         \|-------|/    
────────────────────────────────────────┼────────────────────────────────────────
   CON-CON                              │  DUP-DUP                               
                                        │                                        
   ---|\     /|---    ---, ,---         │  ---|\     /|---    ---, ,---          
      | |---| |    =>     X             │     |:|---|:|    =>     X              
   ---|/     \|---    ---' '---         │  ---|/     \|---    ---' '---          

# Do Lambda Calculus para Interaction Combinators

          Lambda Calculus => Interaction Combinator
          =========================================

      X = λvar. body                X = (fun arg)
      --------------                -------------   
    
       var --|\                       X --|\
             | |-- X                      | |-- fun
      body --|/                     arg --|/
                                                     
                                                     
  *"um lambda vira um CON"*     *"uma aplicação vira um CON"*

       λx. (vários x)               λx. (nenhum x)
       --------------               --------------  
       
         x --|\
             |#|-- x                    O-- x
         x --|/

  *"duplicamos vars com DUPs"*  *"apagamos vars com ERAs"*

# HVM: exemplo completo

> 1. Pegamos um programa de alto nível

```python
def dobro(x):
    return x * 2

print(dobro(1))
```

# HVM: exemplo completo

> 2. Transformamos em um termo lambda

```haskell
let dobro = λx.
    λs.λz.((x λk(s (s k))) z)

(dobro λs.λz.(s z))
```

# HVM: exemplo completo

> 3. Transformamos em Interaction Combinators

                .............              
                :           :              
               /_\         /_\             
          .....: :  .......: :..           
          :      X  :         /_\          
         /_\       /_\      ..: :..        
      ...: :..     : :.     :     :        
      :      :    /_\ :    /#\   /_\       
     /_\    /_\   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  /_\ /_\ :  .... : 
       :......:   :.:.:..: :.: : : /_\ : : 
                    :.:........: :.: :.:.: 
                      :................:   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

                +++++++++++++              
                +           +              
               /_\         /_\             
          .....: :  .......: :..           
          :      X  :         /_\          
         /_\       /_\      ..: :..        
      ...: :..     : :.     :     :        
      :      :    /_\ :    /#\   /_\       
     /_\    /_\   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  /_\ /_\ :  .... : 
       :......:   :.:.:..: :.: : : /_\ : : 
                    :.:........: :.: :.:.: 
                      :................:   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

               .............               
               : ..........:..             
               : :         : :             
          .....: :  .......: :..           
          :      X  :         /_\          
         /_\       /_\      ..: :..        
      ...: :..     : :.     :     :        
      :      :    /_\ :    /#\   /_\       
     /_\    /_\   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  /_\ /_\ :  .... : 
       :......:   :.:.:..: :.: : : /_\ : : 
                    :.:........: :.: :.:.: 
                      :................:   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

                                           
                                           
                               X           
          ...........          :           
          :         :         /_\          
         /_\       /_\      ..: :..        
      ...: :..     : :.     :     :        
      :      :    /_\ :    /#\   /_\       
     /_\    /_\   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  /_\ /_\ :  .... : 
       :......:   :.:.:..: :.: : : /_\ : : 
                    :.:........: :.: :.:.: 
                      :................:   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

                                           
                                           
                               X           
          +++++++++++          :           
          +         +         /_\          
         /_\       /_\      ..: :..        
      ...: :..     : :.     :     :        
      :      :    /_\ :    /#\   /_\       
     /_\    /_\   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  /_\ /_\ :  .... : 
       :......:   :.:.:..: :.: : : /_\ : : 
                    :.:........: :.: :.:.: 
                      :................:   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

                                           
             ..........                    
             :        :        X           
      .......:......  :        :           
      :      :     :  :       /_\          
      :      :     :  :     ..: :..        
      :      :     :  :     :     :        
      :      :    /_\ :    /#\   /_\       
     /_\    /_\   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  /_\ /_\ :  .... : 
       :......:   :.:.:..: :.: : : /_\ : : 
                    :.:........: :.: :.:.: 
                      :................:   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

                                           
             ++++++++++                    
             +        +        X           
      ++++++++++++++  +        :           
      +      +     +  +       /_\          
      +      +     +  +     ..: :..        
      +      +     +  +     :     :        
      +      +    /_\ +    /#\   /_\       
     /_\    /_\   : : +   .: :.  : :...... 
     : :    : :   : : +   :   :  :       : 
     :.:....: :   : : +  /_\ /_\ :  ++++ : 
       :......:   :.:.+..: :.: : : /_\ + : 
                    :.+........: :.: :.+.: 
                      ++++++++++++++++++   

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

              ...........                  
            ..:........ :                  
       .....:.:...... : :      X           
     ..:....:.:.... : : :      :           
     : :    : :   : : : :     /_\          
     : :    : :   : : : :   ..: :..        
     : :    : :   : : : :   :     :        
     : :    : :   : : : :  /#\   /_\       
     : :    : :   : : : : .: :.  : :...... 
     : :    : :   : : : : :   :  :       : 
     :.:....: :   : : : :/_\ /_\ :       : 
       :......:   :.:.:.:: :.: : :       : 
                    :.:.:......: :       : 
                      :.:........:       : 
                        :................: 

# HVM: exemplo completo

> 4. Aplicamos **regras de interação paralelas**, até acabar

                                           
                                           
                               X           
                               :           
                              /_\          
                            ..: :..        
                            :     :        
                           /#\   /_\       
                          .: :.  : :       
                          :   :  : :       
                         /_\ /_\ : :       
                         : :.: :.:.:       
                         :.......:         

# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                           
                                           
                               X           
                               :           
                               λs          
                                :..        
                                  :        
                                  λz       
                       ....   .....:       
                       :  :   :            
                       : (@) (@)           
                       : : : : :           
                       : s z s :           
                       :.......:           
                                           
# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                           
                                           
                               X           
                               :           
                               λs          
                                :..        
                                  :        
                                  λz       
                                   :       
                                   :       
                                  (@)      
                                  : :.     
                                  s  :     
                                    (@)    
                                    : :    
                                    s z    

# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                           
                                           
                               X           
                               :           
                               λs          
                                :..        
                                  λz       
                                   :       
                                  (@)      
                                  : :.     
                                  s (@)    
                                    : :    
                                    s z    

# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                           
                                           
                               X           
                               :           
                               λs..        
                                  λz.      
                                  (@).     
                                  : (@)    
                                  s : :    
                                    s z    

# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                                
                                                
                               X                
                               :                
                               λs.λz.(@).(@)..z
                                      :   :    
                                      s   s    
                                                
                                                
                                                
                                                
                                                
                                                

# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                                   
                                                   
                               X...λs.λz.(@).(@)..z
                                          :   :    
                                          s   s    
                                                   
                                                   
                                                   
                                                   
                                                   
                                                   

# HVM: exemplo completo

> 5. Convertemos de volta pro cálculo lambda



                               X = λs.λz.(s (s z))

# HVM: exemplo completo

> 6. Convertemos de volta pra a linguagem original


```python
                               X = 2
```

# HVM: exemplo completo

> 7. Printamos resultados, lançamos chamadas de sistema, etc.


```python
                               print(2)
```

# HVM é uma implementação eficiente

> "Tudo isso para somar 2 números?"

- Pointeiros de 64-bit: 128 bits por lambda!

- Números "u60" unboxed, com operações nativas

- Arrays representadas com árvores: 46% de eficiência

- Tipos algébricos representados com Scott-Encodings

- Definições globais compiladas para código nativo

- Diversas otimizações => boa performance real

- Muito a melhorar, porém já é competitivo


# Resumo

1. Processadores estão ficando cada vez mais paralelos

2. Programas, em geral, não tem acompanhado a trend

3. Motivo: programar paralelamente é difícil e custoso

4. Abordagens para paralelismo automático não tiveram êxito

5. HVM: paralelismo automático com resultados promissores

6. Baseada em um modelo computacional projetado para paralelismo

7. **programa -> λterm -> icomb -> eval() -> λterm -> programa**

# Obrigado!

A HOC é uma empresa brasileira inovando no âmbito tecnológico.

Temos uma comunidade de apaixonados por computação.

Entusiastas de todos os níveis são bem-vindos!

Discord: https://discord.HigherOrderCO.com/

Minhas redes:

- Twitter: @VictorTaelin

- Instagram: @VictorTaelin

- GitHub: @VictorTaelin

- Reddit: /u/SrPeixinho

-------------------------------------------------------------------------------------------------------
