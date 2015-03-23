# Introduction #

Add your content here.


# Details #

  * ll options either begin with at least one '-' or contain an '=' character.
  * ptions are parsed from the MyApp::Usage static QString.
Options consist of name value pairs.
  * he smallest unique string from the complete set of options can be used to select each option name
  * sing either the -o --option style. Longer strings that still uniquely specify an option are also valid.
  * oolean options are true by default but can also be specified as t,f,true,false,T,F,0,1 etc...

example:
```
GhProgram -i input.file output=output.file -r T --op 123 or=4.023
```