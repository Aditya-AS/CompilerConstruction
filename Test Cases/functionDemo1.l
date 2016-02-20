_function1
input parameters [int c2, int d2cd]
output parameters[int b5d, int d3];
b5d<---c2+234-d2cd;
d3<---b5d+20;
return [b5d, d3];
end
_main
type int: b4d333;
type int : c3ddd34;
type int:c2d3;
type int c2d4;
read(b4d333);
read(c3ddd34);
[c2d3, c2d4]<--- call _function1 with parameters [b4d333, c3ddd34];
print(c2d3); 
print(c2d4);
end