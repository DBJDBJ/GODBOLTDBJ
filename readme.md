# GODBOLT DBJ 
## Savings, trimmings and leftovers

&copy; 2022 by dbj@dbj.org, [CC BY SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)

Why these? I am not entirely sure. And these cpp things in here, they do not matter too. In case you need them you need to dive into `.vscode`. Not very unusual task for you since you are here in the first place.

Human kind still has no answer to this question: If you use [CLANG](https://en.wikipedia.org/wiki/Clang) and you build *.c or *.cpp files with no `-std=` switch on the command line what version of C and what version of C++ are you using? Ah yes intrinsic macros and the rest, but are they all really giving you the right answer? [Prove me wrong](https://releases.llvm.org/15.0.0/tools/clang/docs/UsersManual.html#c-language-features).

## common.h

As any of your own "commons" it is a result of a futile attempt to keep it short and simple. Currently it forbids (or so it seems) C++ code.

## What CLANG?

The one that comes with Visual Studio Community Edition. Always x64, too.

## .vscode folder is in here too

It matches my PC setup. Which is bog standard and `c:\\` based. If yours is not, then again please dive into `./vscode`.

## The Roadmap
[cosmopolitan libc](https://justine.lol/cosmopolitan/windows-compiling.html) : your build-once run-anywhere c library . And yes, OS is not needed too.

--- 

All questions or hate mail send to dbj@dbj.org