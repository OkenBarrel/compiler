10
P' P L S E E' C T T' F

20
; = id if then else while do > < + - * / ( ) int8 int10 int16 ~

24
P' -> P
P -> L
P -> L P
L -> S ;
S -> id = E
S -> if C then S
S -> if C then S else S
S -> while C do S
C -> E > E
C -> E < E
C -> E = E
E -> T E'
E' -> + T E'
E' -> - T E'
E' -> ~
T -> F T'
T' -> * F T'
T' -> / F T'
T' -> ~
F -> ( E )
F -> id
F -> int8
F -> int10
F -> int16

P'

while ( id + int10 ) > int16 do if id = int8 then while id < id do id = id * id / id ; #