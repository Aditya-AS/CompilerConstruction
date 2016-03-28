_recordDemo1 input parameters [record #book d5cc34, record #book d2cd]
output parameters[record #book d3];
d3<--- d5cc34 + d2cd;
return [d3];
end
_main
record #book
type int : edition;
type float: price;
endrecord;
type record #book b2;
type record #book c2;
type record #book d2;
b2.edition <--- 3;
b2.price <--- 24.95;
c2.edition <--- 2;
c2.price <--- 98.80;
[d2]<--- call _function1 with parameters [b2,c2];
print(d2);
end