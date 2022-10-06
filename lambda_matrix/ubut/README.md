<h1><font size="13">ubut&trade;</font></h1>
ubench + utests = ubut

<font size="1">(it also creates unique and somewhat ridiculously sounding name)</font>

Inspired by [Sheredom](https://twitter.com/sheredom) [UTEST](https://github.com/sheredom/utest.h) and [UBENCH](https://github.com/sheredom/ubench.h)
Please review and respect the copyright and licensing from those repositories.

## Why?

This is now single platform, and that platform is Windows.

When using (otherwise very good) `ubench` and `utest`, I experienced issue with Windows platform. Instead of fixing them in two places, I decided to have a single source for common stuff.  

Yes, I know that complicates dependency (no more single header) and delivers SPOF's (Single Point Of Failure). But it is also far easier to manage and if things are done properly SPOF become SPOX aka Single Point Of eXcelence.

Also a lot of complex stuff is now removed since there is only onw platform supported.

## Caveat Emptor

- soft dependency on [dbj--simplelog](https://github.com/dbj-systems/dbj--simplelog)
  - I repeat: that is a soft dependency
    - look into [ubut_print.h](ubut_print.h)
    - I have [windows syslog lib](https://github.com/dbj-data/dbjsysloglib), I plan to use in there too, as an option.
- This is **Windows only**, but not pathologically so
  - [cmd.exe console colouring](win_lib.h) solution imposes Windows 10.0.14393  as a minimal version requirement.
  - It turns out that *might* be redundant
  - Just place `system(" ")` in your main (that is a single space in there)
- Originals feature"fixtures" is removed
- both `UBENCH_MAIN()` and `UTEST_MAIN()` are removed. Please see the comment on the bottom of `ubench.h` and `utest.h`

Having both UTEST and UBENCH function in the same project might or might not work. Testing of that is in motion.
