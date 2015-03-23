# Introduction #

Many tasks are simply based on reading/reformatting/writing text files.

# Details #

Create a template application:
```
ghmake app cli myLexer
```

Build the new template application:
```
cd myLexer
qmake
make
```

Now, lets add in some functionality:
```
vi myApp.cpp
```

You will see that the constructor opens stdout and stderr and there is a single doit() method which will open the "-i" argument using Ifp. The template uses nextLine() to read each line of the input file. The first step is to add a Lexer and in this example, we will be using strings so a Token class will be used.

```
        Token   hdr, tok;
        tok.next( ifp );
        hdr = tok;
        while( tok.next( ifp ) ) {
                for( int i = 0; i < hdr.size(); ++i ) {
                        cout << hdr.at( i );
                        cout <<  "\t";
                        if( i < tok.size() ) {
                                cout << tok.at( i );
                        } else {
                                cout << "NA";
                        }
                        cout << endl;
                }
        }
```