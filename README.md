# rshell

This is the rshell project for the CS 100 class at UC Riverside.

This project uses the MIT license. See LICENSE for details.

* * *

## rshell

This is the custom shell itself.

#### Features

- execute commands without flags
- type exit to exit
- seperate commands with connectors (;, ||, && [see below for note on connectors])

#### Bugs

- cannot perform input/output redirection or piping.
- cannot include #comments in input
- non-functioning connectors (||, &&)
- if a file is called and cannot be found, typing exit will not work (must type exit again)
- pushing arrow keys to move the cursor and then returning to the original position will result in execvp throwing an error
- basically don't use the arrow keys

* * *

## cp

This is a custom made copy program.

Created in collaboration with [nmlapre](http://github.com/nmlapre) for lab.

#### Features

- see the [ucr-cs100 repository](http://github.com/mikeizbicki/ucr-cs100/tree/cs100-2014fall/assignments/lab/lab5-cp) for details.

* * *

## ls

This is a custom made ls program.

#### Features

- `ls -a` by itself or with any parameters
- `ls -l` by itself or with any parameters

#### Bugs

- cannot use `ls -l ~` or `ls -l ~/`, variations such as `ls -l ~/Documents` works fine
- cannot use `ls -R` by itself or combined


* * *
