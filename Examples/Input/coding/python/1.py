def f(s,w,h):print s[:w];i=0;exec'print s[~i]+" "*(w-2)+s[w+i];i+=1;'*(h-2);print s[1-h:w+h-3:-1]