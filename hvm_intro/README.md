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

# Paralelismo automático: desafios

- Linguagens são modeladas para 1 núcleo (loops são sequenciais)

- Overheads podem não compensar (spawn, locks, atomics...)

- Incompatível com referências mutáveis (race conditions, etc.)

```c
// duas threads executam
// x incrementado 1 vez?
void fn(int *x) {
  int val = *x;
  *x = val + 1;
}
```

- Análise **estática** não funciona. É necessário informação **dinâmica**!

```c
// parallelizar ou não?
// depende da execução!
for (int i = 0; i < limit; ++i) {
  arr[i] *= 2;
}
```

- Considerado problema de pesquisa, sem resultados suficientemente bons.

# HVM: um runtime massivamente paralelo

- Runtimes, tal como VMs, são "programas que rodam programas"

- HVM executa linguagens de *alto nível* com paralelismo automático

- Funcional: lambdas, recursividade, tipos algébricos, pattern-matching...

- Procedural: loops, branching, efeitos, "mutabilidade pura"...

- Promessa: se a sua *lógica* não é sequencial, seu *executável* será paralelo 

- Protótipo lançado ano passado, 1a versão estável em algumas semanas!

# Exemplo: somando árvores - Python

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

print(sum_tree(gen(21)))
```

# Exemplo: somando árvores - JavaScript

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

# Exemplo: somando árvores - C

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

# Exemplo: somando árvores - HVM

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

[ ] Eu sou um gênio

[ ] Eu roubei nos benchmarks

[ ] A HVM é mais rápida que C

[X] Paralelismo

> O Apple M3 Max tem 12 núcleos de performance

# Tempo X Núcleos
                                                         🌙 Python
    tempo                                                    
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
                                núcleos

- Ainda há muito a melhorar em 1 núcleo!

- Uso manual e correto de threads em C **será** mais rápido!

- O ponto é: paralelismo correto, sem esforço, com features de alto nível

- Implementar certos algoritmos em paralelo na mão beira o impossível

- Lambdas, dados compartilhados, comunicação... a HVM cuida de **tudo**

# Como funciona?

- A HVM é baseada em Combinadores de Interação

- Inventados para otimizar o Cálculo Lambda

- Acabou se mostrando muito mais fundamental

- Enraizado na lógica, formente confluente, inerentemente paralelo

> Este artigo aborda a seguinte questão: quais são as leis fundamentais da
> computação? Máquinas de Turing modelam apenas computação sequencial. Autômatos
> celulares modelam computação distribuída, mas com sincronização global.
> Sistemas de reescrita como o cálculo-λ têm uma interpretação lógica
> interessante. No entanto, a regra de β-redução é mais complicada do que
> parece: é razoável considerar "substituição" como uma operação atômica? Um
> sistema mais primitivo é a lógica combinatória, mas, novamente, é razoável
> considerar apagamento e duplicação de árvores completas como operações
> atômicas? Redes de interação foram introduzidas como um modelo de computação
> distribuída com sincronização local. Do ponto de vista da computabilidade,
> redes de interação são equivalentes às máquinas de Turing, mas, do ponto de
> vista da computação, há algo mais; por exemplo, paralelismo. Para expressar
> isso rigorosamente, introduzimos uma noção de tradução de sistemas preservando
> as propriedades essenciais da computação, como a complexidade e o grau de
> paralelismo. É mostrado que um sistema muito simples de combinadores de
> interação, com apenas três símbolos e seis regras, é um modelo universal de
> computação distribuída. Isso sugere uma resposta à nossa pergunta original: as
> leis fundamentais da computação são comutação e aniquilação.

> Lafont, Yves. “Interaction Combinators.” Inf. Comput. 137 (1997): 69-101.

# Interaction Combinators

                          Symbols (Graph Nodes)                              
                          =====================                              
                                                                             
       Eraser (ERA)         Constructor (CON)         Duplicator (DUP)       
                                                                             
                                    /|--                      /|--           
          O----                 O--| |                    O--|#|             
                                    \|--                      \|--           
                                                                             
                       Interactions (Rewrite Rules)                          
                       ============================                          
                                                                             
    ERA-CON                            ERA-ERA                               
                                                                             
             /|---    O-----               O-----O     =>       nothing      
        O---| |    =>                                                        
             \|---    O-----                                                 
                                       CON-DUP                /|-------|\    
    ERA-DUP                                               ---|#|       | |---
                                       ---|\     /|---        \|--, ,--|/    
             /|---    O-----              | |---|#|    =>          X         
        O---|#|    =>                  ---|/     \|---        /|--' '--|\    
             \|---    O-----                              ---|#|       | |---
                                                              \|-------|/    
    CON-CON                            DUP-DUP                               
                                                                             
    ---|\     /|---    ---, ,---       ---|\     /|---    ---, ,---          
       | |---| |    =>     X              |#|---|#|    =>     X              
    ---|/     \|---    ---' '---       ---|/     \|---    ---' '---          
                                                                             
# O Cálculo Lambda

- De assustador só tem o nome

- Basicamente uma linguagem com apenas 1 tipo: lambdas
    
    λvar.corpo <- isso é um lambda sz

- A única operação disponível é "substituição". Exemplo:

    (λx.[1, 2, x, 3, 4] 900)
    ------------------------ vira...
    [1, 2, 900, 3, 4]

- E é isso. Acabou. É literalmente só isso. Aaaa

- Turing Completo: "capaz de implementar qualquer coisa"

- Você já usa e nem sabe disso! Quer ver?

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

> Cálculo Lambda!

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

- TODO conceito pode ser representado, de uma forma ou de outra

- Diversas maneiras de representar cada conceito, com diferentes méritos

- Difícil? Talvez! Por isso compilamos linguagens mais simples pro λC.

# Do Cálculo Lambda para Interaction Combinators

          Lambda Calculus => Interaction Combinator
          =========================================

      X = λvar. body                X = (fun arg)
      --------------                -------------   
    
       var --|\                       X --|\
             | |-- X                      | |-- fun
      body --|/                     arg --|/
                                                     
                                                     
  *"um lambda vira um CON"*     *"uma aplicação vira um CON"*

        λx. (vários x)              λx. (nenhum x)
        --------------              --------------  
        
          x --|\
              |#|-- x                    O-- x
          x --|/

  *"duplicamos vars com DUPs"*  *"apagamos vars com ERAs"*

# HVM: a receita completa

- 1. Pegamos um programa de alto nível

- 2. Transformamos em um termo lambda: **lang -> λterm**

- 3. Transformamos em interaction combinators: **λterm -> inet**

- 4. Aplicamos **regras de interação paralelas**, até acabar: **eval(inet)**

- 5. Convertemos de volta pro cálculo lambda: **inet -> λterm**

- 6. Convertemos de volta pra a linguagem original: **λterm -> lang**

- 7. Printamos resultados, lançamos chamadas de sistema, etc.

# Exemplo completo

> 1. Pegamos um programa de alto nível

```python
def dobro(x):
    return x * 2

print(dobro(1))
```

# Exemplo completo

> 2. Transformamos em um termo lambda

```haskell
let dobro = λx.
    λs.λz.((x λk(s (s k))) z)

(dobro λs.λz.(s z))
```

# Exemplo completo

> 3. Transformamos em interaction combinators

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

# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

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
                         ........:         

# Exemplo completo

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
                                           
# Exemplo completo

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

# Exemplo completo

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

# Exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                           
                                           
                               X           
                               :           
                               λs..        
                                  λz.      
                                  (@).     
                                  : (@)    
                                  s : :    
                                    s z    

# Exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                                
                                                
                               X                
                               :                
                               λs.λz.(@).(@)..z
                                      :   :    
                                      s   s    
                                                
                                                
                                                
                                                
                                                
                                                

# Exemplo completo

> 5. Convertemos de volta pro cálculo lambda

                                                   
                                                   
                               X...λs.λz.(@).(@)..z
                                          :   :    
                                          s   s    
                                                   
                                                   
                                                   
                                                   
                                                   
                                                   

# Exemplo completo

> 5. Convertemos de volta pro cálculo lambda



                               X = λs.λz.(s (s z))

# Exemplo completo

> 6. Convertemos de volta pra a linguagem original


```python
                               X = 2
```

# Exemplo completo

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

