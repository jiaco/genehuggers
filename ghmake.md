# Introduction #

ghmake server 2 functions
  1. create a template application structure
  1. create an nsi script for a windows installer

# Details #

ghmake usage:

```
ghmake: [ command ] [ option(s) ]
command = { app }
   app option(s) = type app-name
    - window creates a window template app in pwd
    - wizard creates a wizard template app in pwd
    - cli creates a cli template app in pwd
   nsi [ no-options ]
    - nsi creates a Windows nsi installer for app in pwd
```