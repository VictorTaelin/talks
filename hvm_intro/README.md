# HVM: o Runtime Paralelo

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

# HVM: um programa que roda programas

Rodamos um Radix Sort na HVM, e comparamos com runtimes modernos.

| Runtime         | Tempo   |
|-----------------|---------|
| JavaScript (V8) | 29.081s |
| Haskell (GHC)   | 11.073s |
| Kind (HVM)      | 02.514s |

Como é possível? Já vou explicar! Mas antes...

# A história do paralelismo no hardware

| Ano  | Empresa | Modelo                           | Núcleos    |
|------|---------|----------------------------------|------------|
| 2003 | Intel   | Pentium 4 Extreme 3.2GHz         | 1 núcleo   |
| 2007 | Intel   | Core 2 Extreme QX6800            | 4 núcleos  |
| 2011 | AMD     | FX-8150                          | 8 núcleos  |
| 2015 | Intel   | Core i7-6700K                    | 4 núcleos  |
| 2019 | AMD     | Ryzen 9 3950X                    | 16 núcleos |
| 2023 | AMD     | Ryzen Threadripper 7980X         | 64 núcleos |

> CPUs de Consumo com Maior Número de Núcleos por ano, Intel e AMD, 2003-2023

# A história do paralelismo no software

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

> Algumas pessoas encontram um problema e dizem: "vou usar threads".

> Agora elas tê m doois prlbmeas.

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

# HVM: paralelismo automático ao resgate!

- A **HVM** executa linguagens de **alto nível** com **paralelismo automático**

- **Geral:** suporta todo programa (em contraste com soluções "array-based")

- **Funcional:** lambdas, recursividade, tipos algébricos, pattern-matching...

- **Procedural:** loops, branching, efeitos, "mutabilidade pura"...

- **Promessa** escreva um programa "paralelizável", ganhe um executável paralelo!

- *Para ilustrar, vamos comparar tempos de execução somando árvores!*

# Somando árvores: exemplo

              _________(+)__________           
             /                      \          
         __(+)__                 __(+)__       
        /       \               /       \      
      (+)       (+)           (+)       (+)    
     /   \     /   \         /   \     /   \   
   (+)   (+) (+)   (+)     (+)   (+) (+)   (+) 
   / \   / \ / \   / \     / \   / \ / \   / \ 
   1 6   3 5 1 3   2 3     0 1   7 0 3 1   2 4

# Somando árvores: exemplo

              _________(+)__________           
             /                      \          
         __(+)__                 __(+)__       
        /       \               /       \      
      (+)       (+)           (+)       (+)    
     /   \     /   \         /   \     /   \   
    7     8   4     5       1     7   4     6  

# Somando árvores: exemplo

              _________(+)__________           
             /                      \          
         __(+)__                 __(+)__       
        /       \               /       \      
       15        9             8        10

# Somando árvores: exemplo

              _________(+)__________           
             /                      \          
           24                       18         

# Somando árvores: exemplo

                       42

# Somando árvores - Python

```python
def sum(t: dict) -> int:
    if t["tag"] == "Leaf":
        return t["x"]
    else:
        return sum(t["x0"]) + sum(t["x1"])

print(sum(gen(24)))
```

# Somando árvores - JavaScript

```javascript
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

# Por que a HVM rodou mais rápido?

[ ] Ela foi feita pelo GPT-6

[ ] Algoritmos diferentes

[X] Ela usou 12 núcleos 😬

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

- HVM perde no single-core, e usar threads manualmente em C = mais rápido

- Então pra que? *Paralelismo correto, sem esforço, em linguagens alto nível*

- Suporta features quase impossíveis de paralelizar na mão, ex: closures

- Exemplos mais complexos em **https://github.com/HigherOrderCO/HVM-Core**

# HVM: a receita

- 1. Pegamos um programa de alto nível (Python, JavaScript, etc.)

- 2. Transformamos em um termo do **Cálculo Lambda**

- 3. Transformamos em um grafo de **Combinadores de Interação**

- 4. Aplicamos **regras de reescrita paralelas**, até acabar

- 5. Convertemos de volta para um termo do **Cálculo Lambda**

- 6. Convertemos de volta pra linguagem original

- 7. Printamos resultados, lançamos chamadas de sistema, etc.

**Muito fácil!**

> ????????? ??      ? ?̵̦͈̇̀͂͝͠ ?           ?

> Ok, vamos por partes...

# O que é o Cálculo Lambda?

- O Cálculo Lambda é uma linguagem que contém apenas lambdas

- Um lambda é uma função anônima. Exemplo:

    Na matemática, temos...

    f(x) = x * 2

    No cálculo lambda, temos...

    λx. (x * 2)

    É a mesma coisa, só que sem nome!

- De assustador só tem o nome. De fato, VOCÊ JÁ USA ELE!

# Você já usa o Cálculo Lambda!

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

# Você já usa o Cálculo Lambda!

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

# Você já usa o Cálculo Lambda!

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

# Você já usa o Cálculo Lambda!

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

# Você já usa o Cálculo Lambda!

> Tiramos os números: Cálculo Lambda! **Python = Lambda + Coisas**

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

# É possível transformar QUALQUER linguagem no Cálculo Lambda!

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
| indexação   | list[2]            | (head (2 tail list))                 |
| loop        | while x: f()       | (Y λW λc λs λf (if (c s) (W c (f ... |
| efeitos     | print("oi")        | monadic binding to external env...   |

- Difícil? Talvez! Mas você não precisar usar o ele diretamente, ta bom?

# Mas o que λs tem a ver com paralelismo?

- Me prometeram que "funcional = paralelo" 🥳 

- Mas a regra de substituição β não é atômica... fui iludido?

- E agora? Bem... existe um OUTRO modelo que é realmente paralelo.

- **P:** *"Quais são as regras fundamentais da computação?"* - Yves Lafont, 1997

- **R:** *"Combinadores de Interação"* - Yves Lafont, 1997

- Interessante. Let's go deeper...

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


# É possível transformar o Cálculo Lambda nos Interaction Combinators!

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

# E agora?

- Se qualquer linguagem pode virar o Cálculo Lambda...

- Se o Cálculo Lambda pode virar Interaction Combinators...

- Se Interaction Combinators podem ser processados em paralelo...

- Então **qualuer linguagem pode ser processada em paralelo!**

> Desde que o programa não seja "inerentemente sequencial"

# SOCORRO MUITA INFORMAÇÃO

Eu sei. Calma.

É igual explicar boardgame...

Fica mais fácil na prática.

*respira*

*Que tal um exemplo prático?*

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
    λS.λZ.((x λk(S (S k))) Z)

X = (dobro λs.λz.(s z))
```

# HVM: exemplo completo

> 3. Transformamos em Interaction Combinators

                .............              
                :           :              
               (@)          λx             
          .....: :  x        :..           
          :      X  :          λS
          λs       (@)          :..        
      s    :..     : :.           :        
      :      :     λk :           λZ       
     (@)     λz     : :   S   S    :...... 
     : :      :     : :   :   :          : 
     z :      :     : :  (@) (@)    .... : 
       :......:     : : k: :.: :   (@) : : 
                    :.:........:  Z: :.:.: 
                      :................:   

# HVM: exemplo completo

> 3. Transformamos em Interaction Combinators

                .............              
                :           :              
               (@)          λx             
          .....: :  .......: :..           
          :      X  :          λs
          λs       (@)      ..: :..        
      ...: :..     : :.     :     :        
      :      :    (@) :    /#\    λz       
     (@)     λz   : : :   .: :.  : :...... 
     : :    : :   : : :   :   :  :       : 
     :.:....: :   : : :  (@) (@) :  .... : 
       :......:   :.:.:..: :.: : : (@) : : 
                    :.:........: :.: :.:.: 
                      :................:   


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

> "Tudo isso para multiplicar um número?"

- Pointeiros de 64-bit: 128 bits por lambda!

- Números "u60" unboxed, com operações nativas

- Arrays representadas com árvores: 46% de eficiência

- Tipos algébricos representados com Scott-Encodings

- Definições globais compiladas para código nativo

- Diversas otimizações => boa performance real

- Muito a melhorar, porém já é competitivo


# Resumo

- Processadores estão ficando cada vez mais paralelos

- Programas, em geral, não tem acompanhado a trend

- Motivo: programar paralelamente é difícil e custoso

- Abordagens anteriores para paralelismo automático: pouco êxito

- HVM = paralelismo automático com resultados promissores

- Baseada em um modelo computacional projetado para paralelismo

- Receita: **lang -> λterm -> icomb -> eval() -> λterm -> lang**

- Rodar programas paralelamente, via icombs, traz ganhos expressivos

- Pretendemos dar suportes a múltiplas linguagens futuramente

# Participe!

A HOC é uma empresa brasileira inovando a computação.

Temos uma comunidade de pessoas apaixonadas pela área.

Entusiastas de todos os níveis são bem-vindos!

https://HigherOrderCO.com/

~~~














Benchmarks e programas mostrados disponíveis em:
https://github.com/VictorTaelin/talks/

# Obrigado!                                                                                     ᵖʸᵗʰᵒⁿ
                                                                                                   │
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠲⠶⣶⣶⣤⣄⣀⠀⠀⠀⠀                                                                              │⠀⠀
⠀⠀⠀⠀⠀⣤⣄⠀⠀⠀⠀⠀⠀⠈⠙⢿⣿⣷⣦⡀⠀⠀⠀                                                                            │
⠀⠀⠀⠀⠀⢹⣿⣿⣦⣀⣀⣀⣠⣤⣤⣤⡽⢿⣿⣿⣦⡀⠀     thanks thanks                                                          │
⠀⠀⠀⠀⠀⢀⣿⣿⠿⠿⣿⣿⣿⣿⣿⡿⠀⠈⣿⣿⣿⣷⡀                                                                            │
⠀⠀⠀⠀⠀⠸⢱⢹⣷⠞⢛⣿⣿⣿⡿⠁⠀⠀⢹⣿⣿⣿⣧         much agradecido                                                    │
⠀⠀⠀⠀⠀⢿⣾⣿⣇⡀⠧⢬⣿⣿⠁⠀⠀⠀⢸⣿⣿⣿⣿                                                                            │
⡄⠀⠀⠀⠀⠈⠻⢿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⣿⣿⣿⣿⣿                                                                            │
⢻⣄⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣦⡀⠀⠀⣼⣿⣿⣿⣿⡟    thanks yous                                                             │
⠈⢿⣦⣀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⡿⠁                                                                            │
⠀⠈⠻⣿⣷⣶⣤⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀         very arigatou                                                      │
⠀⠀⠀⠈⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀                                                                            │
⠀⠀⠀⠀⠀⠀⠉⠙⠛⠿⠿⠿⠿⠿⠛⠋⠉⠀⠀⠀⠀ // ?         ? / /!!                                                         │
                                                                                                   │
Minhas redes:                                                                                      │
                                                                                                   │
- Twitter: @VictorTaelin                                                                           │
                                                                                                   │
- Instagram: @VictorTaelin                                                                         │
                                                                                                   │
- GitHub: @VictorTaelin                                                                            │
                                                                                                   │
- Reddit: /u/SrPeixinho                                                                            │ 
                                                                                                   │
                                                                                                  ...
-------------------------------------------------------------------------------------------------------
