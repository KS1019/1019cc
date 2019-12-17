try () {
	expected="$1"
	input="$2"

	./1019cc "$input" > tmp.s
	gcc -o tmp tmp.s
	./tmp 
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$expected expected, but got $actual"
		exit 1
	fi
}

try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 41 ' 12 + 34 - 5 ;'
try 47 '5+6*7;'
try 15 '5*(9-6);'
try 4 '(3+5)/2;'
try 10 '-10+20;'
try 10 '- -10;'
try 10 '- - +10;'
try 0  '0==1;'
try 1  '42==42;'
try 1  '0!=1;'
try 0  '42!=42;'

try 1  '0<1;'
try 0  '1<1;'
try 0  '2<1;'
try 1  '0<=1;'
try 1  '1<=1;'
try 0  '2<=1;'

try 1  '1>0;'
try 0  '1>1;'
try 0  '1>2;'
try 1  '1>=0;'
try 1  '1>=1;'
try 0  '1>=2;'

try 3  '1; 2; 3;'

try 14 'return 14;'
try 1  'return 1; 2;'

try 2  'a=1; b=1; return a + b;'
try 1  'a=2; b=2; return a / b;'
try 4  'a=2; b=2; return a * b;'
try 0  'a=2; b=2; return a - b;'

# Failing test cases - because this compiler currently does not handle multiple variables properly. 
# Last value assigned to a variable is used as values for all the variables.
try 6  'a=3; b=2; return a * b;' # You get 4 (2 * 2)
try 3  'a=1; b=2; return a + b;' # You get 4 (2 + 2)
try 1 'a=2; b=1; return a - b;' # You get 0 (2 - 2)
try 2 'a=2; b=1; return a / b;' # You get 1 (1 / 1)


echo OK
