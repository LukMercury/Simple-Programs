### Algorithm Efficiency Calculator

---

Being given an ascending function f(x) defined on [0,...)   
and a double n  
You have to find the maximum integer x such that f(x) < n still holds   

Useful for computing algorithm speeds: input size x based on maximum execute time n  
and algorithm comparison: input size x such that algorithm 1 runs faster than algorithm 2    

---

#### Usage:  

aec	   			

​	*- interactive mode*  

aec <input_file>	  

​	*- batch mode*  

---

#### Input line format:

\<function> : \<reference value n> : \<lower search boundary> : \<upper search boundary>   

Ctr-D (Linux) / Ctrl-Z (Windows) to signal end of input  

  

Examples:  

​	x * log10(x) : 1000000 : 0 : 1000000  

​	pow(x, 2) : 1000000 : 0 : 1001  

​	pow(2, x) : 1000000 : 0 : 100  

​	fact(x) : 1000000 : 0 : 50   

---

Output "-1" means no solution found for given conditions   

---

